#include "scr_home.h"
#include "screens.h"
#include "ui.h"             /* ui_arcBattery, ui_lblBattery, ui_lblTemp, ui_lblUptime */
#include "bsp_battery.h"
#include "driver/temperature_sensor.h"
#include "esp_timer.h"
#include "esp_log.h"
#include <stdio.h>

static const char *TAG = "scr_home";

#define HOME_REFRESH_MS     1000

/* Li-ion cell: 3.3 V = empty, 4.2 V = full */
#define BATT_V_EMPTY        3.3f
#define BATT_V_FULL         4.2f

static lv_timer_t *s_refresh_timer = NULL;
static temperature_sensor_handle_t s_temp_sensor = NULL;

/* ------------------------------------------------------------------ */

static int battery_percent(void)
{
    float voltage = 0.0f;
    bsp_battery_get_voltage(&voltage, NULL);

    int pct = (int)((voltage - BATT_V_EMPTY) * 100.0f / (BATT_V_FULL - BATT_V_EMPTY));
    if (pct < 0)   pct = 0;
    if (pct > 100) pct = 100;
    return pct;
}

/* Runs inside the LVGL context (called from init() and from the lv_timer) */
static void refresh_values(void)
{
    char buf[32];

    /* Battery */
    int pct = battery_percent();
    lv_arc_set_value(ui_arcBattery, pct);
    lv_label_set_text_fmt(ui_lblBattery, "%d%%", pct);

    /* Chip temperature */
    float temp_c = 0.0f;
    if (s_temp_sensor && temperature_sensor_get_celsius(s_temp_sensor, &temp_c) == ESP_OK) {
        snprintf(buf, sizeof(buf), "Temp: %.1f C", temp_c);
    } else {
        snprintf(buf, sizeof(buf), "Temp: -- C");
    }
    lv_label_set_text(ui_lblTemp, buf);

    /* Uptime */
    uint32_t s = (uint32_t)(esp_timer_get_time() / 1000000ULL);
    snprintf(buf, sizeof(buf), "Uptime: %02lu:%02lu:%02lu",
             (unsigned long)(s / 3600), (unsigned long)((s / 60) % 60), (unsigned long)(s % 60));
    lv_label_set_text(ui_lblUptime, buf);
}

static void refresh_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    refresh_values();
}

/* ------------------------------------------------------------------ */

void scr_home_prepare(void)
{
    /* Internal temperature sensor: install once, keep enabled */
    temperature_sensor_config_t cfg = TEMPERATURE_SENSOR_CONFIG_DEFAULT(-10, 80);
    if (temperature_sensor_install(&cfg, &s_temp_sensor) == ESP_OK) {
        temperature_sensor_enable(s_temp_sensor);
    } else {
        ESP_LOGE(TAG, "temperature sensor init failed");
        s_temp_sensor = NULL;
    }

    /* The battery arc is a display, not an input */
    lv_obj_clear_flag(ui_arcBattery, LV_OBJ_FLAG_CLICKABLE);
    lv_arc_set_range(ui_arcBattery, 0, 100);

    ESP_LOGD(TAG, "prepare");
}

void scr_home_init(void)
{
    refresh_values();   /* show real values immediately, not after 1 s */
    s_refresh_timer = lv_timer_create(refresh_timer_cb, HOME_REFRESH_MS, NULL);
    ESP_LOGD(TAG, "init");
}

void scr_home_uninit(void)
{
    if (s_refresh_timer) {
        lv_timer_del(s_refresh_timer);
        s_refresh_timer = NULL;
    }
    ESP_LOGD(TAG, "uninit");
}

void scr_home_step(void)
{
    /* Values are refreshed by the 1 s lv_timer */
}
