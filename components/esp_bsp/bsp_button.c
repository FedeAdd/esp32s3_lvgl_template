#include "bsp_button.h"
#include "iot_button.h"
#include "button_gpio.h"
#include "esp_log.h"

static const char *TAG = "bsp_button";

static bsp_button_cb_t s_on_click = NULL;

static void button_single_click_cb(void *button_handle, void *usr_data)
{
    (void)button_handle;
    (void)usr_data;
    if (s_on_click) {
        s_on_click();
    }
}

void bsp_button_init(bsp_button_cb_t on_click)
{
    s_on_click = on_click;

    /* espressif/button 4.x API: generic config + GPIO driver config */
    const button_config_t btn_cfg = {0};   /* default long/short press times */
    const button_gpio_config_t gpio_cfg = {
        .gpio_num = BSP_PIN_BUTTON,
        .active_level = BSP_BUTTON_ACTIVE_LEVEL,
    };

    button_handle_t btn = NULL;
    esp_err_t err = iot_button_new_gpio_device(&btn_cfg, &gpio_cfg, &btn);
    if (err != ESP_OK || btn == NULL) {
        ESP_LOGE(TAG, "button create failed: %s", esp_err_to_name(err));
        return;
    }
    iot_button_register_cb(btn, BUTTON_SINGLE_CLICK, NULL, button_single_click_cb, NULL);
    ESP_LOGI(TAG, "button registered on GPIO %d", BSP_PIN_BUTTON);
}