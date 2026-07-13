#include <stdio.h>
#include <string.h>
#include "input_handler.h"
#include "util.h"

int check_format_input(const char *filename, const char *Sigma) {
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
        if (!in_set(sym, Sigma)) {
            printf("\nError: Symbol '%c' in input is not in Sigma\n", line[i]);
            printf("Sigma: %s\n\n", Sigma);
            fclose(fp);
            return -1;
        }
    }
    fclose(fp);
    return 0;
}
