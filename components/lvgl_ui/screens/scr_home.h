#ifndef __SCR_HOME_H__
#define __SCR_HOME_H__

/**
 * Home screen controller (TP1).
 * Shows battery level (ADC), chip temperature (internal sensor) and
 * uptime, refreshed every HOME_REFRESH_MS with an lv_timer.
 */
void scr_home_prepare(void);
void scr_home_init(void);
void scr_home_uninit(void);
void scr_home_step(void);

#endif
