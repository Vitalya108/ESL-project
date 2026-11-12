#include "app_timer_utils.h"
#include "app_timer.h"
#include "drv_rtc.h"
#include "nrf_drv_clock.h"
#include "app_error.h"  

void init_app_timer_and_lfck(void) {
    ret_code_t err = nrf_drv_clock_init();
    if (err != NRF_ERROR_MODULE_ALREADY_INITIALIZED) {
        APP_ERROR_CHECK(err);
    }
    nrf_drv_clock_lfclk_request(NULL);
    while (!nrf_drv_clock_lfclk_is_running()) {}
    app_timer_init();
}