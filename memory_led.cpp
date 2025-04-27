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

void memory_led_update_menu(MemoryLearn* memory_learn, MemoryLED* game, unsigned long delta_time) {
  if (memory_learn->just_pressed_buttons) {
    memory_learn->lcd.clear();
    game->state = MEMORY_LED_STATE_PLAY;
  }
}

void memory_led_update_play(MemoryLearn* memory_learn, MemoryLED* game, unsigned long delta_time) {
  if (game->correct_button) {
    uint8_t target_button = 1 << (game->correct_button - 1);
    if (memory_learn->just_pressed_buttons & target_button) {
      memory_learn->leds->clear();
      game->correct_button = 0;
      tone_buzzer_driver(&memory_learn->buzzer, 440, 250);
    } else if (memory_learn->just_pressed_buttons & ~target_button || game->accumulated_time > 5000) {
      for (uint8_t j = 0; j < 8; j++) {
        memory_learn->leds->setPixelColor(j, 6, 0, 0);
      }
      memory_learn->leds->show();
      game->state = MEMORY_LED_STATE_GAMEOVER;
      tone_buzzer_driver(&memory_learn->buzzer, 220, 250);
    }
  } else if (game->accumulated_time > 2000) {
    game->correct_button = (uint8_t)(esp_random() & 0x07) + 1;
    
    uint8_t red = (uint8_t)(esp_random() & 0x1F);
    uint8_t green = (uint8_t)(esp_random() & 0x1F);
    uint8_t blue = (uint8_t)(esp_random() & 0x1F);

    uint8_t diff_red = red;
    uint8_t diff_green = green;
    uint8_t diff_blue = blue;
    uint8_t color = (uint8_t)(esp_random() & 0x03);
    int8_t max_diff = 16 - (game->level > 15 ? 15 : game->level);
    if (max_diff < 1) {
      max_diff = 1;
    }
    uint8_t diff = 1 + (uint8_t)(esp_random() % max_diff);
    int8_t diff_dir = (int8_t)(esp_random() % 2) * 2 - 1;
    switch (color) {
      case 0:
        diff_red = (diff_red + diff_dir * diff) & 0x1F;
        break;
      case 1:
        diff_green = (diff_green + diff_dir * diff) & 0x1F;
        break;
      case 2:
        diff_blue = (diff_blue + diff_dir * diff) & 0x1F;
        break;
      default:
        memory_learn_error(memory_learn, "Well, that's odd");
        return;
    }
    for (uint8_t i = 0; i < 8; i++) {
      if (i + 1 == game->correct_button) {
        memory_learn->leds->setPixelColor(i, diff_red, diff_green, diff_blue);
      } else {
        memory_learn->leds->setPixelColor(i, red, green, blue);
      }
    }
    memory_learn->leds->show();
    game->accumulated_time = 0;
  }
  game->accumulated_time += delta_time;
}

void memory_led_update_gameover(MemoryLearn* memory_learn, MemoryLED* game, unsigned long delta_time) {
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("=-=Game over=-=");
  memory_learn->lcd.setCursor(0, 1);
  memory_learn->lcd.print("    Restart    ");
  if (memory_learn->just_pressed_buttons & BUTTON_OK) {
    memory_learn->lcd.clear();
    game->level = 0;
    game->accumulated_time = 0;
    game->correct_button = 0;
    game->state = MEMORY_LED_STATE_PLAY;
    memory_learn->leds->clear();
    memory_learn->leds->show();
  } else if (memory_learn->just_pressed_buttons & BUTTON_NO) {
    memory_learn_set_state(memory_learn, MemoryLearnState::SELECT_GAME);
  }
}

void memory_led_update(MemoryLearn* memory_learn, unsigned long delta_time) {
  MemoryLED* game = &memory_learn->memory_led;
  switch (game->state) {
    case MEMORY_LED_STATE_MENU:
      memory_led_update_menu(memory_learn, game, delta_time);
      break;
    case MEMORY_LED_STATE_PLAY:
      memory_led_update_play(memory_learn, game, delta_time);
      break;
    case MEMORY_LED_STATE_GAMEOVER:
      memory_led_update_gameover(memory_learn, game, delta_time);
      break;
    default:
      memory_learn_error(memory_learn, "Unexpected state");
  }
}
