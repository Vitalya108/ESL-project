#include "hsv.h"

void hsv_to_rgb(uint16_t h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b) {
    h %= 360;
    if (h < 0) h += 360;
    if (s < 0) s = 0;
    if (s > 100) s = 100;
    if (v < 0) v = 0;
    if (v > 100) v = 100;

    int region = h / 60;
    int rem    = h % 60;

    int V = (v * 255 + 50) / 100;

    int p = (V * (100 * 60 - s * 60) + 3000) / 6000;
    int q = (V * (100 * 60 - s * rem) + 3000) / 6000;
    int t = (V * (100 * 60 - s * (60 - rem)) + 3000) / 6000;

    switch (region) {
    case 0:  *r = V; *g = t; *b = p; break;
    case 1:  *r = q; *g = V; *b = p; break;
    case 2:  *r = p; *g = V; *b = t; break;
    case 3:  *r = p; *g = q; *b = V; break;
    case 4:  *r = t; *g = p; *b = V; break;
    default: *r = V; *g = p; *b = q; break;
    }
}