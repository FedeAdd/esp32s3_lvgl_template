#ifndef __SCR_SYSTEM_H__
#define __SCR_SYSTEM_H__

/**
 * System screen (TP2): CPU frequency, free heap, minimum free heap
 * (resettable), IDF version, chip model and flash size.
 */
void scr_system_prepare(void);
void scr_system_init(void);
void scr_system_uninit(void);
void scr_system_step(void);

/** Physical button on this screen: reset the minimum-heap tracker. */
void scr_system_on_button(void);

#endif
