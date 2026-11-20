#ifndef __SCR_SPLASH_H__
#define __SCR_SPLASH_H__

/**
 * Splash screen controller (TP1).
 *  - Fades the backlight in over SPLASH_FADE_MS.
 *  - Animates the "Iniciando" label with 0..3 dots.
 *  - Touching anywhere navigates to the home screen.
 */
void scr_splash_prepare(void);
void scr_splash_init(void);
void scr_splash_uninit(void);
void scr_splash_step(void);

#endif
