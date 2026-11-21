#ifndef HSV_H
#define HSV_H

#include <stdint.h>

void hsv_to_rgb(uint16_t h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b);

#endif