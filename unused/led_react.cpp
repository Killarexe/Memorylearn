#include "esp_random.h"
#include "led_react.hpp"

// Initalize le jeu
void led_react_init(MemoryLearn* memory_learn) {
  memory_learn->leds->clear();
  memory_learn->lcd.clear();
  memory_learn->led_react.level = 0;
  memory_learn->led_react.reaction_time = 0;
  memory_learn->led_react.state = LED_REACT_STATE_MENU;
  memory_learn->led_react.triggered_button = 0;
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("LED React: Press any button");
}

// Met le jeu à jour selon l'état du jeu
void led_react_update(MemoryLearn* memory_learn, int64_t delta_time) {
  LEDReact* game = &memory_learn->led_react;
  switch (game->state) {
    case LED_REACT_STATE_MENU:
      // Attend qu'un bouton est appuyé
      if (memory_learn->buttons) {
        memory_learn->lcd.clear();
        game->state = LED_REACT_STATE_PLAY;
      }
      break;
    case LED_REACT_STATE_PLAY:
      // Si il y a un bouton à appuyer
      if (game->triggered_button) {
        int64_t max_reaction_time = 1000; //TODO: Needs to depends on the level.
        // Si le bouton appuyé et mauvais ou que le temps écoulé
        if (memory_learn->buttons & ~game->triggered_button || game->reaction_time > max_reaction_time) {
          game->state = LED_REACT_STATE_GAMEOVER; // Game over
          memory_learn->leds->clear();
        } else if (memory_learn->buttons & game->triggered_button) {
          // Sinon le bon bouton appuyé, passer au prochain
          memory_learn->leds->clear();
          game->triggered_button = 0;
        }
      } else if (game->reaction_time > 500) {
        //Sinon attender 0.5s et attribuer un nouveau bouton a appuyer et allumer la LED
        uint8_t random = (uint8_t)(esp_random() & 0xFF);
        uint8_t chance = 128; //TODO: Needs to depends on the level.
        if (random > chance) {
          uint8_t button_index = (uint8_t)(esp_random() & 0x0F);
          game->triggered_button = 1 << button_index;
          memory_learn->leds->setPixelColor(button_index, 0xFF, 0xFF, 0xFF);
          memory_learn->leds->show();
        }
        game->reaction_time = 0;
      }
      game->reaction_time += delta_time;
      break;
    case LED_REACT_STATE_GAMEOVER:
      // Affiche l'écran de gameover sur l'LCD et attend si le joueur veut rejouer ou non.
      memory_learn->lcd.setCursor(0, 0);
      memory_learn->lcd.print("=-=Game over=-=");
      memory_learn->lcd.setCursor(0, 1);
      memory_learn->lcd.print("    Restart    ");
      if (memory_learn->buttons & BUTTON_OK) {
        memory_learn->lcd.clear();
        game->level = 0;
        game->reaction_time = 0;
        game->triggered_button = 0;
        game->state = LED_REACT_STATE_PLAY;
      } else if (memory_learn->buttons & BUTTON_NO) {
        memory_learn_set_state(memory_learn, MemoryLearnState::SELECT_GAME);
      }
      break;
    default:
      memory_learn_error(memory_learn, "Unexpected state");
  }
}
