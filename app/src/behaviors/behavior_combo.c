/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_combo

#include <device.h>
#include <drivers/behavior.h>
#include <logging/log.h>
#include <sys/dlist.h>

#include <zmk/behavior.h>
#include <zmk/matrix.h>
#include <zmk/endpoints.h>
#include <zmk/event-manager.h>
#include <zmk/events/position-state-changed.h>
#include <zmk/events/keycode-state-changed.h>
#include <zmk/events/modifiers-state-changed.h>
#include <zmk/hid.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

//#if DT_NODE_EXISTS(DT_DRV_INST(0))

// max_keys could be set to the actual number of keys on the board
#define ZMK_BHV_COMBO_MAX_POSITIONS 100

// so many constants are a bit annoying but ¯\_(ツ)_/¯
#define ZMK_BHV_COMBO_MAX_PRESSED_COMBOS 4
#define ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY 5
#define ZMK_BHV_COMBO_MAX_KEYS_PER_COMBO 4

#define ZMK_BHV_COMBO_TIMEOUT_MS 30

typedef struct behavior_combo_config {
    s32_t key_positions[ZMK_BHV_COMBO_MAX_KEYS_PER_COMBO];
    s32_t key_position_len;
    struct zmk_behavior_binding behavior;
    s32_t timeout_ms;
    // if slow release is set, the combo releases when the last key is released.
    // otherwise, the combo releases when the first key is released.
    bool slow_release;
    // the virtual key position is a key position outside the range used by the keyboard.
    // it is necessary so hold-taps can uniquely identify a behavior.
    s32_t virtual_key_position;
    // key_positions_pressed is filled with key_positions when the combo is pressed.
    // The keys are removed from this array when they are released.
    // Once this array is empty, the behavior is released.
    struct position_state_changed *key_positions_pressed[ZMK_BHV_COMBO_MAX_KEYS_PER_COMBO];
} combo;

struct k_delayed_work timeout_task;
s32_t release_at;
// set of keys pressed
struct position_state_changed *pressed_keys[ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY] = {NULL};
// the set of candidate combos based on the currently pressed_keys
combo *candidates[ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY];
// a lookup dict that maps a key position to all combos on that position
combo *combo_lookup[ZMK_BHV_COMBO_MAX_POSITIONS][ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY] = {NULL};
// combos that have been activated and still have (some) keys pressed
// this array is always contiguous from 0.
combo *active_combos[ZMK_BHV_COMBO_MAX_PRESSED_COMBOS] = {NULL};
int active_combo_count = 0;

struct behavior_combo_data {};
static struct behavior_combo_data behavior_combo_data;

/* store the combo key pointer in the combos array, one pointer for each key position */
static int initialize_combo(combo *combo) {
    for (int i = 0; i < combo->key_position_len; i++) {
        s32_t position = combo->key_positions[i];
        // LOG_DBG("combos setting position %d", position);
        for (int j = 0; j < ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY; j++) {
            if (combo_lookup[position][j] == NULL) {
                combo_lookup[position][j] = combo;
                // LOG_DBG("combo_lookup %d %d set", position, j);
                break;
            }
            LOG_ERR("Too many combos for key position %d, ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY %d.",
                    position, ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY);
            return -ENOMEM;
        }
    }
    return 0;
}

static int setup_candidates_for_first_keypress(s32_t position) {
    for (int i = 0; i < ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY; i++) {
        if (combo_lookup[position][i] == NULL) {
            return i;
        }
        candidates[i] = combo_lookup[position][i];
    }
    return ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY;
}

static int filter_candidates(s32_t position) {
    // this code iterates over candidates and the lookup together to filter in O(n)
    // instead of O(n^2)
    int matches = 0, lookup_idx = 0, candidate_idx = 0;
    while (lookup_idx < ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY &&
           candidate_idx < ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY &&
           combo_lookup[position][lookup_idx] != NULL && candidates[candidate_idx] != NULL) {
        s32_t candidate_pos = candidates[candidate_idx]->virtual_key_position;
        s32_t lookup_pos = combo_lookup[position][lookup_idx]->virtual_key_position;
        if (candidate_pos == lookup_pos) {
            candidates[matches] = candidates[candidate_idx];
            matches++;
            candidate_idx++;
            lookup_idx++;
        } else if (candidate_pos > lookup_pos) {
            lookup_idx++;
        } else { // candidate_pos < lookup_pos
            candidate_idx++;
        }
    }
    // clear unmatched candidates
    for (int i = matches; i < candidate_idx; i++) {
        candidates[matches] = NULL;
    }
    return matches;
}

static inline bool candidate_is_completely_pressed(combo *candidate) {
    // this code assumes set(pressed_keys) <= set(candidate->key_positions)
    // this invariant is enforced by filter_candidates
    // the only thing we need to do is check if len(pressed_keys) == len(combo->key_positions)
    return pressed_keys[candidate->key_position_len - 1] != NULL;
}

static int clear_candidates() {
    for (int i = 0; i < ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY; i++) {
        if (candidates[i] == NULL) {
            return i;
        }
        candidates[i] = NULL;
    }
    return ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY;
}

static int capture_pressed_key(struct position_state_changed *ev) {
    for (int i = 0; i < ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY; i++) {
        if (pressed_keys[i] != NULL) {
            continue;
        }
        pressed_keys[i] = ev;
        return ZMK_EV_EVENT_CAPTURED;
    }
    return 0;
}

static void release_pressed_keys() {
    clear_candidates();
    k_delayed_work_cancel(&timeout_task);
    for (int i = 0; i < ZMK_BHV_COMBO_MAX_COMBOS_PER_KEY; i++) {
        if (pressed_keys[i] == NULL) {
            return;
        }
        ZMK_EVENT_RELEASE(pressed_keys[i])
        pressed_keys[i] = NULL;
    }
}

static inline int press_combo_behavior(combo *combo, s32_t timestamp) {
    struct zmk_behavior_binding_event event = {
        .position = combo->virtual_key_position,
        .timestamp = timestamp,
    };

    return behavior_keymap_binding_pressed(&combo->behavior, event);
}

static inline int release_combo_behavior(combo *combo, s32_t timestamp) {
    struct zmk_behavior_binding_event event = {
        .position = combo->virtual_key_position,
        .timestamp = timestamp,
    };

    return behavior_keymap_binding_released(&combo->behavior, event);
}

static void move_pressed_keys_to_combo(combo *combo) {
    for (int i = 0; i < combo->key_position_len; i++) {
        if (pressed_keys[i] == NULL) {
            return;
        }
        combo->key_positions_pressed[i] = pressed_keys[i];
        pressed_keys[i] = NULL;
    }
}

int store_active_combo(combo *combo) {
    for (int i = 0; i < ZMK_BHV_COMBO_MAX_PRESSED_COMBOS; i++) {
        if (active_combos[i] == NULL) {
            active_combos[i] = combo;
            active_combo_count++;
            return 0;
        }
    }
    LOG_ERR("Unable to store combo; already %d active. Increase ZMK_BHV_COMBO_MAX_PRESSED_COMBOS",
            ZMK_BHV_COMBO_MAX_PRESSED_COMBOS);
    return -ENOMEM;
}

/* returns true if a key was released. */
static bool release_combo_key(s32_t position, s64_t timestamp) {
    for (int c = 0; c < active_combo_count; c++) {
        combo *combo = active_combos[c];
        bool key_released = false;
        bool all_keys_pressed = true;
        bool all_keys_released = true;
        for (int i = 0; i < combo->key_position_len; i++) {
            if (combo->key_positions_pressed[i] == NULL) {
                all_keys_pressed = false;
            } else if (combo->key_positions_pressed[i]->position != position) {
                all_keys_released = false;
            } else { // key pressed and position matches
                k_free(combo->key_positions_pressed[i]);
                combo->key_positions_pressed[i] = NULL;
                key_released = true;
            }
        }

        if (key_released) {
            if ((combo->slow_release && all_keys_released) ||
                (!combo->slow_release && all_keys_pressed)) {
                release_combo_behavior(combo, timestamp);
            }
            if (all_keys_released) {
                active_combo_count--;
                active_combos[c] = active_combos[active_combo_count];
                active_combos[active_combo_count] = NULL;
            }
            return true;
        }
    }
    return false;
}

static int position_state_down(struct position_state_changed *ev) {
    int num_candidates;
    bool pre_existing_candidates = candidates[0] != NULL;
    if (pre_existing_candidates) {
        num_candidates = filter_candidates(ev->position);
    } else {
        num_candidates = setup_candidates_for_first_keypress(ev->position);
        if (num_candidates == 0) {
            return 0;
        }
        k_delayed_work_submit(&timeout_task, K_MSEC(ZMK_BHV_COMBO_TIMEOUT_MS));
    }

    switch (num_candidates) {
    case 0:
        release_pressed_keys();
        return position_state_down(ev);
    case 1:;
        combo *combo = candidates[0];
        int ret = capture_pressed_key(ev);
        if (candidate_is_completely_pressed(combo)) {
            // LOG_DBG("combo fully pressed");
            move_pressed_keys_to_combo(combo);
            store_active_combo(combo); // todo: handle ENOMEM
            press_combo_behavior(combo, ev->timestamp);
        }
        return ret;
    default:
        return capture_pressed_key(ev);
    }
}

static int position_state_up(struct position_state_changed *ev) {
    release_pressed_keys();
    if (release_combo_key(ev->position, ev->timestamp)) {
        return ZMK_EV_EVENT_HANDLED;
    } else {
        return 0;
    }
}

static int position_state_changed_listener(const struct zmk_event_header *eh) {
    if (!is_position_state_changed(eh)) {
        return 0;
    }

    struct position_state_changed *ev = cast_position_state_changed(eh);
    if (ev->state) { // keydown
        return position_state_down(ev);
    } else { // keyup
        return position_state_up(ev);
    }
}

ZMK_LISTENER(behavior_combo, position_state_changed_listener);
ZMK_SUBSCRIPTION(behavior_combo, position_state_changed);

static const struct behavior_driver_api behavior_combo_driver_api = {
    .binding_pressed = NULL,
    .binding_released = NULL,
};

static void combo_timeout_handler(struct k_work *item) { release_pressed_keys(); }

static int behavior_combo_init(struct device *dev) {
    k_delayed_work_init(&timeout_task, combo_timeout_handler);
    return initialize_combo((combo *)dev->config_info);
}

#define _TRANSFORM_ENTRY(idx, node)                                                                \
    {                                                                                              \
        .behavior_dev = DT_LABEL(DT_INST_PHANDLE_BY_IDX(node, bindings, idx)),                     \
        .param1 = COND_CODE_0(DT_INST_PHA_HAS_CELL_AT_IDX(node, bindings, idx, param1), (0),       \
                              (DT_INST_PHA_BY_IDX(node, bindings, idx, param1))),                  \
        .param2 = COND_CODE_0(DT_INST_PHA_HAS_CELL_AT_IDX(node, bindings, idx, param2), (0),       \
                              (DT_INST_PHA_BY_IDX(node, bindings, idx, param2))),                  \
    }

#define KP_INST(n)                                                                                 \
    static struct behavior_combo_config behavior_combo_config_##n = {                              \
        .timeout_ms = DT_INST_PROP(n, timeout_ms),                                                 \
        .key_positions = DT_INST_PROP(n, key_positions),                                           \
        .key_position_len = DT_INST_PROP_LEN(n, key_positions),                                    \
        .behavior = _TRANSFORM_ENTRY(0, n),                                                        \
        .virtual_key_position = n + ZMK_BHV_COMBO_MAX_POSITIONS,                                   \
        .slow_release = DT_INST_PROP(n, slow_release),                                             \
    };                                                                                             \
    DEVICE_AND_API_INIT(behavior_combo_##n, DT_INST_LABEL(n), behavior_combo_init,                 \
                        &behavior_combo_data, &behavior_combo_config_##n, APPLICATION,             \
                        CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_combo_driver_api);

DT_INST_FOREACH_STATUS_OKAY(KP_INST)

// #endif