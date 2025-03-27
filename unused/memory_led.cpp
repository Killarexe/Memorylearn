#include "memory_led.hpp"
#include "buzzer_driver.hpp"
#include "esp_random.h"

// Initialize le jeu
void memory_led_init(MemoryLearn* memory_learn) {
  memory_learn->leds->clear();
  memory_learn->lcd.clear();
  memory_learn->memory_led.level = 0;
  memory_learn->memory_led.triggered_button = 0;
  memory_learn->memory_led.reaction_time = 0;
  memory_learn->memory_led.state = MEMORY_LED_STATE_MENU;
  memory_learn->lcd.print("Memory LED: Press any button!");
}

// Met le jeu à jour selon l'état du jeu (non fini)
void memory_led_update(MemoryLearn* memory_learn, int64_t delta_time) {
  MemoryLED* game = &memory_learn->memory_led;
  switch (game->state) {
    case MEMORY_LED_STATE_MENU:
      // Attend qu'un bouton est appuyé
      if (memory_learn->buttons) {
        memory_learn->lcd.clear();
        game->state = MEMORY_LED_STATE_PLAY;
      }
      break;
    case MEMORY_LED_STATE_PLAY:
      // Si il y a bouton a appuyé
      if (game->triggered_button) {
        // Si le bouton est appuyé
        if (memory_learn->buttons & game->triggered_button) {
          // Attendre 1 seconde pour le prochain
          memory_learn->leds->clear();
          game->triggered_button = 0;
          delay(1000);
        }
      } else {
        // Choisi un bouton à appuyer aléatoirement avec une couleur aléatoire
        game->triggered_button = (uint8_t)(esp_random() & 0x0F);
        uint8_t red = (uint8_t)(esp_random() & 0xFF);
        uint8_t green = (uint8_t)(esp_random() & 0xFF);
        uint8_t blue = (uint8_t)(esp_random() & 0xFF);
        // Met la couleur aléatoire pour chaques LED sauf pour le bouton qu'on doit appuyé à une lègère difference
        for (uint8_t i = 0; i < 8; i++) {
          if (i == game->triggered_button) {
            //TODO: Needs to be better...
            memory_learn->leds->setPixelColor(i, red - (255 - game->level), green - (255 - game->level), blue - (255 - game->level));
          } else {
            memory_learn->leds->setPixelColor(i, red, green, blue);
          }
        }
      }
      break;
    case MEMORY_LED_STATE_GAMEOVER:
      // Affiche l'écran de gameover sur l'LCD et attend si le joueur veut rejouer ou non.
      memory_learn->lcd.setCursor(0, 0);
      memory_learn->lcd.print("=-=Game over=-=");
      memory_learn->lcd.setCursor(0, 1);
      memory_learn->lcd.print("    Restart    ");
      if (memory_learn->buttons & BUTTON_OK) {
        memory_learn->lcd.clear();
        game->level = 0;
        game->reaction_time = 0;
        game->state = MEMORY_LED_STATE_SHOW;
      } else if (memory_learn->buttons & BUTTON_NO) {
        memory_learn_set_state(memory_learn, MemoryLearnState::SELECT_GAME);
      }
      break;
    default:
      memory_learn_error(memory_learn, "Unexpected state");
  }
}
