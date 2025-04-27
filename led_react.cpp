#include "esp_random.h"
#include "led_react.hpp"

void led_react_init(MemoryLearn* memory_learn) {
  memory_learn->leds->clear();
  memory_learn->lcd.clear();
  memory_learn->led_react.level = 0;
  memory_learn->led_react.accumulated_time = 0;
  memory_learn->led_react.state = LED_REACT_STATE_MENU;
  memory_learn->led_react.correct_button = 0;
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("LED React");
  memory_learn->lcd.setCursor(0, 1);
  memory_learn->lcd.print("Press any button!");
}

void led_react_update_menu(MemoryLearn* memory_learn, LEDReact* game, unsigned long delta_time) {
  if (memory_learn->just_pressed_buttons) {
    memory_learn->lcd.clear();
    memory_learn->lcd.setCursor(0, 0);
    memory_learn->lcd.print("     Ready.     ");
    game->state = LED_REACT_STATE_PLAY;
  }
}

void led_react_update_play(MemoryLearn* memory_learn, LEDReact* game, unsigned long delta_time) {
  if (game->correct_button != 0) {
    unsigned long max_reaction_time = 10000 >> (game->level >> 2); //TODO: Needs to depends on the level.
    // Si le bouton appuyé et mauvais ou que le temps écoulé
    if (memory_learn->just_pressed_buttons & ~game->correct_button || game->accumulated_time > max_reaction_time) {
      game->state = LED_REACT_STATE_GAMEOVER;
      memory_learn->leds->clear();
      tone_buzzer_driver(&memory_learn->buzzer, 220, 250);
    } else if (memory_learn->just_pressed_buttons & game->correct_button) {
      // Sinon le bon bouton appuyé, passer au prochain
      memory_learn->leds->clear();
      memory_learn->leds->show();
      game->correct_button = 0;
      game->level++;
      tone_buzzer_driver(&memory_learn->buzzer, 880, 250);
      memory_learn->lcd.clear();
      memory_learn->lcd.setCursor(0, 0);
      memory_learn->lcd.print("     Ready.     ");
    }
  } else if (game->accumulated_time > (2000 - (game->level * 100))) {
    uint8_t random = (uint8_t)(esp_random() & 0xFF);
    uint8_t chance = 128; //TODO: Needs to depends on the level.
    if (random > chance) {
      uint8_t button_index = (uint8_t)(esp_random() & 0x07);
      game->correct_button = 1 << button_index;
      memory_learn->leds->setPixelColor(button_index, 0x06, 0x06, 0x06);
      memory_learn->leds->show();
      game->accumulated_time = 0;
      tone_buzzer_driver(&memory_learn->buzzer, 440, 250);
      memory_learn->lcd.setCursor(0, 1);
      memory_learn->lcd.print("     React!     ");
    }
  }
  game->accumulated_time += delta_time;
}

void led_react_update_gameover(MemoryLearn* memory_learn, LEDReact* game, unsigned long delta_time) {
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("=-=Game over=-=");
  memory_learn->lcd.setCursor(0, 1);
  memory_learn->lcd.print("    Restart    ");
  if (memory_learn->just_pressed_buttons & BUTTON_OK) {
    memory_learn->lcd.clear();
    game->level = 0;
    game->accumulated_time = 0;
    game->correct_button = 0;
    game->state = LED_REACT_STATE_PLAY;
    memory_learn->lcd.clear();
    memory_learn->lcd.setCursor(0, 0);
    memory_learn->lcd.print("     Ready.     ");
  } else if (memory_learn->just_pressed_buttons & BUTTON_NO) {
    memory_learn_set_state(memory_learn, MemoryLearnState::SELECT_GAME);
  }
}

void led_react_update(MemoryLearn* memory_learn, unsigned long delta_time) {
  LEDReact* game = &memory_learn->led_react;
  switch (game->state) {
    case LED_REACT_STATE_MENU:
      led_react_update_menu(memory_learn, game, delta_time);
      break;
    case LED_REACT_STATE_PLAY:
      led_react_update_play(memory_learn, game, delta_time);
      break;
    case LED_REACT_STATE_GAMEOVER:
      led_react_update_gameover(memory_learn, game, delta_time);
      break;
    default:
      memory_learn_error(memory_learn, "Unexpected state");
  }
}
