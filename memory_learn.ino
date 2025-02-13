#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#include "memory_learn.hpp"
#include "buzzer_driver.hpp"
#include "led.hpp"
#include "lcd_chars.hpp"

MemoryLearn memory_learn;
LiquidCrystal_I2C lcd(0x27, 20, 4);
Adafruit_NeoPixel leds(8, LEDS_PIN, NEO_GRB + NEO_KHZ800);

SelectGame select_game;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, TOP_LEFT_ARROW);
  lcd.createChar(2, BOTTOM_LEFT_ARROW);
  lcd.createChar(3, TOP_RIGHT_ARROW);
  lcd.createChar(4, BOTTOM_RIGHT_ARROW);

  leds.begin();

  init_buzzer_driver(&memory_learn.buzzer);
  play_buzzer_driver(&memory_learn.buzzer, LED_MUSIC);
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!");
  memory_learn.state = MemoryLearnState::SELECT_GAME;
  init_select_game(&select_game, &memory_learn, &lcd, &leds);
}

void loop() {
  update_buzzer_driver(&memory_learn.buzzer);
}
