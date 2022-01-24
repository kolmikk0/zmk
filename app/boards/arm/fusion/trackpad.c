#include <drivers/sensor.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(trackpad, CONFIG_SENSOR_LOG_LEVEL);

const struct device *trackpad = DEVICE_DT_GET(DT_INST(0, cirque_pinnacle));

static void handle_trackpad(const struct device *dev, struct sensor_trigger *trig) {
    int ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        LOG_ERR("fetch: %d", ret);
        return;
    }
    struct sensor_value dx, dy, wheel;
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
    ret = sensor_channel_get(dev, SENSOR_CHAN_POS_DZ, &wheel);
    if (ret < 0) {
        LOG_ERR("get wheel: %d", ret);
        return;
    }
    LOG_INF("(%d, %d) %d", dx.val1, dy.val1, wheel.val1);
}

static int trackpad_init() {
	struct sensor_trigger trigger = {
		.type = SENSOR_TRIG_DATA_READY,
		.chan = SENSOR_CHAN_ALL,
	};
	if (sensor_trigger_set(trackpad, &trigger, handle_trackpad) < 0) {
        LOG_ERR("can't set trigger");
		return -EIO;
	};
    return 0;
}

SYS_INIT(trackpad_init, APPLICATION, CONFIG_ZMK_KSCAN_INIT_PRIORITY);
