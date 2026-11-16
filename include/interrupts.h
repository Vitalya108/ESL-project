#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdbool.h>
#include "app_timer.h"
#include "nrfx_gpiote.h"

extern volatile bool button_pressed;
extern volatile bool button_double_clicked;
extern volatile bool waiting_for_second_click;
extern volatile bool button_holds;

void switch1_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

void switch1_debounce_timer_handler(void * p_context);

void switch1_double_click_handler(void * p_context);

void create_debounce_timer();

void create_double_click_timer();

#endif