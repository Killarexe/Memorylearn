#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#include "memory_learn.hpp"
#include "buzzer_driver.hpp"
#include "led.hpp"
#include "lcd_chars.hpp"

MemoryLearn memory_learn;
LiquidCrystal_I2C lcd(0x27, 20, 4);
Adafruit_NeoPixel leds(8, LEDS_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, TOP_LEFT_ARROW);
  lcd.createChar(1, BOTTOM_LEFT_ARROW);
  lcd.createChar(2, TOP_RIGHT_ARROW);
  lcd.createChar(3, BOTTOM_RIGHT_ARROW);

  leds.begin();

  init_buzzer_driver(&memory_learn.buzzer);
  play_buzzer_driver(&memory_learn.buzzer, LED_MUSIC);
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!");
}

void loop() {;
  update_buzzer_driver(&memory_learn.buzzer);
}
