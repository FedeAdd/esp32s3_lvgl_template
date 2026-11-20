#include "esp_lvgl_port.h"

#include "bsp_i2c.h"
#include "bsp_display.h"
#include "bsp_touch.h"
#include "bsp_lvgl.h"
#include "bsp_battery.h"
#include "bsp_button.h"

#include "lvgl_ui.h"

/** @brief App entry point */
void app_main(void) {
    // Initialize necessary peripherals to handle display and touch
    i2c_master_bus_handle_t i2c_bus = bsp_i2c_init();
    bsp_display_init();
    bsp_touch_init(i2c_bus);
    ESP_ERROR_CHECK(bsp_lvgl_init());
    bsp_display_brightness_init();
    bsp_display_set_brightness(100);
    // ADC for the battery arc on the home screen (TP2)
    bsp_battery_init();
    // Always take LVGL mutex before doing anything with LVGL
    if (lvgl_port_lock(0)) {
        lvgl_ui_start();
        lvgl_port_unlock();
    }
    // Physical button: the callback only enqueues a UI command (TP2)
    bsp_button_init(lvgl_ui_notify_button);
}