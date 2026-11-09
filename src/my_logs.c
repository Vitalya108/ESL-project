#include "my_logs.h"              
#include "nrf_log.h"              
#include "nrf_log_ctrl.h"         
#include "nrf_log_default_backends.h" 
#include "nrf_log_backend_usb.h"
#include "app_error.h"            

void logs_init(void)
{
    ret_code_t ret = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(ret);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}
void process_front_and_usb(void) {
    LOG_BACKEND_USB_PROCESS();
    NRF_LOG_PROCESS();
}