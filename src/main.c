#include <stdbool.h>
#include <stdint.h>

#include "nrfx_gpiote.h"

#include "nrf_delay.h"

#include "app_timer_utils.h"
#include "interrupts.h"

#include "leds_switch.h"
#include "nrfx_pwm_utils.h"

#include "hsv.h"

#include "my_logs.h"                      
#include "nrf_log.h"         

int main(void)
{
    // logs initialization
    logs_init();
    NRF_LOG_INFO("Main function started");
    process_front_and_usb();

    // timers initializetion
    init_app_timer_and_lfck();
    create_debounce_timer();
    create_double_click_timer();

    // gpiote initialization
    nrfx_gpiote_init();
    nrfx_gpiote_in_config_t config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    config.pull = NRF_GPIO_PIN_PULLUP;

    // initialization switch 1 as gpiote in and enable event
    nrfx_gpiote_in_init(SWITCH1_PIN, &config, switch1_handler);
    nrfx_gpiote_in_event_enable(SWITCH1_PIN, true);

    // nrfx initialization
    init_nrfx_pwm();

    // color vars
    int h = 360 * 0.99;
    int s = 100;
    int v = 100;

    int current_brightness = 0; 

    // hsv color initialization
    set_led_color_hsv(h, s, v);
    int cur_mode = 0;
    int led1_blink_dir = 1;
    int led1_blink_speed = 0;

    while (true)
    {
        if (button_double_clicked) {

            NRF_LOG_INFO("Button double clicked cur mode = %d, blink speed = %d", cur_mode, led1_blink_speed);
            
            button_double_clicked = false;
            cur_mode++;
            cur_mode %= 4;
            set_led1_duty(0);
            
            switch (cur_mode) {
                case 0: // no input
                    led1_blink_speed = 0;
                    break;
                case 1: // h selection
                    led1_blink_speed = 10;
                    break;
                case 2: // s selection
                    led1_blink_speed = 20;
                    break;
                case 3: // v selection
                    set_led1_duty(255);
                    led1_blink_speed = 0;
                    break;
            }

            NRF_LOG_INFO("Button double clicked cur mode = %d, blink speed = %d", cur_mode, led1_blink_speed);
        }
        else if (button_pressed) {
            switch (cur_mode) {
                case 1: // h selection
                    h++;
                    h %= 360;
                    break;
                case 2: // s selection
                    s++;
                    s %= 100;
                    break;
                case 3: // v selection
                    v++;
                    v %= 100;
                    break;
            }
            set_led_color_hsv(h, s, v);

        }
        if (led1_blink_speed > 0) {

            current_brightness += (led1_blink_dir * led1_blink_speed);

            if (current_brightness >= 255) {
                    current_brightness = 255;
                    led1_blink_dir = -1;
            } else if (current_brightness <= 0) {
                current_brightness = 0;
                led1_blink_dir = 1;
            }
            set_led1_duty(current_brightness);
        }
        nrf_delay_ms(30);
        process_front_and_usb();
    }
}