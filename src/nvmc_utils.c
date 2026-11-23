#include "nvmc_utils.h"
#include "nrfx_nvmc.h"
#include <stdint.h>

void save_color_to_flash(uint16_t h, uint8_t s, uint8_t v) {

    uint32_t packed_data = hsv_into_addres(h, s, v);

    if (!nrfx_nvmc_word_writable_check(APP_DATA_START_ADDR, packed_data)) {

        nrfx_nvmc_page_erase(APP_DATA_START_ADDR);
        while(!nrfx_nvmc_write_done_check());
    }

    nrfx_nvmc_word_write(APP_DATA_START_ADDR, packed_data);
    while(!nrfx_nvmc_write_done_check());
}

uint32_t hsv_into_addres(uint16_t h, uint8_t s, uint8_t v) {
    return (h & 0x1FF) | ((s & 0x7F) << 9) | ((v & 0x7F) << 16);
}

bool load_color_from_flash(uint16_t *h, uint8_t *s, uint8_t *v) {

    uint32_t packed_data = *(volatile uint32_t*)APP_DATA_START_ADDR;
    
    if (packed_data != 0xFFFFFFFF) {

        *h = packed_data & 0x1FF;
        *s = (packed_data >> 9) & 0x7F;
        *v = (packed_data >> 16) & 0x7F;
        return true;
    }
    return false;
}