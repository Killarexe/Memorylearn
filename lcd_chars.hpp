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

static unsigned char ERROR_ICON[8] = {
  0b00000100,
  0b00001010,
  0b00001010,
  0b00010101,
  0b00010101,
  0b00010001,
  0b00010101,
  0b00001110
};

#endif
