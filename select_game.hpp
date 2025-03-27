/**
 * select_game.hpp
 * Definie les fonctions pour le menu de selection des jeux
 */
#ifndef SELECT_GAME_HPP
#define SELECT_GAME_HPP

#include "memory_learn.hpp"

/**
 * Initalize le menu de selection des jeux
 * @param MemoryLearn* memory_learn
 */
void select_game_init(MemoryLearn* memory_learn);

/**
 * Met à jour le menu de selection des jeux
 * @param MemoryLearn* memory_learn
 */
void select_game_update(MemoryLearn* memory_learn);

#endif
