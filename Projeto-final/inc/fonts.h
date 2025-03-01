/**
 * This Library was originally written by Olivier Van den Eede (4ilo) in 2016.
 * Some refactoring was done and SPI support was added by Aleksander Alekseev (afiskon) in 2018.
 *
 * https://github.com/afiskon/stm32-ssd1306
 */

 #ifndef __SSD1306_H__
 #define __SSD1306_H__

#ifdef SSD1306_INCLUDE_FONT_6x8
const SSD1306_Font_t Font_6x8 = {6, 8, Font6x8, NULL};
#endif
#ifdef SSD1306_INCLUDE_FONT_7x10
const SSD1306_Font_t Font_7x10 = {7, 10, Font7x10, NULL};
#endif
#ifdef SSD1306_INCLUDE_FONT_11x18
const SSD1306_Font_t Font_11x18 = {11, 18, Font11x18, NULL};
#endif
#ifdef SSD1306_INCLUDE_FONT_16x26
const SSD1306_Font_t Font_16x26 = {16, 26, Font16x26, NULL};
#endif
#ifdef SSD1306_INCLUDE_FONT_16x24
const SSD1306_Font_t Font_16x24 = {16, 24, Font16x24, NULL};
#endif
#ifdef SSD1306_INCLUDE_FONT_16x15
const SSD1306_Font_t Font_16x15 = {16, 15, Font16x15, NULL};

#endif

#endif // __SSD1306_FONTS_H__
