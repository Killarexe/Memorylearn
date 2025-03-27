/* lcd_chars.hpp
 * Ici se trouve les graphiques pour les charatères customs.
 */
#ifndef LCD_CHARS_HPP
#define LCD_CHARS_HPP

/*
 * Ici se trouve tout les charatères ajoutée au LCD
 */

static unsigned char TOP_LEFT_ARROW[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000100,
  0b00001100,
  0b00011111
};

static unsigned char BOTTOM_LEFT_ARROW[8] = {
  0b00011111,
  0b00001100,
  0b00000100,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

static unsigned char TOP_RIGHT_ARROW[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000100,
  0b00000110,
  0b00011111,
};

static unsigned char BOTTOM_RIGHT_ARROW[8] = {
  0b00011111,
  0b00000110,
  0b00000100,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

static unsigned char ERROR_ICON_LEFT[8] = {
  0b00000001,
  0b00000010,
  0b00000010,
  0b00000101,
  0b00000101,
  0b00001000,
  0b00001001,
  0b00000111
};

static unsigned char ERROR_ICON_RIGHT[8] = {
  0b0010000,
  0b0001000,
  0b0001000,
  0b0010100,
  0b0010100,
  0b0000010,
  0b0010010,
  0b0011100
};

#endif
