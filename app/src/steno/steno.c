#include "zmk/steno/steno.h"
#include "zmk/steno/flash.h"
#include "zmk/steno/hist.h"

#include <logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

uint8_t hist_ind = 0;
// Index into `history` that marks how far into the past the translation can go; always less than or
// equal to `hist_ind` or 0xFF
uint8_t stroke_start_ind = 0;
int64_t steno_timestamp = 0;

void steno_process_stroke(uint32_t stroke, int64_t timestamp) {
    steno_timestamp = timestamp;
    if (stroke == 0x1000) { // Asterisk
        hist_ind = HIST_LIMIT(hist_ind - 1);
        hist_undo(hist_ind);
        return;
    }

    history_t *hist = hist_get(hist_ind);
    hist->stroke = stroke;
    // Default `state` set in last cycle
    search_entry(hist_ind);
    hist->bucket = last_bucket;
#ifdef STENO_DEBUG_HIST
    steno_debug_ln("  bucket: %08lX", last_bucket);
#endif
    const uint8_t strokes_len = BUCKET_GET_STROKES_LEN(last_bucket);
    if (strokes_len > 1) {
        hist->state = hist_get(HIST_LIMIT(hist_ind - strokes_len + 1))->state;
    }
#ifdef STENO_DEBUG_HIST
    steno_debug_ln("this %u: scg: %u, %u, %u", hist_ind, hist->state.space, hist->state.cap, hist->state.glue);
#endif
    const state_t new_state = process_output(hist_ind);
#ifdef STENO_DEBUG_HIST
    steno_debug_ln("next %u: scg: %u, %u, %u", HIST_LIMIT(hist_ind + 1), new_state.space, new_state.cap, new_state.glue);
#endif
    if (hist->len) {
#ifdef STENO_DEBUG_HIST
        steno_debug_ln("hist %u:", hist_ind);
        steno_debug_ln("  len: %u, stroke_len: %u", hist->len, BUCKET_GET_STROKES_LEN(hist->bucket));
        const state_t state = hist->state;
        steno_debug_ln("  scg: %u, %u, %u", state.space, state.cap, state.glue);
        char buf[24];
        stroke_to_string(hist->stroke, buf, NULL);
        steno_debug_ln("  stroke: %s", log_strdup(buf));
        if (hist->bucket != 0) {
            steno_debug_ln("  bucket: %08lX", hist->bucket);
        }
#endif
        hist_ind = HIST_LIMIT(hist_ind + 1);
        stroke_start_ind = 0xFF;
    } else {
        stroke_start_ind = hist_ind;
    }
    hist_get(hist_ind)->state = new_state;

#if defined(STENO_DEBUG_HIST) || defined(STENO_DEBUG_FLASH) || defined(STENO_DEBUG_STROKE) || defined(STENO_DEBUG_DICTED)
    steno_debug_ln("----\n");
#endif
}

// Setup the necessary stuff, init SPI flash
int steno_init(void) {
    hist_get(0)->state.cap = CAPS_CAP;
    return flash_init();
}
