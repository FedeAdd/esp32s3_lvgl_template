#include "nav_bar.h"
#include "esp_log.h"

static const char *TAG = "nav_bar";

#define NAV_COLOR_BG        lv_color_hex(0x202020)
#define NAV_COLOR_IDLE      lv_color_hex(0x9E9E9E)
#define NAV_COLOR_ACTIVE    lv_color_hex(0x2196F3)

/* One icon per screen, same order as ui_screens_t */
static const char *s_icons[SCREEN_MAX] = {
    [SCREEN_HOME]     = LV_SYMBOL_HOME,
    [SCREEN_SYSTEM]   = LV_SYMBOL_LIST,
    [SCREEN_SETTINGS] = LV_SYMBOL_SETTINGS,
};

static lv_obj_t *s_bar = NULL;
static lv_obj_t *s_btn_labels[SCREEN_MAX];

/* ------------------------------------------------------------------ */

static void on_nav_btn_clicked(lv_event_t *e)
{
    ui_screens_t target = (ui_screens_t)(uintptr_t)lv_event_get_user_data(e);
    ui_screens_navigate(target);
}

static void on_gesture(lv_event_t *e)
{
    /* Guard: ignore gestures that start on a slider (e.g. dragging the
     * brightness slider horizontally must not change the screen). */
    lv_obj_t *origin = lv_event_get_target(e);
    if (lv_obj_check_type(origin, &lv_slider_class)) {
        return;
    }

    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    ui_screens_t current = ui_screens_get_current();
    if (current >= SCREEN_MAX) return;

    if (dir == LV_DIR_LEFT && current + 1 < SCREEN_MAX) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_screens_navigate((ui_screens_t)(current + 1));
    } else if (dir == LV_DIR_RIGHT && current > 0) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_screens_navigate((ui_screens_t)(current - 1));
    }
}

/* ------------------------------------------------------------------ */

void nav_bar_create(void)
{
    if (s_bar) return;   /* created only once */

    s_bar = lv_obj_create(lv_layer_top());
    lv_obj_set_size(s_bar, lv_pct(100), NAV_BAR_HEIGHT);
    lv_obj_align(s_bar, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(s_bar, NAV_COLOR_BG, 0);
    lv_obj_set_style_radius(s_bar, 0, 0);
    lv_obj_set_style_border_width(s_bar, 0, 0);
    lv_obj_set_style_pad_all(s_bar, 0, 0);
    lv_obj_clear_flag(s_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(s_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(s_bar, LV_FLEX_ALIGN_SPACE_EVENLY,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    for (int i = 0; i < SCREEN_MAX; i++) {
        lv_obj_t *btn = lv_btn_create(s_bar);
        lv_obj_set_size(btn, 80, NAV_BAR_HEIGHT - 6);
        lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);
        lv_obj_set_style_shadow_width(btn, 0, 0);
        lv_obj_add_event_cb(btn, on_nav_btn_clicked, LV_EVENT_CLICKED,
                            (void *)(uintptr_t)i);

        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, s_icons[i]);
        lv_obj_set_style_text_color(lbl, NAV_COLOR_IDLE, 0);
        lv_obj_center(lbl);
        s_btn_labels[i] = lbl;
    }
    ESP_LOGD(TAG, "created");
}

void nav_bar_set_active(ui_screens_t screen)
{
    if (!s_bar) return;
    for (int i = 0; i < SCREEN_MAX; i++) {
        lv_obj_set_style_text_color(s_btn_labels[i],
            (i == (int)screen) ? NAV_COLOR_ACTIVE : NAV_COLOR_IDLE, 0);
    }
}

void nav_enable_swipe(lv_obj_t *screen)
{
    /* A non-scrollable screen lets LVGL report the drag as a gesture */
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(screen, on_gesture, LV_EVENT_GESTURE, NULL);
}
