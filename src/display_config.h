/**************************************************************
 *
 *                     display_config.h
 *
 *     Assignment: ST7789_display
 *     Author:    AJ Romeo
 *     Date:      December 30, 2025
 *
 *     Project-wide display geometry configuration (width/height).
 *
 **************************************************************/

#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

/*
 * Default panel size.
 *
 * Override at build time with:
 *      -DSCREEN_WIDTH=... -DSCREEN_HEIGHT=...
 */
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 320
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 240
#endif

#endif
