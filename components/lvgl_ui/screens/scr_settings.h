#ifndef __SCR_SETTINGS_H__
#define __SCR_SETTINGS_H__

/**
 * Settings screen (TP2): brightness slider (LEDC), screen-timeout switch
 * and build date/time.
 */
void scr_settings_prepare(void);
void scr_settings_init(void);
void scr_settings_uninit(void);
void scr_settings_step(void);

/** Physical button on this screen: restore defaults. */
void scr_settings_on_button(void);

#endif
