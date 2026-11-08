#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "leds.h"
#include "nrfx_gpiote.h"
#include "pwm.h"
#include "nrfx_systick.h"

// static const color color_array[] = {
//     G, G, G, G, G, G, G, G, G,
//     B, B, B, B, B, B, B, B, B
// };

// static const int col_arr_length = sizeof(color_array) / sizeof(color_array[0]);

// static void button_handler(void) {

// }

int main(void)
{
    

    init_leds();
    init_switch1();
    off_all_leds();



    // int current_index = 0; 
    int duty_cycle = 0;
    int delta = 1;
    nrfx_systick_init();

    while (true)
    {
        // color cur_color = color_array[current_index]; 

        do_pwm_step(duty_cycle * 10, B); 
    
        
        duty_cycle += delta;

        if (duty_cycle > 100 || duty_cycle < 0) {
            delta *= -1;
            duty_cycle += delta;
        }
    }
}