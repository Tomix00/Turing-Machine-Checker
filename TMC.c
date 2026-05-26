#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

#define MAX_LINE_LENGTH 100

bool check_q(char Q[]){
    // Check if Q is in the correct format
    regex_t regex;

    const char *patron =
    "^\\{(q[a-zA-Z0-9]+(,q[a-zA-Z0-9]+)*)?\\}$";

    if (regcomp(&regex, patron, REG_EXTENDED)){
        return false;
    }

    bool valid = !regexec(&regex, Q, 0, NULL, 0);
    regfree(&regex);

    if (!valid){
        printf("\nError: Wrong format for Q\n");
        printf("Q: %s\n\n", Q);
        return false;
    }

    char copy[MAX_LINE_LENGTH];
    strcpy(copy, Q);

    // Remove '{'
    char *content = copy + 1;

    // Remove '}'
    content[strlen(content) - 1] = '\0';

    char *tokens[100];
    int token_count = 0;

    // Split by commas
    char *token = strtok(content, ",");

    while (token != NULL){
        tokens[token_count++] = token;
        token = strtok(NULL, ",");
    }

    // Compare tokens
    for (int i = 0; i < token_count; i++){

        for (int j = i + 1; j < token_count; j++){

            if (strcmp(tokens[i], tokens[j]) == 0){

                printf("\nError: Duplicate state %s\n", tokens[i]);
                printf("Q: %s\n\n", Q);

                return false;
            }
        }
    }

    return true;
}

bool check_sigma(char Sigma[]){
    regex_t regex;

    const char *patron =
    "^\\{([^,{} ]+(,[^,{} ]+)*)?\\}$";

    if (regcomp(&regex, patron, REG_EXTENDED)){
        return false;
    }

    bool valid = !regexec(&regex, Sigma, 0, NULL, 0);
    regfree(&regex);

    if (!valid){
        printf("\nError: Wrong format for Sigma\n");
        printf("Sigma: %s\n\n", Sigma);
        return false;
    }

    char copy[MAX_LINE_LENGTH];
    strcpy(copy, Sigma);

    // Skip '{'
    char *content = copy + 1;

    // Remove '}'
    content[strlen(content) - 1] = '\0';

    // Split tokens
    char *tokens[MAX_LINE_LENGTH];
    int token_count = 0;

    char *token = strtok(content, ",");

    while (token != NULL){
        tokens[token_count++] = token;
        token = strtok(NULL, ",");
    }

    // Check duplicates
    for (int i = 0; i < token_count; i++){
        for (int j = i + 1; j < token_count; j++){

            if (strcmp(tokens[i], tokens[j]) == 0){

                printf("\nError: Duplicate symbol %s in Sigma\n", tokens[i]);
                printf("Sigma: %s\n\n", Sigma);

                return false;
            }
        }
    }
    return true;
}

int check_format_machine(char* file1){
    FILE* fp = fopen(file1, "r");
    if (fp == NULL){
        // Check if file exists
        printf("Error: File %s not found.", file1);
        return -1;
    }

    for (unsigned int i = 0; i < 7 ; i++){
        // Read line by line and check the format of each line
        char line[MAX_LINE_LENGTH];
        if (fgets(line, MAX_LINE_LENGTH, fp) == NULL){
            // Check if file is empty or has less than 6 lines
            printf("Error: File %s is empty.\n", file1);
            fclose(fp);
            return -1;
        }

        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        switch (i){
            case 0:
                // check Q
                if (!check_q(line)){
                    fclose(fp);
                    return -1;
                }
                printf("Q format is correct\n");
                break;
            case 1:
                // check q0
                break;
            case 2:
                // check F
                break;
            case 3:
                // Check Sigma
                if (!check_sigma(line)){
                    fclose(fp);
                    return -1;
                }
                printf("Sigma format is correct\n");
                break;
            case 4:
                //Check Gamma
                break;
            case 5:
                //Check Blank symbol
                break;
            default:
                //Check rules
                break;
        }

    }
    fclose(fp);
    return 0;
}

int check_format_input(char* file2){
    // Check if file exists
    FILE* fp = fopen(file2, "r");
    if (fp == NULL){
        printf("Error: File %s not found.", file2);
        return -1;
    }

    // Check format of file
    // ...

    fclose(fp);
    return 0;
}

int main(int argc, char* argv[]){
    // Check usage
    if (argc != 3 || argc > 3){
        printf("Usage: ./TMC machine.in input.in\n");
        exit(1);
    }

    char* machine = argv[1];
    char* input = argv[2];
    // Check correct format
    if (check_format_machine(machine) == -1){
        printf("Error: File %s has incorrect format.\n\n", machine);
        exit(1);
    }
    if (check_format_input(input) == -1){
        printf("Error: File %s has incorrect format.\n\n", input);
        exit(1);
    }

    return 0;
}