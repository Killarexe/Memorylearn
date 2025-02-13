#include "select_game.hpp"

void init_select_game(SelectGame* game, MemoryLearn* memory_learn, LiquidCrystal_I2C* lcd, Adafruit_NeoPixel* led) {
  game->cursor_index = 0;
  lcd.setCursor(0, 0);
  lcd.print("\x01 Select a game\x02\x03 Hello, world!\x04");
}



void update_select_game(SelectGame* game, MemoryLearn* memory_learn, LiquidCrystal_I2C* lcd) {

}