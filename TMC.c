#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

#define MAX_LINE_LENGTH 100
#define MAX_RULES 50

//auxiliary function to print arrays, will be deleted
void dump(char array[]){
    printf("%s\n", array);
}

#pragma region Auxiliary functions

bool symbol_in_alphabet(const char *symbol, const char *alphabet){
    // Validate if current symbol is in alphabet
    char copy[MAX_LINE_LENGTH];
    char *token;

    // copy and clean the alphabet
    strcpy(copy, alphabet);
    //remove '{'
    char *content = copy + 1;
    //remove '}'
    content[strlen(content) - 1] = '\0';

    // Split by commas
    token = strtok(content, ",");

    // Compare tokens with symbol
    while (token != NULL){
        if (strcmp(token, symbol) == 0){
            return true;
        }
        token = strtok(NULL, ",");
    }
    
    printf("Error: Symbol %s is not in alphabet %s\n",symbol, alphabet);
    return false;
}

bool state_in_set(const char *state, const char *states_set){
    // Validate if current state is in Q
    char copy[MAX_LINE_LENGTH];
    char *token;

    // Copy and clean states set
    strcpy(copy, states_set);
    copy[strlen(copy) - 1] = '\0';
    memmove(copy, copy + 1, strlen(copy));

    // Split by commas
    token = strtok(copy, ",");

    // Compare tokens with state
    while(token != NULL){
        if (strcmp(token, state) == 0){
            return true;
        }
        token = strtok(NULL, ",");
    }
    printf("Error: State %s is not in Set: %s\n",state, states_set);
    return false;
}

#pragma endregion

#pragma region Validation functions

bool check_q(char Q[], char load[]){
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

    strcpy(load, Q);
    return true;
}

bool check_initial_state(char q0[], char Q[]){
    // Check if q0 is inside Q
    char copy[MAX_LINE_LENGTH];

    strcpy(copy, Q);
    
    // Remove '{'
    char *content = copy + 1;
    
    // Remove '}'
    content[strlen(content) - 1] = '\0';

    char *token = strtok(content, ",");
    while (token != NULL){
        if (strcmp(token, q0) == 0){
            return true;
        }
        token = strtok(NULL, ",");
    }
    printf("\nError: initial state%s is not in Q\n", q0);
    printf("Q: %s\n\n", Q);

    return false;
}

bool check_f(char F[], char Q[], char load[]){
    // Check if F is in the correct format
    // Check if F is contained in Q
    regex_t regex;

    const char *patron =
    "^\\{(q[a-zA-Z0-9]+(,q[a-zA-Z0-9]+)*)?\\}$";

    if (regcomp(&regex, patron, REG_EXTENDED)){
        return false;
    }

    bool valid = !regexec(&regex, F, 0, NULL, 0);

    regfree(&regex);

    if (!valid){
        printf("\nError: Wrong format for F\n");
        printf("F: %s\n\n", F);
        return false;
    }
    
    char copy[MAX_LINE_LENGTH];
    strcpy(copy, F);
    
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
    
    // Check if each token is in Q
    for (int i = 0; i < token_count; i++){
        bool found = false;
        
        char copy_q[MAX_LINE_LENGTH];
        strcpy(copy_q, Q);
        
        // Remove '{'
        char *content_q = copy_q + 1;
        
        // Remove '}'
        content_q[strlen(content_q) - 1] = '\0';
        
        char *token_q = strtok(content_q, ",");
        while (token_q != NULL){
            if (strcmp(tokens[i], token_q) == 0){
                found = true;
                break;
            }
            token_q = strtok(NULL, ",");
        }
        if (!found){
            printf("\nError: state %s in F is not in Q\n", tokens[i]);
            printf("F: %s\n", F);
            printf("Q: %s\n\n", Q);
            return false;
        }
    }
    strcpy(load, F);

    return true;
}

bool check_sigma(char Sigma[], char load[]){
    // Check if Sigma is in the correct format
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
    
    strcpy(load, Sigma);
    return true;
}

bool check_gamma(char Gamma[], char Sigma[], char load[]){
    // Check if Gamma is in the correct format
    // Check if Gamma contains all symbols of Sigma
    regex_t regex;

    const char *patron =
    "^\\{([^,{} ]+(,[^,{} ]+)*)?\\}$";

    if (regcomp(&regex, patron, REG_EXTENDED)){
        return false;
    }

    bool valid = !regexec(&regex, Gamma, 0, NULL, 0);
    regfree(&regex);

    if (!valid){
        printf("\nError: Wrong format for Gamma\n");
        printf("Gamma: %s\n\n", Gamma);
        return false;
    }

    // Check if Gamma contains all symbols of Sigma
    char copy_gamma[MAX_LINE_LENGTH];
    strcpy(copy_gamma, Gamma);

    // Skip '{'
    char *content_gamma = copy_gamma + 1;
    
    // Remove '}'
    content_gamma[strlen(content_gamma) - 1] = '\0';
    
    char *tokens_gamma[MAX_LINE_LENGTH];
    int token_count_gamma = 0;
    char *token_gamma = strtok(content_gamma, ",");
    
    while (token_gamma != NULL){
        tokens_gamma[token_count_gamma++] = token_gamma;
        token_gamma = strtok(NULL, ",");
    }

    char copy_sigma[MAX_LINE_LENGTH];
    strcpy(copy_sigma, Sigma);
    
    // Skip '{'
    char *content_sigma = copy_sigma + 1;
    
    // Remove '}'
    content_sigma[strlen(content_sigma) - 1] = '\0';
    
    char *token_sigma = strtok(content_sigma, ",");
    while (token_sigma != NULL){
        bool found = false;
        for (int i = 0; i < token_count_gamma; i++){
            if (strcmp(token_sigma, tokens_gamma[i]) == 0){
                found = true;
                break;
            }
        }
        if (!found){
            printf("\nError: Symbol %s in Sigma is not in Gamma\n", token_sigma);
            printf("Sigma: %s\n", Sigma);
            printf("Gamma: %s\n\n", Gamma);
            return false;
        }
        token_sigma = strtok(NULL, ",");
    }

    strcpy(load, Gamma);
    return true;
}

bool check_blank_symbol(char blank_symbol[], char Gamma[]){
    // Check if blank symbol is in Gamma
    
    char copy_gamma[MAX_LINE_LENGTH];        
    strcpy(copy_gamma, Gamma);

    // Skip '{'
    char *content_gamma = copy_gamma + 1;

    // Remove '}'
    content_gamma[strlen(content_gamma) - 1] = '\0';
    
    char *token_gamma = strtok(content_gamma, ",");
    while (token_gamma != NULL){
        if (strcmp(token_gamma, blank_symbol) == 0){
            return true;
        }
        token_gamma = strtok(NULL, ",");
    }

    printf("\nError: Blank symbol %s is not in Gamma\n", blank_symbol);
    printf("Gamma: %s\n\n", Gamma);

    return false;
}

bool check_rule(char rule[], char Q[], char Gamma[], char load_rules[MAX_RULES][MAX_LINE_LENGTH], int rule_count){
    // Check if rules are in the correct format
    // Check if rules are valid according to Q, Sigma and Gamma
    regex_t regex;

    const char *patron = 
    "^\\((q[a-zA-Z0-9]+),([^,{} ]+)\\)=\\((q[a-zA-Z0-9]+),([^,{} ]+),(L|R|K)\\)$";
    
    if (regcomp(&regex, patron, REG_EXTENDED)){
        return false;
    }

    bool valid = !regexec(&regex, rule, 0, NULL, 0);
    regfree(&regex);

    if (!valid){
        printf("\nError: Wrong format for rules\n");
        printf("Rule: %s\n\n",rule);
        return false;
    }

    //check if rule has valid arguments
    // Extract components of the rule
    char current_state[MAX_LINE_LENGTH];
    char current_symbol[MAX_LINE_LENGTH];
    char new_state[MAX_LINE_LENGTH];
    char new_symbol[MAX_LINE_LENGTH];
    char direction[MAX_LINE_LENGTH];
    sscanf(rule, "(%[^,],%[^)])=(%[^,],%[^,],%s)",current_state,
                                                  current_symbol,
                                                  new_state,
                                                  new_symbol,
                                                  direction);
    
    bool valid_args;
    valid_args = state_in_set(current_state, Q) &&
                 state_in_set(new_state, Q) &&
                 symbol_in_alphabet(current_symbol, Gamma) &&
                 symbol_in_alphabet(new_symbol, Gamma);
    
    if (!valid_args){
        printf("Error ↑: Inconsistent arguments in rule\n");
        return false;
    }

    // Check if is reapeated rule
    for (int i = 0; i < rule_count; i++){
        if (strcmp(load_rules[i], rule) == 0){
            printf("\nError: Duplicate rule \"%s\" on line %d and %d\n", rule, 7 + i, 7 + rule_count);
            return false;
        }
    }
    
    // If all checks passed, load the rule
    strcpy(load_rules[rule_count], rule);

    return true;
}

#pragma endregion

#pragma region Check Format

int check_format_machine(char* file1){
    FILE* fp = fopen(file1, "r");
    if (fp == NULL){
        // Check if file exists
        printf("Error: File %s not found.", file1);
        return -1;
    }

    // buffer to store a line read
    char line[MAX_LINE_LENGTH];
    int line_count = 0;

    // Variables to store the components of the Turing machine
    char q[MAX_LINE_LENGTH];
    char f[MAX_LINE_LENGTH];
    char sigma[MAX_LINE_LENGTH];
    char gamma[MAX_LINE_LENGTH];
    char rules[MAX_RULES][MAX_LINE_LENGTH];
    int rule_count = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL){
        // Read line by line and check the format of each line

        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        switch (line_count){
            case 0:
                // check Q
                if (!check_q(line, q)){
                    fclose(fp);
                    return -1;
                }
                printf("\nQ format is correct\n");
                dump(q);
                printf("-------------------------------\n");
                break;
            case 1:
                // check initial state
                if (!check_initial_state(line, q)){
                    fclose(fp);
                    return -1;
                }
                printf("\nInitial state format is correct\n");
                printf("-------------------------------\n");
                break;
            case 2:
                // check F
                if (!check_f(line, q, f)){
                    fclose(fp);
                    return -1;
                }
                printf("\nF format is correct\n");
                dump(f);
                printf("-------------------------------\n");
                break;
            case 3:
                // Check Sigma
                if (!check_sigma(line, sigma)){
                    fclose(fp);
                    return -1;
                }
                printf("\nSigma format is correct\n");
                dump(sigma);
                printf("-------------------------------\n");
                break;
            case 4:
                //Check Gamma
                if (!check_gamma(line, sigma, gamma)){
                    fclose(fp);
                    return -1;
                }
                printf("\nGamma format is correct\n");
                dump(gamma);
                printf("-------------------------------\n");
                break;
            case 5:
                //Check Blank symbol
                if (!check_blank_symbol(line, gamma)){
                    fclose(fp);
                    return -1;
                }
                printf("\nBlank symbol format is correct\n");
                printf("-------------------------------\n");
                break;
            default:
                //Check rules
                if (!check_rule(line, q, gamma, rules, rule_count)){
                    fclose(fp);
                    return -1;
                }else{
                    rule_count++;
                }
                break;
        }
        line_count++;
    }

    if (line_count == 0){
        printf("Error: File %s is empty.\n", file1);
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

int check_format_input(char* file2, char Sigma[]){
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

#pragma endregion

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
        printf("Error ↑: File %s has incorrect format.\n\n", machine);
        exit(1);
    }
    char Sigma[MAX_LINE_LENGTH];
    if (check_format_input(input, Sigma) == -1){
        printf("Error: File %s has incorrect format.\n\n", input);
        exit(1);
    }

    return 0;
}