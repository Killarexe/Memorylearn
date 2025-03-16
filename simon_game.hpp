#ifndef SIMON_GAME_HPP
#define SIMON_GAME_HPP

#define SIMON_GAME_STATE_MENU 0
#define SIMON_GAME_STATE_SHOW 1
#define SIMON_GAME_STATE_PLAY 2
#define SIMON_GAME_STATE_GAMEOVER 3

#include "memory_learn.hpp"

void simon_game_init(MemoryLearn* memory_learn);

void simon_game_update(MemoryLearn* memory_learn);

#endif
