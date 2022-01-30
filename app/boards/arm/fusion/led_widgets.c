#include <device.h>
#include <drivers/led.h>
#include <zmk/event_manager.h>
#include <zmk/keymap.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_selection_changed.h>
#include <zmk/endpoints_types.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(led_widgets, 4);

typedef enum {
    LED_EVENT_BOOT = 0,
    LED_EVENT_BATTERY,
    LED_EVENT_LAYER,
    LED_EVENT_OUTPUT,
    LED_EVENT_PROFILE,
    LED_EVENT_CONN,
    LED_EVENT_SIZE,
} led_event_type_t;

typedef enum {
    LED_ENDPOINT_CONN = 0,
    LED_ENDPOINT_DISCONN,
} led_endpoint_connected_t;

typedef enum {
    LED_STATE_IDLE = 0,
    LED_STATE_PAUSE,
    LED_STATE_ACTIVE,
} led_state_t;

static const struct device *leds = DEVICE_DT_GET(DT_NODELABEL(leds));
#define _F(x) 1
#define NARG(x) FOR_EACH(_F, (+), LIST_DROP_EMPTY(x))
#define NUM_LEDS UTIL_EVAL(NARG(DT_SUPPORTS_DEP_ORDS(DT_NODELABEL(leds))))

typedef struct {
    uint8_t brightness[NUM_LEDS];
    uint16_t timeout;   // 5ms units
} led_cmd_t;

typedef struct {
    uint8_t arg;
    uint8_t priority;
    uint16_t period;    // 5ms units; scheduled in at least this amount of time
    uint8_t cmd_len;
    led_cmd_t commands[5];
} led_widget_t;

struct led_pwm_config {
	int num_leds;
	const void *led;
};

#define TICK_MS_RATIO 5
#define PAUSE_TIMEOUT_MS 500
#define PROFILE_COUNT (CONFIG_BT_MAX_PAIRED - 1)
#define MS_TO_TICK(m) ((m) / TICK_MS_RATIO)
#define TICK_TO_MS(t) ((t) * TICK_MS_RATIO)
#define S_TO_TICK(s) MS_TO_TICK((s) * 1000)
#define LED_ACTIVE_WIDGET_GET(i) (widgets[i][active_widgets_ind[i]])

static void led_widget_work_cb(struct k_work *work);
static K_DELAYED_WORK_DEFINE(led_widget_work, led_widget_work_cb);
static led_state_t state = LED_STATE_IDLE;
static int8_t active_widget_type = -1;
static int8_t active_widgets_ind[LED_EVENT_SIZE];
static int8_t last_widgets_ind[LED_EVENT_SIZE];
static uint8_t led_cmd_ind = 0;
static struct k_timer loop_timers[LED_EVENT_SIZE];
static bool loop_timer_started[LED_EVENT_SIZE];

#define NONE { 0, 0, 0 }
static const led_widget_t widgets[][4] = {
    [LED_EVENT_BOOT] = {
        { 0, 100, 0, 3, { { { 0, 0, 50 }, MS_TO_TICK(100) }, { NONE, MS_TO_TICK(100) }, { { 0, 0, 50 }, MS_TO_TICK(100) }, }, },
    },
    [LED_EVENT_BATTERY] = {
        { 30, 50, S_TO_TICK(30), 2, { { { 0, 20, 0 }, MS_TO_TICK(500) }, { { 100, 0, 0 }, MS_TO_TICK(50) }, }, },
        { 70, 50, S_TO_TICK(30), 2, { { { 0, 50, 0 }, MS_TO_TICK(50) }, { { 100, 0, 0 }, MS_TO_TICK(50) }, }, },
        { 100, 50, S_TO_TICK(30), 1, { { { 0, 100, 0 }, MS_TO_TICK(50) }, }, },
    },
    [LED_EVENT_LAYER] = {
        { 1, 20, 0, 1, { { { 0, 0, 20 }, 0 }, }, },
        { 2, 20, 0, 1, { { { 0, 20, 0 }, 0 }, }, },
        { 3, 20, 0, 1, { { { 0, 0, 50 }, 0 }, }, },
        { 4, 20, 0, 1, { { { 50, 0, 0 }, 0 }, }, },
    },
    [LED_EVENT_OUTPUT] = {
        { ZMK_ENDPOINT_BLE, 100, 0, 1, { { { 0, 0, 100 }, MS_TO_TICK(200) }, }, },
        { ZMK_ENDPOINT_USB, 100, 0, 1, { { { 0, 75, 0 }, MS_TO_TICK(200) }, }, },
    },
    [LED_EVENT_PROFILE] = {
        { 0, 90, 0, 3, { { { 0, 0, 50 }, MS_TO_TICK(50) }, { NONE, MS_TO_TICK(150) }, { { 100, 0, 0 }, MS_TO_TICK(100) }, }, },
        { 1, 90, 0, 3, { { { 0, 0, 50 }, MS_TO_TICK(50) }, { NONE, MS_TO_TICK(150) }, { { 0, 100, 0 }, MS_TO_TICK(100) }, }, },
        { 2, 90, 0, 3, { { { 0, 0, 50 }, MS_TO_TICK(50) }, { NONE, MS_TO_TICK(150) }, { { 0, 0, 100 }, MS_TO_TICK(100) }, }, },
    },
    [LED_EVENT_CONN] = { },
};

static bool widget_is_status(const led_widget_t *widget) {
    return widget->cmd_len == 1 && widget->commands[0].timeout == 0;
}

static void led_off_all() {
    LOG_DBG("off");
    for (uint8_t i = 0; i < NUM_LEDS; i ++) {
        led_off(leds, i);
    }
}

static void run_widget_cmd(const led_event_type_t ev, const uint8_t cmd_ind) {
    const led_widget_t *active_widget = &LED_ACTIVE_WIDGET_GET(ev);
    const uint8_t cmd_len = active_widget->cmd_len;
    const led_cmd_t *cmd = &active_widget->commands[cmd_ind];
    if (cmd_ind == 0) {
        LOG_DBG("run %u", ev);
        const uint16_t period = active_widget->period;
        if (period > 0) {
            LOG_DBG("resched %u", TICK_TO_MS(period));
            if (!loop_timer_started[ev]) {
                k_timer_start(&loop_timers[ev], K_MSEC(TICK_TO_MS(period)), K_MSEC(TICK_TO_MS(period)));
                loop_timer_started[ev] = true;
            }
        } else {
            k_timer_stop(&loop_timers[ev]);
            loop_timer_started[ev] = false;
        }
    }
/* #define _FMT(_i, _a) " %u" */
/* #define _ARG(i, _a) cmd->brightness[i] , */
/*     LOG_DBG("led" UTIL_LISTIFY(NUM_LEDS, _FMT), UTIL_LISTIFY(NUM_LEDS, _ARG)); */
/* #undef _FMT */
/* #undef _ARG */
    LOG_DBG("led %u %u %u", cmd->brightness[0], cmd->brightness[1], cmd->brightness[2]);
    for (uint8_t i = 0; i < NUM_LEDS; i ++) {
        led_set_brightness(leds, i, cmd->brightness[i]);
    }
    if (cmd->timeout > 0) {
        LOG_DBG("wait %u", TICK_TO_MS(cmd->timeout));
        k_delayed_work_submit(&led_widget_work, K_MSEC(TICK_TO_MS(cmd->timeout)));
    }
    active_widget_type = ev;
    if (cmd_len == cmd_ind + 1) {
        LOG_DBG("-> idle");
        state = LED_STATE_IDLE;
        return;
    }
    LOG_DBG("-> active");
    state = LED_STATE_ACTIVE;
    led_cmd_ind = cmd_ind;
}

static void led_widget_pause() {
    LOG_DBG("-> pause");
    led_off_all();
    state = LED_STATE_PAUSE;
    k_delayed_work_submit(&led_widget_work, K_MSEC(PAUSE_TIMEOUT_MS));
}

static void led_widget_work_cb(struct k_work *_work) {
    switch (state) {
    case LED_STATE_IDLE:
        LOG_WRN("IDLE");
        led_off_all();
        LOG_DBG("last[%d] <- %d", active_widget_type, active_widgets_ind[active_widget_type]);
        last_widgets_ind[active_widget_type] = active_widgets_ind[active_widget_type];
        active_widgets_ind[active_widget_type] = -1;
        active_widget_type = -1;
        uint8_t max_priority = 0;
        /* LOG_DBG("actives %d %d %d %d %d", active_widgets_ind[0], active_widgets_ind[1], active_widgets_ind[2], active_widgets_ind[3], active_widgets_ind[4]); */
        for (uint8_t i = 0; i < LED_EVENT_SIZE; i ++) {
            if (active_widgets_ind[i] != -1 && LED_ACTIVE_WIDGET_GET(i).priority > max_priority) {
                max_priority = LED_ACTIVE_WIDGET_GET(i).priority;
                active_widget_type = i;
            }
        }
        if (active_widget_type != -1) {
            LOG_DBG("next %d", active_widget_type);
            led_widget_pause();
        }
        break;
    case LED_STATE_PAUSE:
        LOG_WRN("PAUSE");
        run_widget_cmd(active_widget_type, 0);
        break;
    case LED_STATE_ACTIVE:;
        LOG_WRN("ACTIVE");
        led_off_all();
        run_widget_cmd(active_widget_type, led_cmd_ind + 1);
        break;
    }
}

static void led_widget_schedule(const led_event_type_t ev, const uint8_t widget) {
    LOG_DBG("sched %u %u", ev, widget);
    /* LOG_DBG("actives %d %d %d %d %d", active_widgets_ind[0], active_widgets_ind[1], active_widgets_ind[2], active_widgets_ind[3], active_widgets_ind[4]); */
    if (active_widgets_ind[ev] == widget && widget_is_status(&LED_ACTIVE_WIDGET_GET(ev))) {
        return;
    }
    active_widgets_ind[ev] = widget;
    /* LOG_DBG(">actives %d %d %d %d %d", active_widgets_ind[0], active_widgets_ind[1], active_widgets_ind[2], active_widgets_ind[3], active_widgets_ind[4]); */
    if (active_widget_type > 0) {
        LOG_WRN("active %u", active_widget_type);
        if (state == LED_STATE_PAUSE
                || LED_ACTIVE_WIDGET_GET(ev).priority < LED_ACTIVE_WIDGET_GET(active_widget_type).priority) {
            return;
        }
        if (widget_is_status(&LED_ACTIVE_WIDGET_GET(ev))) {
            led_off_all();
            run_widget_cmd(ev, 0);
            return;
        }
        active_widget_type = ev;
        led_widget_pause();
    } else {
        run_widget_cmd(ev, 0);
    }
}

static void loop_timer_handler(struct k_timer *timer) {
    const led_event_type_t ev = (timer - loop_timers) / sizeof(struct k_timer) + 1;
    LOG_DBG("loop %u %d", ev, last_widgets_ind[ev]);
    led_widget_schedule(ev, last_widgets_ind[ev]);
}

#define widget_handler(TYPE, EV, MEMBER, EXPR, CMP, MSG) \
    const struct TYPE *EV##_ev = as_##TYPE(ev); \
    if (EV##_ev) { \
        const uint8_t match = COND_CODE_0(IS_EMPTY(MEMBER), (EV##_ev->MEMBER), (EXPR)); \
        LOG_WRN(MSG, match); \
        for (uint8_t i = 0; i < ARRAY_SIZE(widgets[LED_EVENT_##EV]); i ++) { \
            if (match CMP widgets[LED_EVENT_##EV][i].arg) { \
                led_widget_schedule(LED_EVENT_##EV, i); \
                return ZMK_EV_EVENT_BUBBLE; \
            } \
        } \
        active_widgets_ind[LED_EVENT_##EV] = -1; \
        k_delayed_work_submit(&led_widget_work, K_NO_WAIT); \
        return ZMK_EV_EVENT_BUBBLE; \
    }

static int led_widgets_event_listener(const zmk_event_t *ev) {
    widget_handler(zmk_battery_state_changed, BATTERY, state_of_charge,, <, "bat level %u");
#ifdef CONFIG_ZMK_BLE
    widget_handler(zmk_ble_active_profile_changed, PROFILE, index,, ==, "ble profile %u");
#endif
    widget_handler(zmk_layer_state_changed, LAYER,, zmk_keymap_highest_layer_active(), ==, "layer %u");
    widget_handler(zmk_endpoint_selection_changed, OUTPUT, endpoint,, ==, "endpoint %u");
    return ZMK_EV_EVENT_BUBBLE;
}

static int led_widgets_init() {
    for (uint8_t i = 0; i < LED_EVENT_SIZE; i ++) {
        active_widgets_ind[i] = -1;
        last_widgets_ind[i] = -1;
        k_timer_init(&loop_timers[i], loop_timer_handler, NULL);
    }
    return 0;
}

ZMK_LISTENER(led_widgets_event, led_widgets_event_listener);
ZMK_SUBSCRIPTION(led_widgets_event, zmk_battery_state_changed);
#if defined(CONFIG_USB)
ZMK_SUBSCRIPTION(led_widgets_event, zmk_usb_conn_state_changed);
#endif
#if defined(CONFIG_ZMK_BLE)
ZMK_SUBSCRIPTION(led_widgets_event, zmk_ble_active_profile_changed);
#endif
ZMK_SUBSCRIPTION(led_widgets_event, zmk_layer_state_changed);
ZMK_SUBSCRIPTION(led_widgets_event, zmk_endpoint_selection_changed);

SYS_INIT(led_widgets_init, APPLICATION, CONFIG_ZMK_KSCAN_INIT_PRIORITY);
