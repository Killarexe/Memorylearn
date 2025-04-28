#include "simon_game.hpp"
#include "buzzer_driver.hpp"
#include "esp_random.h"

// Couleur de chaque boutons
const uint8_t BUTTON_COLORS[8][3] = {
  {0x00, 0x06, 0x00},
  {0x06, 0x06, 0x06},
  {0x00, 0x00, 0x06},
  {0x06, 0x04, 0x00},
  {0x04, 0x00, 0x06},
  {0x06, 0x00, 0x00},
  {0x06, 0x00, 0x06},
  {0x06, 0x06, 0x00}
};

// Frequence sonore de chaque boutons
const uint16_t BUTTON_FREQUENCIES[8] = {
  440,
  466,
  494,
  523,
  554,
  587,
  622,
  659
};

// Donne la liste des boutons à appuyé
void randomize_buttons(SimonGame* simon_game) {
  simon_game->buttons = (uint8_t*)realloc(simon_game->buttons, simon_game->level);
  for (uint8_t i = 0; i < simon_game->level; i++) {
    uint8_t random = (uint8_t)(esp_random() & 0xFF);
    simon_game->buttons[i] = random % 8;
    Serial.print("Button ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(simon_game->buttons[i]);
  }
}

// Initialize le jeu
void simon_game_init(MemoryLearn* memory_learn) {
  memory_learn->just_pressed_buttons = 0;
  memory_learn->leds->clear();
  memory_learn->lcd.clear();
  memory_learn->simon_game.level = 1;
  memory_learn->simon_game.button_index = 0;
  memory_learn->simon_game.reaction_time = 0;
  memory_learn->simon_game.state = SIMON_GAME_STATE_MENU;
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("Simon");
  memory_learn->lcd.setCursor(0, 1);
  memory_learn->lcd.print("Press any button!");
}

void simon_game_update_menu(MemoryLearn* memory_learn, SimonGame* game, unsigned long delta_time) {
  if (memory_learn->just_pressed_buttons) {
    randomize_buttons(&memory_learn->simon_game);
    memory_learn->lcd.clear();
    game->state = SIMON_GAME_STATE_SHOW;
  }
}

void simon_game_update_show(MemoryLearn* memory_learn, SimonGame* game, unsigned long delta_time) {
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("     Wait..     ");
  // Montre les boutons a appuyé avec les leds et le son
  for (uint8_t i = 0; i < game->level; i++) {
    uint8_t button_index = game->buttons[i];
    const uint8_t* rgb_color = BUTTON_COLORS[button_index];
    uint32_t duration = 1000 >> (game->level >> 2); //TODO: needs to depends on leve
    tone_buzzer_driver(&memory_learn->buzzer, BUTTON_FREQUENCIES[button_index], duration);
    memory_learn->leds->clear();
    memory_learn->leds->setPixelColor(button_index, rgb_color[0], rgb_color[1], rgb_color[2]);
    memory_learn->leds->show();
    delay(duration);
    noTone(memory_learn->buzzer.output_pin);
    delay(100);
    
    memory_learn->leds->setPixelColor(button_index, 0, 0, 0);
    memory_learn->leds->show();
  }
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("   Your turn!   ");
  memory_learn->leds->clear();
  memory_learn->leds->show();
  game->state = SIMON_GAME_STATE_PLAY;
}

void simon_game_update_play(MemoryLearn* memory_learn, SimonGame* game, unsigned long delta_time) {
  // Quand c'est au tour de la personne
  game->reaction_time += delta_time;
  unsigned long max_reaction_time = 0xFFFF; //TODO: needs to depends on level
  if (game->reaction_time >= max_reaction_time) {
    tone_buzzer_driver(&memory_learn->buzzer, 220, 250);
    game->state = SIMON_GAME_STATE_GAMEOVER; // Si temps écoulé: Game Over
    return;
  }
  // Si on appuie un bouton
  if (memory_learn->just_pressed_buttons) {
    game->reaction_time = 0;
    uint8_t correct_button = game->buttons[memory_learn->simon_game.button_index];
    for (uint8_t i = 0; i < 8; i++) {
      if (memory_learn->just_pressed_buttons & (1 << i)) {
        const uint8_t* rgb_color = BUTTON_COLORS[correct_button];
        // Si le boutton appuyé et correct.
        if (i == correct_button) {
          tone_buzzer_driver(&memory_learn->buzzer, BUTTON_FREQUENCIES[correct_button], 250);
          memory_learn->leds->setPixelColor(correct_button, rgb_color[0], rgb_color[1], rgb_color[2]);
          memory_learn->leds->show();
          // Si c'est le dernier bouton a appuyé
          if (game->button_index + 1 >= game->level) {
            delay(250);
            memory_learn->leds->setPixelColor(correct_button, 0, 0, 0);
            memory_learn->leds->show();
            delay(250);
            // Passer au niveau suivant
            game->button_index = 0;
            game->level++;
            randomize_buttons(&memory_learn->simon_game);
            game->state = SIMON_GAME_STATE_SHOW;
          } else {
            // Attendre le prochain
            game->button_index++;
          }
        } else {
          tone_buzzer_driver(&memory_learn->buzzer, 220, 250);
          for (uint8_t j = 0; j < 8; j++) {
            memory_learn->leds->setPixelColor(j, 6, 0, 0);
          }
          memory_learn->leds->show();
          // Sinon game over
          memory_learn->just_pressed_buttons = 0;
          game->state = SIMON_GAME_STATE_GAMEOVER;
          break;
        }
      } else {
        memory_learn->leds->setPixelColor(i, 0, 0, 0);
      }
      memory_learn->leds->show();
    }
  }
}

void simon_game_update_gameover(MemoryLearn* memory_learn, SimonGame* game, unsigned long delta_time) {
  // Affiche l'écran de gameover sur l'LCD et attend si le joueur veut rejouer ou non.
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("=-=Game over=-=");
  memory_learn->lcd.setCursor(0, 1);
  memory_learn->lcd.print("    Restart    ");
  if (memory_learn->just_pressed_buttons & BUTTON_OK) {
    memory_learn->lcd.clear();
    game->level = 1;
    game->reaction_time = 0;
    game->button_index = 0;
    randomize_buttons(&memory_learn->simon_game);
    game->state = SIMON_GAME_STATE_SHOW;
  } else if (memory_learn->just_pressed_buttons & BUTTON_NO) {
    memory_learn_set_state(memory_learn, MemoryLearnState::SELECT_GAME);
  } else if (game->reaction_time > 2000) {
    game->button_index++;
    if (game->button_index > 2) {
      game->button_index = 0;
    }
    memory_learn->lcd.setCursor(0, 1);
    if (game->button_index) {
      memory_learn->lcd.print("Score: ");
      memory_learn->lcd.printf("%08d", game->level);
    } else {
      memory_learn->lcd.print("    Restart    ");
    }
    game->reaction_time = 0;
  }
  game->reaction_time += delta_time;
}

// Met le jeu à jour selon l'état du jeu
void simon_game_update(MemoryLearn* memory_learn, unsigned long delta_time) {
  SimonGame* game = &memory_learn->simon_game;
  switch (game->state) {
    case SIMON_GAME_STATE_MENU:
      simon_game_update_menu(memory_learn, game, delta_time);
      break;
    case SIMON_GAME_STATE_SHOW:
      simon_game_update_show(memory_learn, game, delta_time);
      break;
    case SIMON_GAME_STATE_PLAY:
      simon_game_update_play(memory_learn, game, delta_time);
      break;
    case SIMON_GAME_STATE_GAMEOVER:
      simon_game_update_gameover(memory_learn, game, delta_time);
      break;
    default:
      memory_learn_error(memory_learn, "Unexpected state");
      break;
  }
}
