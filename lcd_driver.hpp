#ifndef LCD_DRIVER_HPP
#define LCD_DRIVER_HPP

#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <stdint.h>

/* Uses I2C 100khz mode */
#define STANDARD_RATE 100000

/*
 * All these define statemens are based
 * on pages 24-25 of the HD44780U datasheet pdf.
 */

/* Commands. */
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

/* Flags for Entry mode. */
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYLEFT 0x02

/* Flags for display and cursor control. */
#define LCD_BLINKON 0x01
#define LCD_CURSORON 0x02
#define LCD_DISPLAYON 0x04

/* Flags for display and cursor shift. */
#define LCD_MOVERIGHT 0x04
#define LCD_DISPLAYMOVE 0x08

/* Flags for function set. */
#define LCD_5x10DOTS 0x04
#define LCD_2LINE 0x08
#define LCD_8BITMODE 0x10

/* Flag for backlight control. */
#define LCD_BACKLIGHT 0x08

/*
 * E (Enable) pin is used to enable the display.
 * When this pin is set to LOW, the LCD does not
 * care what is happening on the R/W, RS, and data bus lines.
 * When this pin is set to HIGH, the LCD processes the incoming data.
 *
 * Explanation taken from
 * https://lastminuteengineers.com/arduino-1602-character-lcd-tutorial/
 */

#define LCD_ENABLE_PIN 0x04

/*
 * RS (Register Select) pin is set to LOW when sending commands
 * to the LCD (such as setting the cursor to a specific location, clearing the display, etc.)
 * and HIGH when sending data to the LCD. Basically this pin
 * is used to separate the command from the data.
 *
 * Explanation taken from
 * https://lastminuteengineers.com/arduino-1602-character-lcd-tutorial/
 */
#define LCD_COMMAND_MODE 0x00
#define LCD_CHAR_MODE 0x01

/* 1602 config */
#define MAX_LINES 2
#define MAX_CHARS 16

/* Delay values in microseconds, check page 24 */
#define FAST_DELAY 80
#define SLOW_DELAY 2000 // Used for Returning home and Clearing the display

#define LCD_FLAGS_SHIFT_RIGHT 1
#define LCD_FLAGS_SHIFT_DISPLAY 2
#define LCD_FLAGS_BACKLIGHT 4
#define LCD_FLAGS_CURSOR_BLINKING 8
#define LCD_FLAGS_DISPLAY_ENABLE 16
#define LCD_FLAGS_DISPLAY_SHIFT 32
#define LCD_FLAGS_CURSOR_ENABLE 64
#define LCD_FLAGS_CURSOR_DIRECTION 128

/*
 * LCD flags:
 * 0bDCSOBLPR
 *   |||||||+- Shift right
 *   ||||||+-- Shift display
 *   |||||+--- Backlight ON
 *   ||||+---- Cursor blinking ON
 *   |||+----- Display ON
 *   ||+------ Display shift
 *   |+------- Cursor ON
 *   +-------- Cursor direction
 */
typedef struct LCD {
  uint8_t address;
  uint8_t sda_pin;
  uint8_t scl_pin;
  uint8_t i2c_type;
  uint8_t flags;
} LCD;

void lcd_toggle_enable(LCD* lcd, uint8_t value, const uint64_t delay_us);

void lcd_send_byte(LCD* lcd, uint8_t value, uint8_t mode, uint64_t delay_us);

void lcd_init(LCD* lcd, uint8_t address, uint8_t sda_pin, uint8_t scl_pin, i2c_inst_t* i2c_inst);

void lcd_clear(LCD* lcd);

void lcd_putc(LCD* lcd, char chr);

void lcd_print(LCD* lcd, const char* string);

void lcd_set_cursor_pos(LCD* lcd, uint8_t line, uint8_t column);

void lcd_set_flags(LCD* lcd, uint8_t flags);

void lcd_shift_display(LCD* lcd, bool direction, uint8_t amount);

void lcd_shift_cursor(LCD* lcd, bool direction, uint8_t amount);

#endif
