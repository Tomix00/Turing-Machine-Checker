#ifndef TM_H
#define TM_H

#include "machine.h"

#define MAX_TAPE_SIZE 2000
#define MAX_STEPS 2000

void run_simulation(TMDefinition *tm, const char *input, const char *stop_state);

#endif
