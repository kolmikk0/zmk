#include <drivers/sensor.h>
#include <logging/log.h>

#include <zmk/hid.h>
#include <zmk/endpoints.h>
#include <zmk/keymap.h>
#include <dt-bindings/zmk/mouse.h>

LOG_MODULE_REGISTER(trackpad, CONFIG_SENSOR_LOG_LEVEL);

const struct device *trackpad = DEVICE_DT_GET(DT_INST(0, cirque_pinnacle));

static void handle_trackpad(const struct device *dev, struct sensor_trigger *trig) {
    static uint8_t last_pressed = 0;
    int ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        LOG_ERR("fetch: %d", ret);
        return;
    }
    struct sensor_value dx, dy, btn;
    ret = sensor_channel_get(dev, SENSOR_CHAN_POS_DX, &dx);
    if (ret < 0) {
        LOG_ERR("get dx: %d", ret);
        return;
    }
    ret = sensor_channel_get(dev, SENSOR_CHAN_POS_DY, &dy);
    if (ret < 0) {
        LOG_ERR("get dy: %d", ret);
        return;
    }
    ret = sensor_channel_get(dev, SENSOR_CHAN_PRESS, &btn);
    if (ret < 0) {
        LOG_ERR("get btn: %d", ret);
        return;
    }
    zmk_hid_mouse_movement_set(0, 0);
    zmk_hid_mouse_scroll_set(0, 0);
    const uint8_t layer = zmk_keymap_highest_layer_active();
    uint8_t button;
    if (layer == 1) {   // lower
        zmk_hid_mouse_scroll_update(dx.val1, dy.val1);
        button = RCLK;
    } else {
        zmk_hid_mouse_movement_update(dx.val1, dy.val1);
        button = LCLK;
    }
    if (!last_pressed && btn.val1) {
        zmk_hid_mouse_buttons_press(button);
    } else if (last_pressed && !btn.val1) {
        zmk_hid_mouse_buttons_release(button);
    }
    zmk_endpoints_send_mouse_report();
    last_pressed = btn.val1;
}

static int trackpad_init() {
    struct sensor_trigger trigger = {
        .type = SENSOR_TRIG_DATA_READY,
        .chan = SENSOR_CHAN_ALL,
    };
    printk("trackpad");
    if (sensor_trigger_set(trackpad, &trigger, handle_trackpad) < 0) {
        LOG_ERR("can't set trigger");
        return -EIO;
    };
    return 0;
}

SYS_INIT(trackpad_init, APPLICATION, CONFIG_ZMK_KSCAN_INIT_PRIORITY);
