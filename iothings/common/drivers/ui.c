#include "ui.h"

/*Ticks before change duty cycle of each LED*/
#define TICKS_BEFORE_CHANGE_0   500
#define TICKS_BEFORE_CHANGE_1   400

// Bitmasks for the three LEDs
#define LED_BLUE_MASK       (1 << LED_BLUE)
#define LED_RED_MASK        (1 << LED_RED)
#define LED_GREEN_MASK      (1 << LED_GREEN)

//APP_TIMER_DEF(ui_timer_id);

static low_power_pwm_t low_power_pwm_blue;
static low_power_pwm_t low_power_pwm_red;
static low_power_pwm_t low_power_pwm_green;

static bool pwm_enabled = false;


//app_timer_start(ui_timer_id, UI_TIMER_INTERVAL, NULL);
//app_timer_stop(ui_timer_id);

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

    if (nrf_drv_clock_init_check()) NRF_LOG_INFO("Init already");

    err_code = nrf_drv_clock_init();
    if (err_code == NRF_ERROR_MODULE_ALREADY_INITIALIZED)
    {
    	NRF_LOG_INFO("Module already init");
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
    low_power_pwm_config.active_high    = false;
    low_power_pwm_config.period         = 255;
    low_power_pwm_config.bit_mask       = LED_RED_MASK;
    low_power_pwm_config.p_timer_id     = &lpp_timer_0;
    low_power_pwm_config.p_port         = NRF_GPIO;

    err_code = low_power_pwm_init((&low_power_pwm_red), &low_power_pwm_config, NULL);
    APP_ERROR_CHECK(err_code);
    /*err_code = low_power_pwm_duty_set(&low_power_pwm_red, 35);
    APP_ERROR_CHECK(err_code);*/

    APP_TIMER_DEF(lpp_timer_1);
    low_power_pwm_config.active_high    = false;
    low_power_pwm_config.period         = 255;
    low_power_pwm_config.bit_mask       = LED_GREEN_MASK;
    low_power_pwm_config.p_timer_id     = &lpp_timer_1;
    low_power_pwm_config.p_port         = NRF_GPIO;

    err_code = low_power_pwm_init((&low_power_pwm_green), &low_power_pwm_config, NULL);
    APP_ERROR_CHECK(err_code);
    /*err_code = low_power_pwm_duty_set(&low_power_pwm_green, 0);
    APP_ERROR_CHECK(err_code);*/

    APP_TIMER_DEF(lpp_timer_2);
    low_power_pwm_config.active_high    = false;
    low_power_pwm_config.period         = 255;
    low_power_pwm_config.bit_mask       = LED_BLUE_MASK;
    low_power_pwm_config.p_timer_id     = &lpp_timer_2;
    low_power_pwm_config.p_port         = NRF_GPIO;

    err_code = low_power_pwm_init((&low_power_pwm_blue), &low_power_pwm_config, NULL);
    APP_ERROR_CHECK(err_code);
    /*err_code = low_power_pwm_duty_set(&low_power_pwm_blue, 102);
    APP_ERROR_CHECK(err_code);*/
}

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

void ui_set_led_duty(ui_channel_t channel, uint32_t duty)
{
    switch (channel)
    {
    case UI_LED_RED:
        low_power_pwm_duty_set(&low_power_pwm_red, 255 - (uint8_t)duty);
        break;
    case UI_LED_GREEN:
        low_power_pwm_duty_set(&low_power_pwm_green, 255 - (uint8_t)duty);
        break;
    case UI_LED_BLUE:
        low_power_pwm_duty_set(&low_power_pwm_blue, 255 - (uint8_t)duty);
        break;
    default:
        break;
    }
}


void ui_set_RGB_duty(uint32_t duty_red, uint32_t duty_green, uint32_t duty_blue)
{
    low_power_pwm_duty_set(&low_power_pwm_red, 255 - (uint8_t)duty_red);
    low_power_pwm_duty_set(&low_power_pwm_green, 255 - (uint8_t)duty_green);
    low_power_pwm_duty_set(&low_power_pwm_blue, 255 - (uint8_t)duty_blue);
}

void ui_set_RGB_on(ui_rgb_t* color)
{
    low_power_pwm_duty_set(&low_power_pwm_red, 255 - (uint8_t)color->red);
    low_power_pwm_duty_set(&low_power_pwm_green, 255 - (uint8_t)color->green);
    low_power_pwm_duty_set(&low_power_pwm_blue, 255 - (uint8_t)color->blue);
}

void ui_set_RGB_off()
{
    low_power_pwm_duty_set(&low_power_pwm_red, 255);
    low_power_pwm_duty_set(&low_power_pwm_green, 255);
    low_power_pwm_duty_set(&low_power_pwm_blue, 255);
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

    //Create the ui timer
    //uint32_t err_code = app_timer_create(&ui_timer_id, APP_TIMER_MODE_REPEATED, ui_timer_handler);
    //APP_ERROR_CHECK(err_code);
}
