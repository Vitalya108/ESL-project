#ifndef NRFX_PWM_UTILS_H
#define NRFX_PWM_UTILS_H

#include <stdint.h>

void init_nrfx_pwm();

void set_led_color_rgb(uint8_t r, uint8_t g, uint8_t b);

void set_led_color_hsv(uint16_t h, uint8_t s, uint8_t v);

void set_led1_duty(int duty);

#endif