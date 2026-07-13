#include <stdio.h>
#include <string.h>
#include "tm.h"
#include "util.h"

static void print_id(const char *tape, int head, int rightmost, const char *state) {
    for (int i = 0; i < head; i++) {
        putchar(tape[i]);
    }
    printf(" %s ", state);
    for (int i = head; i <= rightmost; i++) {
        putchar(tape[i]);
    }
    putchar('\n');
}

void run_simulation(TMDefinition *tm, const char *input, const char *stop_state) {
    char tape[MAX_TAPE_SIZE];
    memset(tape, tm->blank[0], MAX_TAPE_SIZE);

    int len = strlen(input);
    int head = 0;
    int rightmost = 0;

    if (len > 0) {
        memcpy(tape, input, len);
        rightmost = len - 1;
    }

    char current_state[MAX_LINE_LENGTH];
    strcpy(current_state, tm->q0);

    printf("\n--- Starting simulation ---\n");

    int step;
    for (step = 0; step < MAX_STEPS; step++) {
        print_id(tape, head, rightmost, current_state);

        if (stop_state != NULL && strcmp(current_state, stop_state) == 0) {
            printf("\nReached stop state %s\n", stop_state);
            return;
        }
        if (stop_state == NULL && in_set(current_state, tm->F)) {
            printf("\nReached final state\n");
            return;
        }

        char sym_str[2] = {tape[head], '\0'};
        Rule *matched = NULL;
        for (int i = 0; i < tm->rule_count; i++) {
            if (strcmp(tm->rules[i].current_state, current_state) == 0 &&
                strcmp(tm->rules[i].current_symbol, sym_str) == 0) {
                matched = &tm->rules[i];
                break;
            }
        }

        if (matched == NULL) {
            printf("\nNo matching rule - machine halted\n");
            return;
        }

        tape[head] = matched->new_symbol[0];
        strcpy(current_state, matched->new_state);

        if (matched->direction == 'R') {
            head++;
            if (head >= MAX_TAPE_SIZE) {
                printf("\nTape overflow\n");
                return;
            }
            if (head > rightmost) {
                rightmost = head;
            }
        } else if (matched->direction == 'L') {
            if (head > 0) {
                head--;
            }
        }
    }

    if (step >= MAX_STEPS) {
        printf("\nReached step limit (%d) - possible infinite loop\n", MAX_STEPS);
    }
}
