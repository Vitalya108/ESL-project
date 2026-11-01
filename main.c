/**
 * Copyright (c) 2014 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup blinky_example_main main.c
 * @{
 * @ingroup blinky_example
 * @brief Blinky Example Application main file.
 *
 * This file contains the source code for a sample application to blink LEDs.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"

#define R_LED_PIN NRF_GPIO_PIN_MAP(0, 8)
#define G_LED_PIN NRF_GPIO_PIN_MAP(1, 9)
#define B_LED_PIN NRF_GPIO_PIN_MAP(0, 12)

typedef enum {
    R, G, B
} color;


static const uint32_t SWITCH1_PIN = NRF_GPIO_PIN_MAP(1, 6);

static const color sequence[] = {
    G, G, G, G, G, G, G, G, G,
    B, B, B, B, B, B, B, B, B
};

static const int seq_length = sizeof(sequence) / sizeof(sequence[0]);

static void off_all_leds() {
    nrf_gpio_pin_set(R_LED_PIN);
    nrf_gpio_pin_set(G_LED_PIN);
    nrf_gpio_pin_set(B_LED_PIN);
}

static void blink_color(color c) {
        switch (c) {
            case R: 
                nrf_gpio_pin_clear(R_LED_PIN);
                nrf_delay_ms(300);
                nrf_gpio_pin_set(R_LED_PIN);
                nrf_delay_ms(300);
                break;
            case G: 
                nrf_gpio_pin_clear(G_LED_PIN);
                nrf_delay_ms(300);
                nrf_gpio_pin_set(G_LED_PIN);
                nrf_delay_ms(300);
                break;
            case B: 
                nrf_gpio_pin_clear(B_LED_PIN);
                nrf_delay_ms(300);
                nrf_gpio_pin_set(B_LED_PIN);
                nrf_delay_ms(300);
                break;
        }
    }

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    off_all_leds();
    nrf_gpio_cfg_output(R_LED_PIN);
    nrf_gpio_cfg_output(G_LED_PIN);
    nrf_gpio_cfg_output(B_LED_PIN);

    nrf_gpio_cfg_input(SWITCH1_PIN, NRF_GPIO_PIN_PULLUP);

    int current_index = 0;


    while (true)
    {
    
        if (nrf_gpio_pin_read(SWITCH1_PIN) == 0) {
            blink_color(sequence[current_index]);
            current_index++;
            if (current_index >= seq_length) {
                current_index = 0;
            }
        }
    }
}