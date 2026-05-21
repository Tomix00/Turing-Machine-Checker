#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

bool check_sigma(char Sigma[]){
    // Check if Sigma is correct
    unsigned int len = strlen(Sigma);
    if (len == 1){
        // the line is empty
        printf("Error 1: Wrong format for Sigma\n");
        return false;
    }else if (len == 2 && Sigma[0] != '{'){
        // the fisrt simbol is not {
        printf("Error 2: Wrong format for Sigma\n");
        printf("Sigma: %s\n", Sigma);
        return false;
    }else if (len > 1 && Sigma[0] == '{'){
        // the first simbol is { and there are more simbols
        if(Sigma[len - 2] != '}'){
            // does not end with }
            printf("Error 3: Wrong format for Sigma\n");
            printf("Sigma: %s\n", Sigma);
            return false;
        }else if (Sigma[1] == '}' && Sigma[2] != '\n'){
            // is empty set but has extra characters
            printf("Error 4: Wrong format for Sigma\n");
            printf("Sigma: %s\n", Sigma);
            return false;
        }else if(Sigma[1] == '}' && Sigma[2] == '\n'){
            // is empty set and has correct format
            printf("Sigma is an empty set\n");
            printf("Sigma: %s\n", Sigma);
            return true;
        }else{
            // at this case, we know that
            // Sigma[0] == '{' and Sigma[len - 2] == '}',
            // so we can check the symbols in between
            for (unsigned int i = 1; i < len - 2; i++){
                if (Sigma[i] == ' ' || Sigma[i] == '{' || Sigma[i] == '}'){
                    // if there is a space or a new line in the middle of the symbols, it is an error
                    printf("Error 5: Wrong format for Sigma\n");
                    printf("Sigma: %s\n", Sigma);
                    return false;
                }else if (Sigma[i] == ',' && (Sigma[i - 1] == '{' || Sigma[i - 1] == ',' || Sigma[i + 1] == '}' || Sigma[i + 1] == ',')){
                    // if there is no symbols between the { and }, it is an error
                    printf("Error 6: Wrong format for Sigma\n");
                    printf("Sigma: %s\n", Sigma);
                    return false;
                }
            }
        }
    }

    // The format of Sigma is correct, now we can check for duplicate symbols
    for (unsigned int i = 1; i < len - 2; i++){
        if (Sigma[i] != ',' && Sigma[i] != ' ' && Sigma[i] != '{' && Sigma[i] != '}'){
            for (unsigned int j = i + 1; j < len - 2; j++){
                if (Sigma[i] == Sigma[j]){
                    printf("Error 7: Duplicate symbol %c in Sigma\n", Sigma[i]);
                    printf("Sigma: %s\n", Sigma);
                    return false;
                }
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

        switch (i){
            case 0:
                // check Q
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
        printf("Error: File %s has incorrect format.\n", machine);
        exit(1);
    }
    if (check_format_input(input) == -1){
        printf("Error: File %s has incorrect format.\n", input);
        exit(1);
    }

    return 0;
}