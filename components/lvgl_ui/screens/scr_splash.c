#include "scr_splash.h"
#include "screens.h"
#include "ui.h"             /* ui_scrSplash, ui_lblLoading */
#include "bsp_display.h"    /* bsp_display_set_brightness() */
#include "esp_log.h"

static const char *TAG = "scr_splash";

#define SPLASH_FADE_MS      800     /* backlight fade-in duration */
#define SPLASH_DOTS_MS      400     /* "Iniciando" dot animation period */

static lv_timer_t *s_dots_timer = NULL;
static uint8_t     s_dots = 0;

/* ------------------------------------------------------------------ */
/* Callbacks — all run inside the LVGL context (event / anim / timer) */
/* ------------------------------------------------------------------ */

/* lv_anim exec callback: value goes 0 -> 100 over SPLASH_FADE_MS */
static void fade_exec_cb(void *var, int32_t value)
{
    (void)var;
    bsp_display_set_brightness((uint8_t)value);
}

/* "Iniciando", "Iniciando.", "Iniciando..", "Iniciando..." */
static void dots_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    s_dots = (s_dots + 1) % 4;
    lv_label_set_text_fmt(ui_lblLoading, "Iniciando%.*s", (int)s_dots, "...");
}

/* Touch anywhere on the splash -> go to home */
static void on_splash_clicked(lv_event_t *e)
{
    (void)e;
    ui_screens_navigate(SCREEN_HOME);
}

/* ------------------------------------------------------------------ */

void scr_splash_prepare(void)
{
    /* Labels are not clickable, so a tap anywhere reaches the screen */
    lv_obj_add_flag(ui_scrSplash, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui_scrSplash, on_splash_clicked, LV_EVENT_CLICKED, NULL);
    ESP_LOGD(TAG, "prepare");
}

void scr_splash_init(void)
{
    /* Backlight fade-in */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, ui_scrSplash);
    lv_anim_set_exec_cb(&a, fade_exec_cb);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_set_time(&a, SPLASH_FADE_MS);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in);
    lv_anim_start(&a);

    /* Animated dots */
    s_dots = 0;
    lv_label_set_text(ui_lblLoading, "Iniciando");
    s_dots_timer = lv_timer_create(dots_timer_cb, SPLASH_DOTS_MS, NULL);

    ESP_LOGD(TAG, "init");
}

void scr_splash_uninit(void)
{
    /* Stop the fade (if the user tapped before it finished) and leave
     * the backlight fully on for the next screen. */
    lv_anim_del(ui_scrSplash, fade_exec_cb);
    bsp_display_set_brightness(100);

    if (s_dots_timer) {
        lv_timer_del(s_dots_timer);
        s_dots_timer = NULL;
    }
    ESP_LOGD(TAG, "uninit");
}

void scr_splash_step(void)
{
    /* Nothing to poll: everything is driven by the anim and the timer */
}
