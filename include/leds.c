#include "leds.h"
#include "nrf_gpio.h"

int get_led_pin_by_color(color c) {
    switch(c) {
        case R: return R_LED_PIN;
        case G: return G_LED_PIN;
        case B: return B_LED_PIN;
        default: return -1;
    }
}

void off_all_leds(void) {
    nrf_gpio_pin_set(R_LED_PIN);
    nrf_gpio_pin_set(G_LED_PIN);
    nrf_gpio_pin_set(B_LED_PIN);
}

void turn_on_color(color c) {
    int led_pin = get_led_pin_by_color(c);
    if (led_pin != -1) {
        nrf_gpio_pin_clear(led_pin);
    }
}

void turn_off_color(color c) {
    int led_pin = get_led_pin_by_color(c);
    if (led_pin != -1) {
        nrf_gpio_pin_set(led_pin);
    }
}

void init_leds() {
    nrf_gpio_cfg_output(R_LED_PIN);
    nrf_gpio_cfg_output(G_LED_PIN);
    nrf_gpio_cfg_output(B_LED_PIN);
}

void init_switch1() {
    nrf_gpio_cfg_input(SWITCH1_PIN, NRF_GPIO_PIN_PULLUP);
}