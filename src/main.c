#include <stdbool.h>
#include <stdint.h>

#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "nrfx_systick.h"

#include "nrf_delay.h"

#include "app_timer.h"
#include "app_timer_utils.h"
#include "interrupts.h"

#include "leds_switch.h"
#include "pwm.h"

#include "my_logs.h"              
#include "nrf_log.h"              
#include "nrf_log_ctrl.h"         
#include "nrf_log_default_backends.h" 
#include "nrf_log_backend_usb.h"
#include "app_usbd.h"
#include "app_usbd_serial_num.h"

static const color color_array[] = {
    G, G, G, G, G, G, G, G, G,
    B, B, B, B, B, B, B, B, B
};

static const int col_arr_length = sizeof(color_array) / sizeof(color_array[0]);

int main(void)
{
    // logs initialization
    logs_init();
    NRF_LOG_INFO("Main function started");
    process_front_and_usb();

    // leds initializations and turns off
    init_leds();
    off_all_leds();

    // systick initializetion
    nrfx_systick_init();

    // timers initializetion
    init_app_timer_and_lfck();
    create_debounce_timer();
    create_double_click_timer();

    // gpiote initialization
    nrfx_gpiote_init();
    nrfx_gpiote_in_config_t config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    config.pull = NRF_GPIO_PIN_PULLUP;

    // initialization switch 1 as gpiote in and enavle event
    nrfx_gpiote_in_init(SWITCH1_PIN, &config, switch1_handler);
    nrfx_gpiote_in_event_enable(SWITCH1_PIN, true);

    static bool led_is_on = true;
    int current_brightness = 0; 
    int brightness_step = 10;
    int direction = 1;          
    int cur_color_index = 0; 

    while (true)
    {
        color cur_color = color_array[cur_color_index]; 

        if (button_double_clicked) {
            NRF_LOG_INFO("Button double clicked enter");
            
            button_double_clicked = false;
            led_is_on = !led_is_on;

            NRF_LOG_INFO("Button double clicked exit\n");
        }
        else if (button_pressed) {
            if (led_is_on) {    
                NRF_LOG_INFO("Button pressed enter");

                button_pressed = false;

                brightness_step--;
                if (brightness_step < 1) {
                    brightness_step = 10; 
                }
                NRF_LOG_INFO("Cur step = %d", brightness_step);
                NRF_LOG_INFO("Button pressed exit \n");
            }
        }
        if (led_is_on) {
            current_brightness += (direction * brightness_step);
            
            if (current_brightness >= 100) {
                current_brightness = 100;
                direction = -1;
            } else if (current_brightness <= 0) {
                current_brightness = 0;
                direction = 1;
                cur_color_index++;
                if (cur_color_index >= col_arr_length) {
                    cur_color_index = 0;
                }
            }
        }
        do_pwm_step(current_brightness  * 10, cur_color);
        
        nrf_delay_ms(20);
        process_front_and_usb();
    }
}