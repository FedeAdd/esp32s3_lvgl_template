#ifndef __SCR_HOME_H__
#define __SCR_HOME_H__

/**
 * Home screen (TP2): battery, chip temperature, uptime and a colour-coded
 * status indicator (normal / alert / critical) driven by the temperature.
 */
void scr_home_prepare(void);
void scr_home_init(void);
void scr_home_uninit(void);
void scr_home_step(void);

/** Physical button on this screen: force an immediate refresh. */
void scr_home_on_button(void);

#endif
