#include "memory_led.hpp"
#include "buzzer_driver.hpp"
#include "esp_random.h"

void memory_led_init(MemoryLearn* memory_learn) {
  memory_learn->leds->clear();
  memory_learn->lcd.clear();
  memory_learn->memory_led.level = 0;
  memory_learn->memory_led.correct_button = 0;
  memory_learn->memory_led.accumulated_time = 0;
  memory_learn->memory_led.state = MEMORY_LED_STATE_MENU;
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("Memory LED");
  memory_learn->lcd.setCursor(0, 1);
  memory_learn->lcd.print("Press any button!");
}

void memory_led_update(MemoryLearn* memory_learn, unsigned long delta_time) {
  MemoryLED* game = &memory_learn->memory_led;
  switch (game->state) {
    case MEMORY_LED_STATE_MENU:
      if (memory_learn->just_pressed_buttons) {
        memory_learn->lcd.clear();
        game->state = MEMORY_LED_STATE_PLAY;
      }
      break;
    case MEMORY_LED_STATE_PLAY:
      if (game->correct_button) {
        if (memory_learn->just_pressed_buttons & game->correct_button) {
          memory_learn->leds->clear();
          game->correct_button = 0;
          tone_buzzer_driver(&memory_learn->buzzer, 440, 250);
          delay(1000);
        } else if (memory_learn->just_pressed_buttons & ~game->correct_button || game->accumulated_time > 2000)
      } else {
        game->correct_button = (uint8_t)(esp_random() & 0x0F);
        uint8_t red = (uint8_t)(esp_random() & 0xFF);
        uint8_t green = (uint8_t)(esp_random() & 0xFF);
        uint8_t blue = (uint8_t)(esp_random() & 0xFF);
        for (uint8_t i = 0; i < 8; i++) {
          if (i == game->correct_button) {
            //TODO: Needs to be better...
            memory_learn->leds->setPixelColor(i, red - (0x04 - game->level), green - (0x04 - game->level), blue - (0x04 - game->level));
          } else {
            memory_learn->leds->setPixelColor(i, red, green, blue);
          }
        }
      }
      game->accumulated_time += delta_time;
      break;
    case MEMORY_LED_STATE_GAMEOVER:
      memory_learn->lcd.setCursor(0, 0);
      memory_learn->lcd.print("=-=Game over=-=");
      memory_learn->lcd.setCursor(0, 1);
      memory_learn->lcd.print("    Restart    ");
      if (memory_learn->just_pressed_buttons & BUTTON_OK) {
        memory_learn->lcd.clear();
        game->level = 0;
        game->accumulated_time = 0;
        game->state = MEMORY_LED_STATE_SHOW;
      } else if (memory_learn->just_pressed_buttons & BUTTON_NO) {
        memory_learn_set_state(memory_learn, MemoryLearnState::SELECT_GAME);
      }
      break;
    default:
      memory_learn_error(memory_learn, "Unexpected state");
  }
}
