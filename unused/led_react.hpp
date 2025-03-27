/* led_react.hpp
 * Definie les fonctions pour jeu LED React.
 */
#ifndef LED_REACT_HPP
#define LED_REACT_HPP

#include "memory_learn.hpp"

// Etats possible du jeux
#define LED_REACT_STATE_MENU 0
#define LED_REACT_STATE_PLAY 1
#define LED_REACT_STATE_GAMEOVER 2

/**
 * Initalize le jeu LED React.
 * @param MemoryLearn* memory_learn
 */
void led_react_init(MemoryLearn* memory_learn);

/**
 * Met à jour le jeu LED React.
 * @param MemoryLearn* memory_learn
 */
void led_react_update(MemoryLearn* memory_learn, int64_t delta_time);

#endif
