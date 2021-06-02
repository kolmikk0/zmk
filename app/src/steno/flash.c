// Functions for interacting with SPI flash
#include "zmk/steno/flash.h"
#include "zmk/steno/steno.h"
#include <device.h>
#include <logging/log.h>
#include <drivers/flash.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#ifdef STENO_DEBUG_FLASH
uint8_t flash_debug_enable = 0;
#endif

/* #define FLASH_DEVICE DT_LABEL(DT_INST(0, nordic_qspi_nor)) */
#define FLASH_DEVICE DT_LABEL(DT_INST(0, jedec_spi_nor))

const struct device *flash_dev;

int flash_init(void) {
    flash_dev = device_get_binding(FLASH_DEVICE);
	if (!flash_dev) {
		LOG_ERR("SPI flash driver %s was not found!\n", FLASH_DEVICE);
		return -ENODEV;
	}
    return 0;
}

int steno_flash_read(uint32_t addr, uint8_t *buf, uint8_t len) {
#ifdef STENO_DEBUG_FLASH
    if (flash_debug_enable) {
        steno_debug_ln("flash_read(# 0x%02X @ 0x%06lX)", len, addr);
    }
#endif
    return flash_read(flash_dev, addr, buf, len);
}
