#include <stdio.h>
#include <string.h>
#include "input_handler.h"
#include "util.h"

int check_format_input(const char *filename, const char *Gamma, char *input_out) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: File %s not found.", filename);
        return -1;
    }
    char line[MAX_LINE_LENGTH];
    if (fgets(line, MAX_LINE_LENGTH, fp) == NULL) {
        printf("Error: File %s is empty.\n", filename);
        fclose(fp);
        return -1;
    }
    line[strcspn(line, "\n")] = '\0';
    for (int i = 0; line[i] != '\0'; i++) {
        char sym[2] = {line[i], '\0'};
        if (!in_set(sym, Gamma)) {
            printf("\nError: Symbol '%c' in input is not in Gamma\n", line[i]);
            printf("Gamma: %s\n\n", Gamma);
            fclose(fp);
            return -1;
        }
    }
    strcpy(input_out, line);
    fclose(fp);
    return 0;
}
