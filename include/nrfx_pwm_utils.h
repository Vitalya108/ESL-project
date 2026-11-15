#ifndef NRFX_PWM_UTILS_H
#define NRFX_PWM_UTILS_H

void init_nrfx_pwm();

void set_led_color_rgb(int r, int g, int b);

void set_led_color_hsv(int h, int s, int v);

#endif