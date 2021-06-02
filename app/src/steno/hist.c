// History management and output control
#include <ctype.h>
#include "zmk/steno/hist.h"
#include "zmk/steno/steno.h"
#include "zmk/steno/flash.h"

#include <logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static history_t history[HIST_SIZE];
extern int64_t steno_timestamp;

#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>

#include <dt-bindings/zmk/keys.h>
#include <sys/ring_buffer.h>
#include <kernel.h>

typedef struct __attribute__((packed)) {
    bool down;
    uint32_t key;
} key_event_t;

RING_BUF_DECLARE(macro_key_queue, CONFIG_MACRO_BUFFER_SIZE * sizeof(key_event_t));

void macro_key_cb(struct k_timer *timer) {
    key_event_t ev;
    if (ring_buf_get(&macro_key_queue, (uint8_t *) &ev, sizeof(key_event_t))) {
        ZMK_EVENT_RAISE(zmk_keycode_state_changed_from_encoded(ev.key, ev.down, k_uptime_get()));
    }
}

K_TIMER_DEFINE(macro_key_timer, macro_key_cb, NULL);

static void register_code(uint32_t keycode) {
    const key_event_t ev = { .down = true, .key = keycode };
    ring_buf_put(&macro_key_queue, (const uint8_t *) &ev, sizeof(key_event_t));
}

static void unregister_code(uint32_t keycode) {
    const key_event_t ev = { .down = false, .key = keycode };
    ring_buf_put(&macro_key_queue, (const uint8_t *) &ev, sizeof(key_event_t));
}
 
static void tap_code(uint32_t keycode) {
    register_code(keycode);
    unregister_code(keycode);
}

static void steno_back(void) {
    tap_code(BSPC);
}

void macro_init(void) {
    k_timer_start(&macro_key_timer, K_MSEC(CONFIG_MACRO_KEY_INTERVAL_MS), K_MSEC(CONFIG_MACRO_KEY_INTERVAL_MS));
}

// !!! FROM QMK !!!
const char *decode_utf8(const char *str, int32_t *code_point) {
    const char *next;

    if (str[0] < 0x80) {  // U+0000-007F
        *code_point = str[0];
        next        = str + 1;
    } else if ((str[0] & 0xE0) == 0xC0) {  // U+0080-07FF
        *code_point = ((int32_t)(str[0] & 0x1F) << 6) | ((int32_t)(str[1] & 0x3F) << 0);
        next        = str + 2;
    } else if ((str[0] & 0xF0) == 0xE0) {  // U+0800-FFFF
        *code_point = ((int32_t)(str[0] & 0x0F) << 12) | ((int32_t)(str[1] & 0x3F) << 6) | ((int32_t)(str[2] & 0x3F) << 0);
        next        = str + 3;
    } else if ((str[0] & 0xF8) == 0xF0 && (str[0] <= 0xF4)) {  // U+10000-10FFFF
        *code_point = ((int32_t)(str[0] & 0x07) << 18) | ((int32_t)(str[1] & 0x3F) << 12) | ((int32_t)(str[2] & 0x3F) << 6) | ((int32_t)(str[3] & 0x3F) << 0);
        next        = str + 4;
    } else {
        *code_point = -1;
        next        = str + 1;
    }

    // part of a UTF-16 surrogate pair - invalid
    if (*code_point >= 0xD800 && *code_point <= 0xDFFF) {
        *code_point = -1;
    }

    return next;
}
const uint32_t ascii_to_keycode_lut[128] = {
    // NUL   SOH      STX      ETX      EOT      ENQ      ACK      BEL
    0, 0, 0, 0, 0, 0, 0, 0,
    // BS    TAB      LF       VT       FF       CR       SO       SI
    BSPC, TAB,  RET,  0, 0, 0, 0, 0,
    // DLE   DC1      DC2      DC3      DC4      NAK      SYN      ETB
    0, 0, 0, 0, 0, 0, 0, 0,
    // CAN   EM       SUB      ESC      FS       GS       RS       US
    0, 0, 0, ESC,  0, 0, 0, 0,

    //       !        "        #        $        %        &        '
    SPACE,  EXCL,    DQT, HASH,    DOLLAR,    PERCENT,    AMPS,    SQT,
    // (     )        *        +        ,        -        .        /
    LPAR,    RPAR,    STAR,    PLUS,  COMMA, MINUS, DOT,  SLASH,
    // 0     1        2        3        4        5        6        7
    N0,    N1,    N2,    N3,    N4,    N5,    N6,    N7,
    // 8     9        :        ;        <        =        >        ?
    N8,    N9,    COLON, SEMI, LT, EQL,  GT,  QMARK,
    // @     A        B        C        D        E        F        G
    AT,    LS(A),    LS(B),    LS(C),    LS(D),    LS(E),    LS(F),    LS(G),
    // H     I        J        K        L        M        N        O
    LS(H),    LS(I),    LS(J),    LS(K),    LS(L),    LS(M),    LS(N),    LS(O),
    // P     Q        R        S        T        U        V        W
    LS(P),    LS(Q),    LS(R),    LS(S),    LS(T),    LS(U),    LS(V),    LS(W),
    // X     Y        Z        [        \        ]        ^        _
    LS(X),    LS(Y),    LS(Z),    LBKT, BSLH, RBKT, CARET,    UNDER,
    // `     a        b        c        d        e        f        g
    GRAVE,  A,    B,    C,    D,    E,    F,    G,
    // h     i        j        k        l        m        n        o
    H,    I,    J,    K,    L,    M,    N,    O,
    // p     q        r        s        t        u        v        w
    P,    Q,    R,    S,    T,    U,    V,    W,
    // x     y        z        {        |        }        ~        DEL
    X,    Y,    Z,    LBRC, PIPE, RBRC, TILDE,  DEL
};

void send_char(char ascii_code) {
    tap_code(ascii_to_keycode_lut[(uint8_t)ascii_code]);
}

static void steno_send_char(const char c) {
    send_char(c);
#ifdef STENO_DEBUG_HIST
    steno_debug("%c", c);
#endif
}

static uint8_t steno_send_unicode(const uint32_t u) {
#ifdef STENO_DEBUG_HIST
    if (u < 0xFFFF) {
        steno_debug("\\u%04lX", u);
    } else {
        steno_debug("\\U%06lX", u);
    }
#endif
    /* register_unicode(u); */
    return 1;
}

inline history_t *hist_get(const uint8_t ind) {
    return &history[ind];
}

// Undo the last history entry. First delete the output, and then start from the initial state of the
// multi-stage input, and rebuild the output from there.
void hist_undo(const uint8_t h_ind) {
#ifdef STENO_DEBUG_HIST
    steno_debug_ln("hist_undo()");
#endif
    history_t *hist = hist_get(h_ind);
    const uint8_t len = hist->len;
    if (!len) {
        steno_error_ln("bad cur hist entry");
        steno_back();
        return;
    }

#ifdef STENO_DEBUG_HIST
    steno_debug_ln("  back: %u", len);
#endif
    for (uint8_t i = 0; i < len; i++) {
        steno_back();
    }
    const uint8_t strokes_len = BUCKET_GET_STROKES_LEN(hist->bucket);
    const uint8_t repl_len = strokes_len > 1 ? strokes_len - 1 : 0;
    for (uint8_t i = 0; i < repl_len; i++) {
        const uint8_t old_hist_ind = HIST_LIMIT(h_ind + i - repl_len);
        const history_t *old_hist = hist_get(old_hist_ind);
#ifdef STENO_DEBUG_HIST
        steno_debug_ln("  old_hist_ind: %u", old_hist_ind);
#endif
        if (!old_hist->len) {
            hist->len = 0;
            steno_error_ln("bad prev hist entry");
            return;
        }
        process_output(old_hist_ind);
    }
    return;
}

static uint8_t steno_send_keycodes(const uint8_t *keycodes, const uint8_t len) {
#ifdef STENO_DEBUG_HIST
    steno_debug("keys(%u):", len);
#endif
    uint8_t mods = 0;
    for (uint8_t i = 0; i < len; i++) {
        if ((keycodes[i] & 0xFC) == 0xE0) {
            uint8_t mod = keycodes[i] & 0x03;
            uint8_t mod_char;
            switch (mod) {
            case 0: mod_char = 'c'; break;
            case 1: mod_char = 's'; break;
            case 2: mod_char = 'a'; break;
            case 3: mod_char = 'g'; break;
            }
            const uint8_t mod_mask = 1 << mod;
            if (mods & mod_mask) {
                unregister_code(keycodes[i]);
#ifdef STENO_DEBUG_HIST
                steno_debug(" %c", mod_char);
#endif
            } else {
                register_code(keycodes[i]);
#ifdef STENO_DEBUG_HIST
                steno_debug(" %c", toupper(mod_char));
#endif
            }
            mods ^= mod_mask;
        } else {
#ifdef STENO_DEBUG_HIST
            steno_debug(" %02X", keycodes[i]);
#endif
            tap_code(keycodes[i]);
        }
    }
#ifdef STENO_DEBUG_HIST
    steno_debug("\n");
#endif
    return 0;
}

// Process the output. If it's a raw stroke (no nodes found for the input), then just output the stroke;
// otherwise, load the entry, perform the necessary transformations for capitalization, and output according
// to the bytes. Also takes care of outputting key codes and Unicode characters.
state_t process_output(const uint8_t h_ind) {
#ifdef STENO_DEBUG_HIST
    steno_debug_ln("process_output()");
#endif
    // TODO optimization: compare beginning of current and string to replace
    history_t *hist = hist_get(h_ind);
    const uint8_t strokes_len = BUCKET_GET_STROKES_LEN(hist->bucket);
    {
        const uint8_t repl_len = strokes_len > 1 ? strokes_len - 1 : 0;
        for (int8_t counter = repl_len; counter > 0; ) {
            const uint8_t old_hist_ind = HIST_LIMIT(h_ind + counter - repl_len - 1);
            const history_t *old_hist = hist_get(old_hist_ind);
#ifdef STENO_DEBUG_HIST
            steno_debug_ln("  old_hist_ind: %u, bspc len: %u", old_hist_ind, old_hist->len);
#endif
            if (!old_hist->len) {
                hist->len = 0;
                steno_error_ln("bad prev hist entry");
                break;
            }
            for (uint8_t j = 0; j < old_hist->len; j++) {
                steno_back();
            }
            const uint8_t old_strokes_len = BUCKET_GET_STROKES_LEN(old_hist->bucket);
            const uint8_t old_repl_len = old_strokes_len > 1 ? old_strokes_len - 1 : 0;
            counter -= old_repl_len + 1;
        }
    }

    const state_t old_state = hist->state;
    state_t new_state = old_state;
#ifdef STENO_DEBUG_HIST
    steno_debug_ln("  old_state: scg: %u, %u, %u", old_state.space, old_state.cap, old_state.glue);
#endif

    if (hist->bucket == 0) {
#ifdef STENO_DEBUG_HIST
        steno_debug_ln("  stroke: %lX", hist->stroke & 0xFFFFFF);
#endif
        char buf[24];
        if (stroke_to_string(hist->stroke, buf, &hist->len)) {
            new_state.glue = 1;
        }
#ifdef STENO_DEBUG_HIST
        steno_debug("  output: '");
#endif
        if (old_state.space && !(old_state.glue && new_state.glue)) {
            steno_send_char(' ');
            hist->len++;
        }
        for (char *str = buf; *str; str ++) {
            steno_send_char(*str);
        }
#ifdef STENO_DEBUG_HIST
        steno_debug_ln("%s'", log_strdup(buf));
        steno_debug_ln("  -> %u", hist->len);
#endif
        return new_state;
    }

    const uint32_t bucket = hist->bucket;
    read_entry(bucket);
    const uint8_t entry_len = BUCKET_GET_ENTRY_LEN(bucket);
#ifdef STENO_DEBUG_HIST
    steno_debug_ln("  entry_len: %u", entry_len);
#endif

    const attr_t attr = *((attr_t *) &entry_buf[strokes_len * STROKE_SIZE]);
    new_state.space = attr.space_after;
    new_state.glue = attr.glue;
    uint8_t space = old_state.space && attr.space_prev && entry_len && !(old_state.glue && attr.glue);
#ifdef STENO_DEBUG_HIST
    steno_debug_ln("  attr: glue: %u", attr.glue);
    steno_debug("  output: '");
#endif

    uint8_t valid_len = 1, str_len = 0;
    uint8_t set_case;
    const uint8_t *entry = entry_buf + STROKE_SIZE * strokes_len + 1;
    for (uint8_t i = 0; i < entry_len; i++) {
        // Commands
        set_case = 0;
        if (entry[i] < 32) {
#ifdef STENO_DEBUG_HIST
            steno_debug("'\n    ");
#endif
            switch (entry[i]) {
            case 0: // raw bytes of "length"
                space = 0;
                uint8_t len = entry[i + 1];
                uint8_t keycode_len = steno_send_keycodes(entry + i + 2, len);
                if (keycode_len > 0) {
                    str_len += keycode_len;
                } else {
                    valid_len = 0;
                }
                i += len + 1;   // Not + 2 because of increment at the end of the loop
                break;

            case 1: // lowercase next entry
                new_state.cap = CAPS_LOWER;
#ifdef STENO_DEBUG_HIST
                steno_debug_ln("LOWER");
#endif
                break;

            // TODO handle keep cases at the end of entry
            case 2: // Uppercase next entry
                new_state.cap = CAPS_UPPER;
#ifdef STENO_DEBUG_HIST
                steno_debug_ln("UPPER");
#endif
                set_case = 1;
                break;

            case 3: // capitalize next entry
                new_state.cap = CAPS_CAP;
#ifdef STENO_DEBUG_HIST
                steno_debug_ln("CAP");
#endif
                set_case = 1;
                break;

            case 4:; // keep case after "length" amount of characters
                const uint8_t length = entry[i + 1];
#ifdef STENO_DEBUG_HIST
                steno_debug_ln("KEEP(%u)", length);
#endif
                for (int j = 0; j < length; j++) {
                    if (entry[j] >= 32 && entry[j] <= 127) {
                        steno_send_char(entry[j]);
                        i++;
                        str_len++;
                    } else if (entry[i] >= 128) {
                        int32_t code_point = 0;
                        const char *str = decode_utf8((char *) &entry[i], &code_point);
                        if (code_point > 0) {
                            str_len += steno_send_unicode(code_point);
                        }
                        i += str - (char *) &entry[i];
                    }
                }
                new_state.cap = old_state.cap;
                set_case = 1;
                break;

            case 5: // reset formatting
                new_state.cap = CAPS_LOWER;
                break;

            default:
                steno_error_ln("\nInvalid cmd: %X", entry[i]);
                return new_state;
            }
#ifdef STENO_DEBUG_HIST
            steno_debug("    '");
#endif
        } else if (entry[i] < 127) {
            if (space) {
                str_len++;
                steno_send_char(' ');
                space = 0;
            }
            switch (new_state.cap) {
            case CAPS_LOWER:
                steno_send_char(entry[i]);
                break;
            case CAPS_CAP:
                steno_send_char(toupper(entry[i]));
                new_state.cap = CAPS_LOWER;
                break;
            case CAPS_UPPER:
                steno_send_char(toupper(entry[i]));
                if (entry[i] == ' ') {
                    new_state.cap = CAPS_LOWER;
                }
                break;
            }
            str_len++;
            // Unicode
        } else {
            int32_t code_point = 0;
            const char *str = decode_utf8((char *) &entry[i], &code_point);
            if (code_point > 0) {
                steno_send_unicode(code_point);
            }
            str_len += 1;
            i = str - (char *) entry;
        }
    }
    if (!set_case) {
        new_state.cap = CAPS_LOWER;
    }

#ifdef STENO_DEBUG_HIST
    steno_debug_ln("'");
    steno_debug_ln("  -> %u", str_len);
#endif
    hist->len = valid_len ? str_len : 0;
    return new_state;
}
