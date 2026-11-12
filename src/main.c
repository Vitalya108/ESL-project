#include <stdbool.h>
#include <stdint.h>

#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "nrfx_systick.h"

#include "app_timer.h"
#include "app_timer_utils.h"

#include "leds.h"
#include "pwm.h"

#include "my_logs.h"              
#include "nrf_log.h"              
#include "nrf_log_ctrl.h"         
#include "nrf_log_default_backends.h" 
#include "nrf_log_backend_usb.h"
#include "app_usbd.h"
#include "app_usbd_serial_num.h"

// static const color color_array[] = {
//     G, G, G, G, G, G, G, G, G,
//     B, B, B, B, B, B, B, B, B
// };

// static const int col_arr_length = sizeof(color_array) / sizeof(color_array[0]);

static volatile bool button_pressed = false;
static volatile bool button_double_clicked = false;
static bool waiting_for_second_click = false;
static bool led_is_on = false;
int duty_cycle = 0;
int blink_speed = 20;

//timer def
APP_TIMER_DEF(SW1_DEBOUNCE_TIMER_ID);
APP_TIMER_DEF(SW1_DOUBLE_CLICK_ID);

static void switch1_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{

    if (SWITCH1_PRESSED) {
        app_timer_stop(SW1_DEBOUNCE_TIMER_ID);
        app_timer_start(SW1_DEBOUNCE_TIMER_ID, APP_TIMER_TICKS(30), NULL);
    }
}

static void switch1_debounce_timer_handler(void * p_context)
{
    if (SWITCH1_PRESSED) {
        if (waiting_for_second_click) {
            app_timer_stop(SW1_DOUBLE_CLICK_ID);
            waiting_for_second_click = false;
            button_double_clicked = true;
        } else {
            waiting_for_second_click = true;
            app_timer_start(SW1_DOUBLE_CLICK_ID, APP_TIMER_TICKS(300), NULL);
        }
    }
}

static void switch1_double_click_handler(void * p_context)
{
    waiting_for_second_click = false;
    button_pressed = true; 
}


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
    app_timer_create(&SW1_DEBOUNCE_TIMER_ID, APP_TIMER_MODE_SINGLE_SHOT, switch1_debounce_timer_handler);
    app_timer_create(&SW1_DOUBLE_CLICK_ID, APP_TIMER_MODE_SINGLE_SHOT, switch1_double_click_handler);

    // gpiote initialization
    nrfx_gpiote_init();
    nrfx_gpiote_in_config_t config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    config.pull = NRF_GPIO_PIN_PULLUP;

    // initialization switch 1 as gpiote in and enavle event
    nrfx_gpiote_in_init(SWITCH1_PIN, &config, switch1_handler);
    nrfx_gpiote_in_event_enable(SWITCH1_PIN, true);


    int cur_duty = duty_cycle;

    // int current_index = 0; 

    while (true)
    {
        // color cur_color = color_array[current_index]; 

        if (button_double_clicked) {
            NRF_LOG_INFO("Button double clicked enter %d", cur_duty);

            button_double_clicked = false;
            
            led_is_on = !led_is_on;
            
            if (led_is_on) {
                NRF_LOG_INFO("LED ON");
                cur_duty = duty_cycle;
            } else {
                NRF_LOG_INFO("LED OFF");
                cur_duty = 0;
            }
            NRF_LOG_INFO("Button double clicked exit %d \n", cur_duty);
        }
        else if (button_pressed) {
            NRF_LOG_INFO("Button pressed enter");

            button_pressed = false;

            if (led_is_on) {
                
                duty_cycle += blink_speed;

                if (duty_cycle > 100 || duty_cycle < 0) {
                    blink_speed *= -1;
                    duty_cycle += blink_speed;
                }
                cur_duty = duty_cycle;
            }
            NRF_LOG_INFO("Button pressed exit \n");
        }
        do_pwm_step(cur_duty * 10, B);
        process_front_and_usb();
    }
}