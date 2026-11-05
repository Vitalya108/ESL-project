#ifndef LEDS_H
#define LEDS_H

#define R_LED_PIN NRF_GPIO_PIN_MAP(0, 8)
#define G_LED_PIN NRF_GPIO_PIN_MAP(1, 9)
#define B_LED_PIN NRF_GPIO_PIN_MAP(0, 12)
#define SWITCH1_PIN NRF_GPIO_PIN_MAP(1, 6)
#define SWITCH1_PRESSED nrf_gpio_pin_read(SWITCH1_PIN) == 0
#define SWITCH1_RELEASED nrf_gpio_pin_read(SWITCH1_PIN) != 0


typedef enum {
    R, G, B
} color;

int get_led_pin_by_color(color c);

void off_all_leds();

void turn_on_color(color c);

void turn_off_color(color c);

void init_leds();

void init_switch1();

#endif