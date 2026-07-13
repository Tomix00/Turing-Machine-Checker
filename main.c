#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "machine.h"
#include "input_handler.h"
#include "tm.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./TMC machine.in input.in\n");
        return 1;
    }

    TMDefinition tm;
    if (parse_machine_file(argv[1], &tm) == -1) {
        printf("Error: File %s has incorrect format.\n\n", argv[1]);
        return 1;
    }

    char input[MAX_LINE_LENGTH];
    if (check_format_input(argv[2], tm.Sigma, input) == -1) {
        printf("Error: File %s has incorrect format.\n\n", argv[2]);
        return 1;
    }

    printf("\nDo you want to stop at a specific state? (s/n): ");
    char response[16];
    if (fgets(response, sizeof(response), stdin) == NULL) {
        return 1;
    }
    response[strcspn(response, "\n")] = '\0';

    char stop_state[MAX_LINE_LENGTH] = "";
    if (tolower((unsigned char)response[0]) == 's') {
        printf("\nAvailable states: %s\n", tm.Q);
        printf("Enter stop state: ");
        if (fgets(stop_state, sizeof(stop_state), stdin) == NULL) {
            return 1;
        }
        stop_state[strcspn(stop_state, "\n")] = '\0';
        if (!in_set(stop_state, tm.Q)) {
            printf("Error: State %s is not in Q\n", stop_state);
            return 1;
        }
        run_simulation(&tm, input, stop_state);
    } else {
        run_simulation(&tm, input, NULL);
    }

    return 0;
}
