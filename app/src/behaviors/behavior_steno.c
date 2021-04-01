/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_steno

#include <device.h>
#include <drivers/behavior.h>
#include <logging/log.h>
#include <dt-bindings/zmk/steno.h>

#include <zmk/event_manager.h>
#include <zmk/behavior.h>
#include <zmk/steno/steno.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define FLASH_DEVICE DT_LABEL(DT_INST(0, nordic_qspi_nor))

static uint32_t pressed = 0;
static uint32_t current = 0;

static bool steno_key_valid(uint32_t key) {
    return key >= STN__Z && key <= STN_NUM;
}

static int behavior_steno_init(const struct device *dev) {
    return steno_init();
}

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    uint32_t key = binding->param1;
    LOG_DBG("pressed: %u", key);
    if (steno_key_valid(key)) {
        pressed |= 1 << key;
        current |= 1 << key;
        return 0;
    } else {
        return -ENOTSUP;
    }
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    uint32_t key = binding->param1;
    if (steno_key_valid(key)) {
        current &= ~(1 << key);
        if (current == 0) {
            char stroke[32];
            stroke_to_string(pressed, stroke, NULL);
            LOG_WRN("chord pressed: %06X, %s", pressed, log_strdup(stroke));
            steno_process_stroke(pressed, event.timestamp);
            pressed = 0;
        }
        return 0;
    } else {
        return -ENOTSUP;
    }
}

static const struct behavior_driver_api behavior_steno_driver_api = {
    .binding_pressed = on_keymap_binding_pressed, .binding_released = on_keymap_binding_released
};

DEVICE_AND_API_INIT(behavior_steno, DT_INST_LABEL(0), behavior_steno_init, NULL, NULL, APPLICATION,
                    CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_steno_driver_api);
