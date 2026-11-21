#include <stdbool.h>
#include "interrupts.h"
#include "app_timer.h"
#include "app_timer_utils.h"
#include "leds_switch.h"
#include "nrfx_gpiote.h"

volatile bool button_pressed = false;
volatile bool button_double_clicked = false;
volatile bool waiting_for_second_click = false;

APP_TIMER_DEF(SW1_DEBOUNCE_TIMER_ID);
APP_TIMER_DEF(SW1_DOUBLE_CLICK_ID);

void switch1_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if (SWITCH1_PRESSED) {
        app_timer_stop(SW1_DEBOUNCE_TIMER_ID);
        app_timer_start(SW1_DEBOUNCE_TIMER_ID, APP_TIMER_TICKS(30), NULL);
    } else {
        app_timer_stop(SW1_DEBOUNCE_TIMER_ID);
        button_pressed = false;
    }
}

void switch1_debounce_timer_handler(void * p_context)
{
    if (SWITCH1_PRESSED) {
        if (waiting_for_second_click) {
            app_timer_stop(SW1_DOUBLE_CLICK_ID);
            waiting_for_second_click = false;
            button_double_clicked = true;
            button_pressed = false;
        } else {
            button_pressed = true;
            waiting_for_second_click = true;
            app_timer_start(SW1_DOUBLE_CLICK_ID, APP_TIMER_TICKS(300), NULL);
        }
    }
}

void switch1_double_click_handler(void * p_context)
{
    waiting_for_second_click = false;
}

void create_debounce_timer() {
    app_timer_create(&SW1_DEBOUNCE_TIMER_ID, APP_TIMER_MODE_SINGLE_SHOT, switch1_debounce_timer_handler);
}

void create_double_click_timer() {
    app_timer_create(&SW1_DOUBLE_CLICK_ID, APP_TIMER_MODE_SINGLE_SHOT, switch1_double_click_handler);
}