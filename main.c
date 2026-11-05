#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "leds.h"

static const color color_array[] = {
    G, G, G, G, G, G, G, G, G,
    B, B, B, B, B, B, B, B, B
};

static const int col_arr_length = sizeof(color_array) / sizeof(color_array[0]);

int main(void)
{
    init_leds();
    init_switch1();
    off_all_leds();

    int current_index = 0; 

    while (true)
    {
        
        color cur_color = color_array[current_index];

        if (SWITCH1_PRESSED) {
            turn_on_color(cur_color);
            nrf_delay_ms(500);
            
            if (SWITCH1_RELEASED) {
                continue;
            } else {
                turn_off_color(cur_color);
                nrf_delay_ms(500);
                current_index++;
                if (current_index >= col_arr_length) {
                    current_index = 0;
                }
            }
        }
    }
}