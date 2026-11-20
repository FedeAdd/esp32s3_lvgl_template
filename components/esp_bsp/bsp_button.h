#ifndef __BSP_BUTTON_H__
#define __BSP_BUTTON_H__

#include "driver/gpio.h"

/* BOOT button of the ESP32-S3 (active low) */
#define BSP_PIN_BUTTON          GPIO_NUM_0
#define BSP_BUTTON_ACTIVE_LEVEL 0

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*bsp_button_cb_t)(void);

/**
 * Register the physical button with the espressif/button component.
 * on_click is called on every single click. It runs in the button
 * component's timer task, NOT in the LVGL context: never call LVGL there.
 */
void bsp_button_init(bsp_button_cb_t on_click);

#ifdef __cplusplus
}
#endif

#endif
