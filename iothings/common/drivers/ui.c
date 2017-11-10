#include "ui.h"

// Bitmasks for the three LEDs
#define LED_BLUE_MASK       (1 << LED_BLUE)
#define LED_RED_MASK        (1 << LED_RED)
#define LED_GREEN_MASK      (1 << LED_GREEN)

//APP_TIMER_DEF(ui_timer_id);

static low_power_pwm_t low_power_pwm_blue;
static low_power_pwm_t low_power_pwm_red;
static low_power_pwm_t low_power_pwm_green;

static bool pwm_enabled;


//app_timer_start(ui_timer_id, UI_TIMER_INTERVAL, NULL);
//app_timer_stop(ui_timer_id);


/**
*	@brief Starts the low power pwm instances
*/
static void _start_pwm()
{
    if (pwm_enabled)
        return;

    uint32_t err_code;
    err_code = low_power_pwm_start((&low_power_pwm_blue), low_power_pwm_blue.bit_mask);
    APP_ERROR_CHECK(err_code);
    err_code = low_power_pwm_start((&low_power_pwm_red), low_power_pwm_red.bit_mask);
    APP_ERROR_CHECK(err_code);
    err_code = low_power_pwm_start((&low_power_pwm_green), low_power_pwm_green.bit_mask);
    APP_ERROR_CHECK(err_code);

    pwm_enabled = true;
}

/**
*	@brief Stops the low power pwm instances
*/
/*static void _stop_pwm()
{
    if (!pwm_enabled)
        return;

    uint32_t err_code;
    err_code = low_power_pwm_stop(&low_power_pwm_blue);
    APP_ERROR_CHECK(err_code);
    err_code = low_power_pwm_stop(&low_power_pwm_red);
    APP_ERROR_CHECK(err_code);
    err_code = low_power_pwm_stop(&low_power_pwm_green);
    APP_ERROR_CHECK(err_code);

    pwm_enabled = false;
}*/


/**
*	@brief Initialise the low frequency clock
*/
/*static void _lfclk_init(void)
{
    uint32_t err_code;
    err_code = nrf_drv_clock_init();
    //if (err_code != MODULE_ALREADY_INITIALIZED && err_code != NRF_SUCCESS)
    if (err_code != NRF_SUCCESS)
    {
        APP_ERROR_CHECK(err_code);
    }

    nrf_drv_clock_lfclk_request(NULL);
}*/

/**
*	@brief Initialise the low power PWM settings for the RGB LEDs
*	@details Creates three LED pwm instances and configures a default setting for each of them, the
*	default value of the duty cycle is set to zero
*/
static void _low_power_init(void)
{
    // Blue : R 0, G 100, B 255
    uint32_t err_code;
    low_power_pwm_config_t low_power_pwm_config;

    APP_TIMER_DEF(lpp_timer_0);
    low_power_pwm_config.active_high = false;
    low_power_pwm_config.period = 255;
    low_power_pwm_config.bit_mask = LED_BLUE_MASK;
    low_power_pwm_config.p_timer_id = &lpp_timer_0;

    err_code = low_power_pwm_init((&low_power_pwm_blue), &low_power_pwm_config, NULL);
    APP_ERROR_CHECK(err_code);
    err_code = low_power_pwm_duty_set(&low_power_pwm_blue, 0);
    APP_ERROR_CHECK(err_code);

    APP_TIMER_DEF(lpp_timer_1);
    low_power_pwm_config.active_high = false;
    low_power_pwm_config.period = 255;
    low_power_pwm_config.bit_mask = LED_RED_MASK;
    low_power_pwm_config.p_timer_id = &lpp_timer_1;

    err_code = low_power_pwm_init((&low_power_pwm_red), &low_power_pwm_config, NULL);
    APP_ERROR_CHECK(err_code);
    err_code = low_power_pwm_duty_set(&low_power_pwm_red, 0);
    APP_ERROR_CHECK(err_code);

    APP_TIMER_DEF(lpp_timer_2);
    low_power_pwm_config.active_high = false;
    low_power_pwm_config.period = 255;
    low_power_pwm_config.bit_mask = LED_GREEN_MASK;
    low_power_pwm_config.p_timer_id = &lpp_timer_2;

    err_code = low_power_pwm_init((&low_power_pwm_green), &low_power_pwm_config, NULL);
    APP_ERROR_CHECK(err_code);
    err_code = low_power_pwm_duty_set(&low_power_pwm_green, 0);
    APP_ERROR_CHECK(err_code);

    pwm_enabled = false;
}

static void _set_pwm(ui_channel_t channel, uint32_t duty)
{
    switch (channel)
    {
    case UI_LED_RED:
        low_power_pwm_duty_set(&low_power_pwm_red, (uint8_t)duty);
        break;
    case UI_LED_GREEN:
        low_power_pwm_duty_set(&low_power_pwm_green, (uint8_t)duty);
        break;
    case UI_LED_BLUE:
        low_power_pwm_duty_set(&low_power_pwm_blue, (uint8_t)duty);
        break;
    default:
        break;
    }
}

/**
*	@brief Initialise the user interface consisting of 3 LEDS (RGB)
*/
void ui_init(void)
{
    //_lfclk_init();

    // initialise the low power pwm
    _low_power_init();

	_start_pwm();

	//_set_pwm(UI_LED_GREEN, 100);
	_set_pwm(UI_LED_GREEN, 100);
    //Create the ui timer
    //uint32_t err_code = app_timer_create(&ui_timer_id, APP_TIMER_MODE_REPEATED, ui_timer_handler);
    //APP_ERROR_CHECK(err_code);
}
