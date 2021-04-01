#pragma once

#include <stdint.h>

// Program page size
#define FLASH_PP_SIZE 256
#define FLASH_ERASED_BYTE 0xFF

int flash_init(void);
int steno_flash_read(uint32_t addr, uint8_t *buf, uint8_t len);

#include "zmk/steno/steno.h"
#ifdef STENO_DEBUG_FLASH
extern uint8_t flash_debug_enable;
#endif
