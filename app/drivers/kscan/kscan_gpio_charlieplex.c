/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_kscan_gpio_charlieplex

#include <device.h>
#include <drivers/kscan.h>
#include <drivers/gpio.h>
#include <logging/log.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

struct kscan_gpio_item_config {
    char *label;
    gpio_pin_t pin;
};

struct kscan_matrix_config {
    int inst_n;
    int pin_count;
    const struct kscan_gpio_item_config *pins;
};

struct kscan_matrix_data {
    kscan_callback_t callback;
    struct k_timer poll_timer;
    struct k_delayed_work work;
    /* n x n; matrix_state[n][n] is undefined */
    bool *matrix_state;
    const struct device **pins;
    const struct device *dev;                                                                  \
};

#define _KSCAN_GPIO_ITEM_CFG_INIT(n, prop, idx)                                                    \
    {                                                                                              \
        .label = DT_INST_GPIO_LABEL_BY_IDX(n, prop, idx),                                          \
        .pin = DT_INST_GPIO_PIN_BY_IDX(n, prop, idx),                                              \
    },

#define _KSCAN_GPIO_PIN_CFG_INIT(idx, n) _KSCAN_GPIO_ITEM_CFG_INIT(n, gpios, idx)

#define INST_MATRIX_PINS(n) DT_INST_PROP_LEN(n, gpios)

static int kscan_matrix_read(const struct device *dev) {
    struct kscan_matrix_data *data = dev->data;
    const struct kscan_matrix_config *config = dev->config;
    int err;
    const int pin_count = config->pin_count;
    for (int r = 0; r < pin_count; r++) {
        const struct kscan_gpio_item_config *row_pin_config = &config->pins[r];
        err = gpio_pin_configure(data->pins[r], row_pin_config->pin,
                GPIO_OUTPUT_HIGH | GPIO_ACTIVE_HIGH);
        if (err) {
            LOG_ERR("Unable to configure pin %d on %s for output (err %d)",
                    row_pin_config->pin, row_pin_config->label, err);
            return err;
        }
        for (int o = 0; o < pin_count; o ++) {
            if (o == r) {
                continue;
            }
            const struct kscan_gpio_item_config *other_pin_config = &config->pins[o];
            err = gpio_pin_configure(data->pins[o], config->pins[o].pin,
                    GPIO_INPUT | GPIO_ACTIVE_HIGH | GPIO_PULL_DOWN);
            if (err) {
                LOG_ERR("Unable to configure pin %d on %s for input (err %d)",
                        other_pin_config->pin, other_pin_config->label, err);
                return err;
            }
        }
        for (int c = 0; c < pin_count; c++) {
            if (c == r) {
                continue;
            }
            const struct kscan_gpio_item_config *col_pin_config = &config->pins[c];
            err = gpio_pin_get(data->pins[c], col_pin_config->pin);
            if (err < 0) {
                LOG_ERR("Failed to read input pin %d on %s (err %d)",
                        col_pin_config->pin, col_pin_config->label, err);
                return err;
            }
            const bool pressed = err;
            const int ind = r * pin_count + c;
            if (pressed != data->matrix_state[ind]) {
                LOG_DBG("Sending event at %d,%d state %s", r, c, (pressed ? "on" : "off"));
                data->matrix_state[ind] = pressed;
                data->callback(dev, r, c, pressed);
            }
        }
        err = gpio_pin_set(data->pins[r], config->pins[r].pin, 0);
        if (err) {
            LOG_ERR("Failed to set output pin %d on %s inactive (err %d)",
                    row_pin_config->pin, row_pin_config->label, err);
            return err;
        }
    }
    return 0;
}

static void kscan_matrix_timer_handler(struct k_timer *timer) {
    struct kscan_matrix_data *data = CONTAINER_OF(timer, struct kscan_matrix_data, poll_timer);
    k_work_submit(&data->work.work);
}

static void kscan_matrix_work_handler(struct k_work *work) {
    struct kscan_matrix_data *data = CONTAINER_OF(work, struct kscan_matrix_data, work);
    const struct device *dev = data->dev;
    kscan_matrix_read(dev);
}

static int kscan_matrix_init(const struct device *dev) {
    struct kscan_matrix_data *data = dev->data;
    const struct kscan_matrix_config *config = dev->config;
    const struct device **pins = data->pins;
    int err;
    for (int i = 0; i < config->pin_count; i++) {
        const struct kscan_gpio_item_config *pin_config = &config->pins[i];
        pins[i] = device_get_binding(pin_config->label);
        if (!pins[i]) {
            LOG_ERR("Unable to find input GPIO device %s", pin_config->label);
            return -EINVAL;
        }
        err = gpio_pin_configure(pins[i], pin_config->pin, GPIO_INPUT);
        if (err) {
            LOG_ERR("Unable to configure pin %d on %s for input", pin_config->pin, pin_config->label);
            return err;
        } else {
            LOG_DBG("Configured pin %d on %s for input", pin_config->pin, pin_config->label);
        }
    }
    data->dev = dev;
    k_delayed_work_init(&data->work, kscan_matrix_work_handler);
    k_timer_init(&data->poll_timer, kscan_matrix_timer_handler, NULL);
    return 0;
}

static int kscan_matrix_configure(const struct device *dev, kscan_callback_t callback) {
    struct kscan_matrix_data *data = dev->data;
    const struct kscan_matrix_config *config = dev->config;
    if (!callback) {
        return -EINVAL;
    }
    data->callback = callback;
    LOG_DBG("Configured matrix %d", config->inst_n);
    return 0;
};

static int kscan_matrix_enable(const struct device *dev) {
    struct kscan_matrix_data *data = dev->data;
    k_timer_start(&data->poll_timer, K_MSEC(10), K_MSEC(10));
    return 0;
};

static int kscan_matrix_disable(const struct device *dev) {
    struct kscan_matrix_data *data = dev->data;
    k_timer_stop(&data->poll_timer);
    return 0;
};

static const struct kscan_driver_api charlieplex_driver_api = {
    .config = kscan_matrix_configure,
    .enable_callback = kscan_matrix_enable,
    .disable_callback = kscan_matrix_disable,
};

#define GPIO_INST_INIT(n)                                                                          \
    static const struct kscan_gpio_item_config kscan_matrix_config_pins_##n[INST_MATRIX_PINS(n)] = \
            {UTIL_LISTIFY(INST_MATRIX_PINS(n), _KSCAN_GPIO_PIN_CFG_INIT, n)}; \
    static struct kscan_matrix_config kscan_matrix_config##n = { \
        .pins = kscan_matrix_config_pins_##n,   \
        .pin_count = INST_MATRIX_PINS(n),           \
        .inst_n = n, \
    };  \
    static const struct device *kscan_matrix_data_pins_##n[INST_MATRIX_PINS(n)] = \
            {[INST_MATRIX_PINS(n) - 1] = NULL}; \
    static bool matrix_state_##n[INST_MATRIX_PINS(n)][INST_MATRIX_PINS(n)] = { false }; \
    static struct kscan_matrix_data kscan_matrix_data_##n = {           \
        .matrix_state = (bool *) matrix_state_##n, \
        .pins = kscan_matrix_data_pins_##n,         \
    };          \
    DEVICE_AND_API_INIT(kscan_gpio_##n, DT_INST_LABEL(n), kscan_matrix_init,                     \
                        &kscan_matrix_data_##n, &kscan_matrix_config##n, APPLICATION,                 \
                        CONFIG_APPLICATION_INIT_PRIORITY, &charlieplex_driver_api);

DT_INST_FOREACH_STATUS_OKAY(GPIO_INST_INIT)

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */
