#ifndef _STENO_H_
#define _STENO_H_

#include <stdint.h>
#include "zmk/steno/stroke.h"

#include "zmk/steno/config.h"

#if defined(CONSOLE_ENABLE) && defined(STENO_DEBUG)
#ifdef DEBUG_FLASH
#define STENO_DEBUG_FLASH
#endif

#ifdef DEBUG_HIST
#define STENO_DEBUG_HIST
#endif

#ifdef DEBUG_STROKE
#define STENO_DEBUG_STROKE
#endif

#ifdef DEBUG_DICTED
#define STENO_DEBUG_DICTED
#endif
#endif

#ifdef __AVR__
#define log_strdup(s) (s)

#define steno_error(format, ...) xprintf(format, ##__VA_ARGS__)
#define steno_error_ln(format, ...) xprintf(format "\n", ##__VA_ARGS__)
#if defined(CONSOLE_ENABLE) && defined(STENO_DEBUG)
#define steno_debug(format, ...) xprintf(format, ##__VA_ARGS__)
#define steno_debug_ln(format, ...) xprintf(format "\n", ##__VA_ARGS__)
#else
#define steno_debug(...)
#define steno_debug_ln(...)
#endif
#else
#define steno_error(format, ...) LOG_ERR(format, ##__VA_ARGS__)
#define steno_error_ln(format, ...) LOG_ERR(format, ##__VA_ARGS__)
#if defined(CONSOLE_ENABLE) && defined(STENO_DEBUG)
#define steno_debug(format, ...) LOG_DBG(format, ##__VA_ARGS__)
#define steno_debug_ln(format, ...) LOG_DBG(format, ##__VA_ARGS__)
#else
#define steno_debug(...)
#define steno_debug_ln(...)
#endif
#endif

#define STENO_R_R 0x008100

extern uint8_t stroke_start_ind;
int steno_init(void);
void steno_process_stroke(uint32_t stroke, int64_t timestamp);

#endif
