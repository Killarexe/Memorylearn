#include "memory_learn.hpp"

int main(void) {
  MemoryLearn memory_learn;
  init_memory_learn(&memory_learn);
  
  while (1) {
    update_memory_learn(&memory_learn);
  }
}
