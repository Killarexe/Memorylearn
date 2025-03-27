/**
 * simon_game.hpp
 * Definie les fonctions pour le jeu du simon.
 */
#ifndef SIMON_GAME_HPP
#define SIMON_GAME_HPP

// Etats possible du jeux
#define SIMON_GAME_STATE_MENU 0
#define SIMON_GAME_STATE_SHOW 1
#define SIMON_GAME_STATE_PLAY 2
#define SIMON_GAME_STATE_GAMEOVER 3

#include "memory_learn.hpp"

/**
 * Initialize le jeu Simon
 * @param MemoryLearn* memory_learn
 */
void simon_game_init(MemoryLearn* memory_learn);

/**
 * Met à jour le jeu Simon
 * @param MemoryLearn* memory_learn
 * @param int64_t delta_time Temps passée avant la dernière appel en ms
 */
void simon_game_update(MemoryLearn* memory_learn, int64_t delta_time);

#endif
