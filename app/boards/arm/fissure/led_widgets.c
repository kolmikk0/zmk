#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_selection_changed.h>

typedef enum {
    LED_EVENT_BATTERY = 0,
    LED_EVENT_LAYER,
    LED_EVENT_OUTPUT,
    LED_EVENT_PROFILE,
    LED_EVENT_CONN,
    LED_EVENT_SIZE,
} led_event_type_t;

typedef enum {
    LED_OUTPUT_TYPE_BLE = 0,
    LED_OUTPUT_TYPE_USB,
} led_output_type_t;

typedef enum {
    LED_ENDPOINT_CONN = 0,
    LED_ENDPOINT_DISCONN,
} led_endpoint_connected_t;

typedef enum {
    LED_STATE_IDLE = 0,
    LED_STATE_PAUSE,
    LED_STATE_ACTIVE,
} led_state_t;

typedef struct {
    uint8_t led_no;
    uint8_t brightness;
    uint8_t timeout;    // 25ms units
} led_cmd_t;

typedef struct {
    uint8_t arg;
    uint8_t priority;
    uint16_t period;    // 25ms units; scheduled in at least this amount of time
    led_cmd_t commands[1];
} led_widget_t;

#define TICK_MS_RATIO 25
#define PAUSE_TIMEOUT_MS 200
#define NUM_LEDS (leds->config->num_leds)
#define MS_TO_TICK(m) ((m) / TICK_MS_RATIO)
#define TICK_TO_MS(t) ((t) * TICK_MS_RATIO)
#define S_TO_TICK(s) MS_TO_TICK((s) * SEC_TO_MSEC)
#define LED_ACTIVE_WIDGET_GET(i) (widgets[i][led_widgets_ind[i]])

static led_state_t state = LED_STATE_IDLE;
static int8_t active_widget_ind = -1;
static int8_t active_led_ind = -1;
static void led_widget_work_cb(struct k_work *work);
static K_DELAYED_WORK_DEFINE(led_widget_work, led_widget_work_cb);
static int8_t led_widgets_ind[LED_EVENT_SIZE] = {-1};
static int8_t led_cmd_ind = -1;

static const struct device *leds = DEVICE_DT_GET(DT_NODELABEL(leds));

static const led_widget_t widgets[][] = {
    [LED_EVENT_BATTERY] = {
        { 100, 50, S_TO_TICK(120), { 1, 100, MS_TO_TICK(50) }, },
        { 70, 50, S_TO_TICK(120), { 1, 50, MS_TO_TICK(50) }, { 0, 100, MS_TO_TICK(50) }, },
        { 30, 50, S_TO_TICK(120), { 1, 20, MS_TO_TICK(500) }, { 0, 200, MS_TO_TICK(50) }, },
    },
    [LED_EVENT_OUTPUT] = {
        { LED_OUTPUT_TYPE_BLE, 100, 0, { 2, 200, MS_TO_TICK(100) }, },
        { LED_OUTPUT_TYPE_USB, 100, 0, { 1, 100, MS_TO_TICK(100) }, },
    },
};

static int led_widgets_init() {
    return 0;
}

static void led_widget_work_cb(struct k_work *_work) {
}

static void led_widget_schedule(const led_event_type_t ev) {
    if (active_widget_ind > 0) {
        if (LED_ACTIVE_WIDGET_GET(ev).priority < LED_ACTIVE_WIDGET_GET(active_widget_ind).priority) {
            return;
        }
        for (uint8_t i = 0; i < NUM_LEDS; i ++) {
            led_off(leds, i);
        }
        state = LED_STATE_PAUSE;
        k_delayed_work_submit(led_widget_work, K_MSEC(PAUSE_TIMEOUT_MS));
    } else {
        const uint8_t cmd_len = ARRAY_SIZE(LED_ACTIVE_WIDGET_GET(ev).commands);
        const led_cmd_t *cmd = &LED_ACTIVE_WIDGET_GET(ev).commands[0];
        led_set_brightness(cmd->led_no, cmd->brightness);
        k_delayed_work_submit(led_widget_work, K_MSEC(TICK_TO_MS(cmd->timeout)));
    }
}

static int led_widgets_event_listener(const zmk_event_t *ev) {
    const struct zmk_battery_state_changed *bat_ev = as_zmk_battery_state_changed(ev);
    if (bat_ev) {
        const uint8_t level = bat_ev->state_of_charge;
        for (uint8_t i = 0; i < ARRAY_SIZE(widgets[LED_EVENT_BATTERY]); i ++) {
            if (level < widgets[LED_EVENT_BATTERY][i].arg) {
                led_widgets_ind[LED_EVENT_BATTERY] = i;
                led_widget_schedule(LED_EVENT_BATTERY);
                break;
            }
        }
        return ZMK_EV_EVENT_BUBBLE;
    }
    
#ifdef CONFIG_USB
    const struct zmk_usb_conn_state_changed *usb_ev = as_zmk_usb_conn_state_changed(ev);
    if (usb_ev) {
        const zmk_usb_conn_state conn_state = usb_ev->conn_state;
        return ZMK_EV_EVENT_BUBBLE;
    }
#endif

#ifdef CONFIG_ZMK_BLE
    const struct zmk_ble_active_profile_changed *ble_ev = as_zmk_ble_active_profile_changed(ev);
    if (ble_ev) {
        const uint8_t index = ble_ev->index;
        return ZMK_EV_EVENT_BUBBLE;
    }
#endif

    const struct zmk_layer_state_changed *layer_ev = as_zmk_layer_state_changed(ev);
    if (layer_ev) {
        const uint8_t layer = zmk_keymap_highest_layer_active();
        return ZMK_EV_EVENT_BUBBLE;
    }

    const struct zmk_endpoint_selection_changed *ep_ev = as_zmk_endpoint_selection_changed(ev);
    if (ep_ev) {
        const zmk_endpoint endpoint = ep_ev->endpoint;
        return ZMK_EV_EVENT_BUBBLE;
    }

    return ZMK_EV_EVENT_BUBBLE;
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

SYS_INIT(led_widgets_init, APPLICATION, CONFIG_DEVICE_INIT_PRIORITY);
