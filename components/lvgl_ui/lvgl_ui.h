#ifndef __LVGL_UI_H__
#define __LVGL_UI_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Bootstrap the UI layer. Calls ui_init() (Squareline), then registers
 * screen lifecycle callbacks and starts the 20 ms step timer.
 *
 * Must be called with the LVGL port lock held.
 */
void lvgl_ui_start(void);

/**
 * Notify the UI that the physical button was pressed. Safe to call from
 * any task (e.g. the espressif/button callback): it only enqueues a
 * command with ui_update_cmd(); the action runs later in the LVGL tick.
 */
void lvgl_ui_notify_button(void);

#ifdef __cplusplus
}
#endif

#endif
