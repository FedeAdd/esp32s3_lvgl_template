#ifndef __NAV_BAR_H__
#define __NAV_BAR_H__

#include "lvgl.h"
#include "screens.h"

/**
 * Persistent bottom navigation bar + swipe navigation (TP2).
 *
 * The bar lives on lv_layer_top(), so it is created ONCE and stays
 * visible on top of every screen (it is not a separate screen).
 */

#define NAV_BAR_HEIGHT  40

/** Create the bar on the top layer. Call once (from a screen prepare()). */
void nav_bar_create(void);

/** Highlight the icon of the active screen. Call from each screen init(). */
void nav_bar_set_active(ui_screens_t screen);

/**
 * Enable horizontal swipe navigation on a screen object.
 * Swipe left -> next screen, swipe right -> previous screen.
 */
void nav_enable_swipe(lv_obj_t *screen);

#endif
