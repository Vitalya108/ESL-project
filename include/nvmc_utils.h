#ifndef NVMC_H
#define NVMC_H

#include <stdint.h>
#include <stdbool.h>

#define BOOTLOADER_START_ADDR 0xE0000
#define NRF_DFU_APP_DATA_AREA_SIZE 3
#define APP_DATA_START_ADDR (BOOTLOADER_START_ADDR - (NRF_DFU_APP_DATA_AREA_SIZE * 0x1000))
#define FLASH_MAGIC 0xDEADBEEF

void save_color_to_flash(uint16_t h, uint8_t s, uint8_t v);

uint32_t hsv_into_addres(uint16_t h, uint8_t s, uint8_t v);

bool load_color_from_flash(uint16_t *h, uint8_t *s, uint8_t *v);

#endif