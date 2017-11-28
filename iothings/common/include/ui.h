#ifndef __UI_H
#define __UI_H

#include <stdbool.h>
#include <stdint.h>

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "app_error.h"
#include "sdk_errors.h"
#include "app_timer.h"
#include "nrf_log.h"
#include "nrf_drv_clock.h"
#include "app_util_platform.h"
#include "low_power_pwm.h"
#include "nordic_common.h"

typedef enum
{
    UI_LED_RED,
    UI_LED_GREEN,
    UI_LED_BLUE,
    UI_LED_NONE
} ui_channel_t;

typedef struct
{
    uint8_t red; /* Intensity 0-255 */
    uint8_t green; /* Intensity 0-255 */
    uint8_t blue; /* Intensity 0-255 */
} ui_rgb_t;

#define UI_LED_INTENSITY_HIGH               200
#define UI_LED_INTENSITY_LOW                100

#define BLINK_FAST_ON_OFF_DUR               20
#define BLINK_SLOW_ON_OFF_DUR               80

// Function prototypes
void ui_init(void);

void ui_set_led_duty(ui_channel_t channel, uint32_t duty);
void ui_set_RGB_duty(uint32_t duty_red, uint32_t duty_green, uint32_t duty_blue);
void ui_set_RGB_on(ui_rgb_t* color);
void ui_set_RGB_off();

/*void ui_set_led_off(ui_channel_t channel);
void ui_set_led_on(ui_channel_t channel, uint32_t intensity);
void ui_set_led_dur(ui_channel_t channel, uint32_t duration, uint32_t intensity);
void ui_set_led_blink(ui_channel_t channel, uint32_t on_duration, uint32_t off_duration, uint32_t cycles, uint32_t intensity);
void ui_set_led_table(ui_channel_t channel, uint16_t* table, uint32_t table_len, uint32_t cycles);

void ui_set_RGB_off();
void ui_set_RGB_on(ui_rgb_t* rgb, uint32_t duration);
void ui_set_RGB_blink(ui_rgb_t* rgb, uint32_t on_duration, uint32_t off_duration, uint32_t cycles, uint32_t intensity);*/

#endif //__UI_H
