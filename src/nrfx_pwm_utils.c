#include "nrfx_pwm_utils.h"
#include "leds_switch.h"
#include "nrfx_pwm.h"
#include "app_error.h"
#include "hsv.h"

static nrfx_pwm_t m_pwm0 = NRFX_PWM_INSTANCE(0);
static nrf_pwm_values_individual_t m_pwm_values;

static nrf_pwm_sequence_t const m_seq = {
    .values.p_individual = &m_pwm_values,
    .length              = NRF_PWM_VALUES_LENGTH(m_pwm_values),
    .repeats             = 0,
    .end_delay           = 0
};

void init_nrfx_pwm() {
    nrfx_pwm_config_t config = NRFX_PWM_DEFAULT_CONFIG;

    config.output_pins[0] = R_LED_PIN | NRFX_PWM_PIN_INVERTED; 
    config.output_pins[1] = G_LED_PIN | NRFX_PWM_PIN_INVERTED;
    config.output_pins[2] = B_LED_PIN | NRFX_PWM_PIN_INVERTED;
    config.output_pins[3] = LED1_PIN | NRFX_PWM_PIN_INVERTED;
    // config.output_pins[3] = NRFX_PWM_PIN_NOT_USED;

    config.load_mode = NRF_PWM_LOAD_INDIVIDUAL;

    config.base_clock = NRF_PWM_CLK_1MHz;
    config.top_value = 255;

    APP_ERROR_CHECK(nrfx_pwm_init(&m_pwm0, &config, NULL));
}

void set_led1_duty(int duty) {
    m_pwm_values.channel_3 = duty;
    nrfx_pwm_simple_playback(&m_pwm0, &m_seq, 1, NRFX_PWM_FLAG_LOOP);
}

void set_led_color_hsv(uint16_t h, uint8_t s, uint8_t v) {
    uint8_t r, g, b;
    hsv_to_rgb(h, s, v, &r, &g, &b);
    set_led_color_rgb(r, g, b);
}

void set_led_color_rgb(uint8_t r, uint8_t g, uint8_t b) {
    m_pwm_values.channel_0 = r;
    m_pwm_values.channel_1 = g;
    m_pwm_values.channel_2 = b;

    nrfx_pwm_simple_playback(&m_pwm0, &m_seq, 1, NRFX_PWM_FLAG_LOOP);
}