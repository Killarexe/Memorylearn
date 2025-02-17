#include "lcd_driver.hpp"

void lcd_write_byte(LCD* lcd, uint8_t value) {
  i2c_write_blocking(lcd->i2c_type == 1 ? i2c1 : i2c0, lcd->address, &value, 1, false);
}

void lcd_toggle_enable(LCD *lcd, uint8_t value, const int delay_us) {
  sleep_us(delay_us);!
  lcd_write_byte(lcd, value | LCD_ENABLE_PIN);
  sleep_us(delay_us);
  lcd_write_byte(lcd, value & ~LCD_ENABLE_PIN);
}

void lcd_send_byte(LCD *lcd, uint8_t value, uint8_t mode, uint64_t delay_us) {
  uint8_t high = mode | (value & 0xF0) | (lcd->flags & LCD_FLAGS_BACKLIGHT);
  uint8_t low = mode | (value << 4) | (lcd->flags & LCD_FLAGS_BACKLIGHT);

  if (mode == LCD_COMMAND_MODE) {
    if (value & LCD_CURSORSHIFT) {
      low |= (lcd->flags & LCD_FLAGS_SHIFT_DISPLAY) << 6;
      low |= (lcd->flags & LCD_FLAGS_SHIFT_RIGHT) << 7;
    } else if (value & LCD_DISPLAYCONTROL) {
      low |= (lcd->flags & LCD_FLAGS_DISPLAY_ENABLE) << 2;
      low |= (lcd->flags & LCD_FLAGS_CURSOR_ENABLE) >> 1;
      low |= (lcd->flags & LCD_FLAGS_CURSOR_BLINKING) << 1;
    } else if (value & LCD_ENTRYMODESET) {
      low |= (lcd->flags & LCD_FLAGS_CURSOR_DIRECTION) >> 2;
      low |= (lcd->flags & LCD_FLAGS_DISPLAY_SHIFT) >> 1;
    }
  }

  lcd_toggle_enable(lcd, high, delay_us);
  lcd_toggle_enable(lcd, low, delay_us);
}

void lcd_init(LCD *lcd, uint8_t address, uint8_t sda_pin, uint8_t scl_pin, i2c_inst_t* i2c_inst) {
  i2c_init(i2c_inst, STANDARD_RATE);
  gpio_set_function(sda_pin, GPIO_FUNC_I2C);
  gpio_set_function(scl_pin, GPIO_FUNC_I2C);
  gpio_pull_up(sda_pin);
  gpio_pull_up(scl_pin);

  lcd->address = (address == 0xFF) ? 0x27 : address;
  lcd->i2c_type = (i2c_inst == i2c1);
  lcd->sda_pin = sda_pin;
  lcd->scl_pin = scl_pin;
  lcd->flags = LCD_FLAGS_CURSOR_DIRECTION | LCD_FLAGS_DISPLAY_ENABLE | LCD_FLAGS_BACKLIGHT;

  sleep_us(40000);
  lcd_toggle_enable(lcd, LCD_FUNCTIONSET | LCD_8BITMODE, FAST_DELAY);
  sleep_us(4100);
  lcd_toggle_enable(lcd, LCD_FUNCTIONSET | LCD_8BITMODE, FAST_DELAY);
  sleep_us(100);
  lcd_toggle_enable(lcd, LCD_FUNCTIONSET | LCD_8BITMODE, FAST_DELAY);
  lcd_toggle_enable(lcd, LCD_FUNCTIONSET, FAST_DELAY);

  lcd_send_byte(lcd, LCD_ENTRYMODESET, LCD_COMMAND_MODE, FAST_DELAY);
  lcd_send_byte(lcd, LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND_MODE, FAST_DELAY);
  lcd_send_byte(lcd, LCD_DISPLAYCONTROL, LCD_COMMAND_MODE, FAST_DELAY);

  sleep_ms(1);
  lcd_clear(lcd);
}

void lcd_clear(LCD *lcd) {
  lcd_send_byte(lcd, LCD_CLEARDISPLAY, LCD_COMMAND_MODE, SLOW_DELAY);
}

void lcd_putc(LCD *lcd, char chr) {
  lcd_send_byte(lcd, chr, LCD_CHAR_MODE, FAST_DELAY);
}

void lcd_print(LCD *lcd, const char *string) {
  while (*string) {
    lcd_putc(lcd, *string++);
  }
}

void lcd_set_cursor_pos(LCD *lcd, uint8_t line, uint8_t column) {
  if (column >= MAX_CHARS || line >= MAX_LINES) {
    return;
  }
  uint8_t position = line == 0 ? 0x80 + column : 0xC0 + column;
  lcd_send_byte(lcd, position, LCD_COMMAND_MODE, FAST_DELAY);
}

void lcd_set_flags(LCD *lcd, uint8_t flags) {
  lcd->flags = flags;
  lcd_send_byte(lcd, LCD_DISPLAYCONTROL, LCD_COMMAND_MODE, FAST_DELAY);
}

void lcd_shift_display(LCD *lcd, bool direction, uint8_t amount) {
  lcd->flags |= LCD_FLAGS_SHIFT_DISPLAY;
  if (direction) {
    lcd->flags |= LCD_FLAGS_SHIFT_RIGHT;
  }
  while (amount--) {
    lcd_send_byte(lcd, LCD_CURSORSHIFT, LCD_COMMAND_MODE, FAST_DELAY);
  }
}

void lcd_shift_cursor(LCD *lcd, bool direction, uint8_t amount) {
  lcd->flags &= ~LCD_FLAGS_SHIFT_DISPLAY;
  if (direction) {
    lcd->flags |= LCD_FLAGS_SHIFT_RIGHT;
  }
  while (amount--) {
    lcd_send_byte(lcd, LCD_ENTRYMODESET, LCD_COMMAND_MODE, FAST_DELAY);
  }
}
