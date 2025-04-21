/* about.hpp
 * Definiton des fonctions pour le menu à propos.
 */
#ifndef ABOUT_HPP
#define ABOUT_HPP

#define ABOUT_MENU_WAIT_TIME 5000

#include "memory_learn.hpp"

/**
 * Initialize le menu à propos.
 * @param MemoryLearn* memory_learn
 */
void about_init(MemoryLearn* memory_learn);

/**
 * Met à jour le menu à propos.
 * @param MemoryLearn* memory_learn
 */
void about_update(MemoryLearn* memory_learn, unsigned long delta_time);

#endif
