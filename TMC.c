#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine.h"
#include "input_handler.h"

#define MAX_LINE_LENGTH 100

int main(int argc, char* argv[]){
    if (argc != 3 || argc > 3){
        printf("Usage: ./TMC machine.in input.in\n");
        exit(1);
    }
    char* machine = argv[1];
    char* input = argv[2];
    char Sigma[MAX_LINE_LENGTH];
    if (check_format_machine(machine, Sigma) == -1){
        printf("Error: File %s has incorrect format.\n\n", machine);
        exit(1);
    }
    if (check_format_input(input, Sigma) == -1){
        printf("Error: File %s has incorrect format.\n\n", input);
        exit(1);
    }
    return 0;
}
