#ifndef MACHINE_H
#define MACHINE_H

#include "util.h"

typedef struct {
    char current_state[MAX_LINE_LENGTH];
    char current_symbol[MAX_LINE_LENGTH];
    char new_state[MAX_LINE_LENGTH];
    char new_symbol[MAX_LINE_LENGTH];
    char direction;
} Rule;

typedef struct {
    char Q[MAX_LINE_LENGTH];
    char q0[MAX_LINE_LENGTH];
    char F[MAX_LINE_LENGTH];
    char Sigma[MAX_LINE_LENGTH];
    char Gamma[MAX_LINE_LENGTH];
    char blank[MAX_LINE_LENGTH];
    Rule rules[MAX_RULES];
    int rule_count;
} TMDefinition;

int parse_machine_file(const char *filename, TMDefinition *tm);

#endif
