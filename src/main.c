#include <stdbool.h>
#include <stdint.h>

#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "nrfx_systick.h"
#include "app_timer.h"
#include "drv_rtc.h"
#include "nrf_drv_clock.h"

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
// static bool sw1_is_pressed = false; 
int duty_cycle = 0;
int delta = 5;

//timer vars and def
// APP_TIMER_DEF(sw1_db_timer_id);

// static void switch1_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
// {
//     app_timer_stop(sw1_db_timer_id);
//     app_timer_start(sw1_db_timer_id, APP_TIMER_TICKS(30), NULL);
// }

static void switch1_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
    button_pressed = true;
}

// static void switch1_db_timer_handler(void * p_context)
// {
//     bool pressed = (nrf_gpio_pin_read(SWITCH1_PIN) == 0);

//     if (pressed && !sw1_is_pressed) {
//         button_pressed = true;
//     }
//     sw1_is_pressed = pressed;
// }


int main(void)
{
    // logs initialization
    logs_init();
    NRF_LOG_INFO("Starting up the test project with USB logging");
    process_front_and_usb();

    // leds initializations and turns off
    init_leds();
    off_all_leds();

    // systick initializetion
    nrfx_systick_init();

    // timer initializetion
    // ret_code_t err = nrf_drv_clock_init();
    // if (err != NRF_ERROR_MODULE_ALREADY_INITIALIZED) {
    //     APP_ERROR_CHECK(err);
    // }
    // nrf_drv_clock_lfclk_request(NULL);
    // while (!nrf_drv_clock_lfclk_is_running()) {}
    // app_timer_init();
    // app_timer_create(&sw1_db_timer_id, APP_TIMER_MODE_SINGLE_SHOT, switch1_db_timer_handler);


    // gpiote initialization
    nrfx_gpiote_init();
    nrfx_gpiote_in_config_t config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    config.pull = NRF_GPIO_PIN_PULLUP;

    // initialization switch 1 as gpiote in and enavle event
    nrfx_gpiote_in_init(SWITCH1_PIN, &config, switch1_handler);
    nrfx_gpiote_in_event_enable(SWITCH1_PIN, true);



    // int current_index = 0; 

    while (true)
    {
        // color cur_color = color_array[current_index]; 

        if (button_pressed) {

            NRF_LOG_INFO("Button pressed enter");
            process_front_and_usb();

            button_pressed = false;

            duty_cycle += delta;

            if (duty_cycle > 100 || duty_cycle < 0) {
                delta *= -1;
                duty_cycle += delta;
            }
            NRF_LOG_INFO("Button pressed exit");
        }
        do_pwm_step(duty_cycle * 10, B);
        
        process_front_and_usb();
    }
}