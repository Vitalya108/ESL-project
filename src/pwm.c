#include "leds.h"
#include "nrfx_systick.h"
#include "nrf_gpio.h"
#include "pwm.h"

void do_pwm_step(int duty_cycle_us, color c) {

    if (duty_cycle_us == 0) {
        return;
    }
    nrfx_systick_state_t start_time;
  
    nrfx_systick_get(&start_time);
    turn_on_color(c);

    while (!nrfx_systick_test(&start_time, duty_cycle_us)) {}

    turn_off_color(c);
    nrfx_systick_get(&start_time);

    while (!nrfx_systick_test(&start_time, 1000 - duty_cycle_us)) {}
}