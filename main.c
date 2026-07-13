#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "input_handler.h"
#include "util.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./TMC machine.in input.in\n");
        return 1;
    }
    char *machine = argv[1];
    char *input = argv[2];
    char Sigma[MAX_LINE_LENGTH];
    if (check_format_machine(machine, Sigma) == -1) {
        printf("Error: File %s has incorrect format.\n\n", machine);
        return 1;
    }
    if (check_format_input(input, Sigma) == -1) {
        printf("Error: File %s has incorrect format.\n\n", input);
        return 1;
    }
    return 0;
}
