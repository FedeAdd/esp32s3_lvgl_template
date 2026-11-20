#include "scr_settings.h"
#include "screens.h"
#include "screens_timeout.h"
#include "nav_bar.h"
#include "ui.h"     /* ui_scrSettings, ui_sldBrightness, ui_lblBrightness,
                       ui_swTimeout, ui_lblBuild */
#include "bsp_display.h"
#include "esp_log.h"

static const char *TAG = "scr_settings";

#define DEFAULT_BRIGHTNESS  100
#define MIN_BRIGHTNESS      10      /* never let the screen go fully black */
#define SCREEN_TIMEOUT_MS   30000   /* display off after 30 s without touch */

/* ------------------------------------------------------------------ */

static void apply_brightness(int value)
{
    bsp_display_set_brightness((uint8_t)value);   /* LEDC duty, real time */
    lv_label_set_text_fmt(ui_lblBrightness, "%d%%", value);
}

static void apply_timeout(bool enabled)
{
    disp_timeout_set_off_ms(enabled ? SCREEN_TIMEOUT_MS : TIMEOUT_NEVER);
}

static void on_brightness_changed(lv_event_t *e)
{
    (void)e;
    apply_brightness(lv_slider_get_value(ui_sldBrightness));
}

static void on_timeout_changed(lv_event_t *e)
{
    (void)e;
    apply_timeout(lv_obj_has_state(ui_swTimeout, LV_STATE_CHECKED));
}

/* ------------------------------------------------------------------ */

void scr_settings_prepare(void)
{
    nav_enable_swipe(ui_scrSettings);

    lv_slider_set_range(ui_sldBrightness, MIN_BRIGHTNESS, 100);
    lv_slider_set_value(ui_sldBrightness, DEFAULT_BRIGHTNESS, LV_ANIM_OFF);
    /* Dragging the slider must not be interpreted as a screen swipe */
    lv_obj_clear_flag(ui_sldBrightness, LV_OBJ_FLAG_GESTURE_BUBBLE);

    lv_obj_add_event_cb(ui_sldBrightness, on_brightness_changed, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_swTimeout, on_timeout_changed, LV_EVENT_VALUE_CHANGED, NULL);

    lv_label_set_text(ui_lblBuild, "Build: " __DATE__ " " __TIME__);
}

void scr_settings_init(void)
{
    nav_bar_set_active(SCREEN_SETTINGS);
    /* Show the real current brightness */
    int value = bsp_display_get_brightness();
    if (value < MIN_BRIGHTNESS) value = MIN_BRIGHTNESS;
    lv_slider_set_value(ui_sldBrightness, value, LV_ANIM_OFF);
    lv_label_set_text_fmt(ui_lblBrightness, "%d%%", value);
}

void scr_settings_uninit(void)
{
}

void scr_settings_step(void)
{
}

void scr_settings_on_button(void)
{
    ESP_LOGI(TAG, "button: restore defaults");
    lv_slider_set_value(ui_sldBrightness, DEFAULT_BRIGHTNESS, LV_ANIM_ON);
    apply_brightness(DEFAULT_BRIGHTNESS);

    lv_obj_clear_state(ui_swTimeout, LV_STATE_CHECKED);
    apply_timeout(false);
}
