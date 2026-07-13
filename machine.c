#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "machine.h"
#include "util.h"

static void dump(char array[]) {
    printf("%s\n", array);
}

static bool check_q(char Q[], char load[]) {
    regex_t regex;
    const char *patron = "^\\{(q[a-zA-Z0-9]+(,q[a-zA-Z0-9]+)*)?\\}$";
    if (regcomp(&regex, patron, REG_EXTENDED)) return false;
    bool valid = !regexec(&regex, Q, 0, NULL, 0);
    regfree(&regex);
    if (!valid) {
        printf("\nError: Wrong format for Q\n");
        printf("Q: %s\n\n", Q);
        return false;
    }
    char copy[MAX_LINE_LENGTH];
    strcpy(copy, Q);
    char *content = copy + 1;
    content[strlen(content) - 1] = '\0';
    char *tokens[MAX_TOKENS];
    int token_count = 0;
    char *token = strtok(content, ",");
    while (token != NULL) {
        tokens[token_count++] = token;
        token = strtok(NULL, ",");
    }
    for (int i = 0; i < token_count; i++) {
        for (int j = i + 1; j < token_count; j++) {
            if (strcmp(tokens[i], tokens[j]) == 0) {
                printf("\nError: Duplicate state %s\n", tokens[i]);
                printf("Q: %s\n\n", Q);
                return false;
            }
        }
    }
    strcpy(load, Q);
    return true;
}

static bool check_initial_state(char q0[], char Q[]) {
    if (in_set(q0, Q)) return true;
    printf("\nError: initial state %s is not in Q\n", q0);
    printf("Q: %s\n\n", Q);
    return false;
}

static bool check_f(char F[], char Q[], char load[]) {
    regex_t regex;
    const char *patron = "^\\{(q[a-zA-Z0-9]+(,q[a-zA-Z0-9]+)*)?\\}$";
    if (regcomp(&regex, patron, REG_EXTENDED)) return false;
    bool valid = !regexec(&regex, F, 0, NULL, 0);
    regfree(&regex);
    if (!valid) {
        printf("\nError: Wrong format for F\n");
        printf("F: %s\n\n", F);
        return false;
    }
    char copy[MAX_LINE_LENGTH];
    strcpy(copy, F);
    char *content = copy + 1;
    content[strlen(content) - 1] = '\0';
    char *tokens[MAX_TOKENS];
    int token_count = 0;
    char *token = strtok(content, ",");
    while (token != NULL) {
        tokens[token_count++] = token;
        token = strtok(NULL, ",");
    }
    for (int i = 0; i < token_count; i++) {
        if (!in_set(tokens[i], Q)) {
            printf("\nError: state %s in F is not in Q\n", tokens[i]);
            printf("F: %s\n", F);
            printf("Q: %s\n\n", Q);
            return false;
        }
    }
    strcpy(load, F);
    return true;
}

static bool check_sigma(char Sigma[], char load[]) {
    regex_t regex;
    const char *patron = "^\\{([^,{} ]+(,[^,{} ]+)*)?\\}$";
    if (regcomp(&regex, patron, REG_EXTENDED)) return false;
    bool valid = !regexec(&regex, Sigma, 0, NULL, 0);
    regfree(&regex);
    if (!valid) {
        printf("\nError: Wrong format for Sigma\n");
        printf("Sigma: %s\n\n", Sigma);
        return false;
    }
    char copy[MAX_LINE_LENGTH];
    strcpy(copy, Sigma);
    char *content = copy + 1;
    content[strlen(content) - 1] = '\0';
    char *tokens[MAX_TOKENS];
    int token_count = 0;
    char *token = strtok(content, ",");
    while (token != NULL) {
        tokens[token_count++] = token;
        token = strtok(NULL, ",");
    }
    for (int i = 0; i < token_count; i++) {
        for (int j = i + 1; j < token_count; j++) {
            if (strcmp(tokens[i], tokens[j]) == 0) {
                printf("\nError: Duplicate symbol %s in Sigma\n", tokens[i]);
                printf("Sigma: %s\n\n", Sigma);
                return false;
            }
        }
    }
    strcpy(load, Sigma);
    return true;
}

static bool check_gamma(char Gamma[], char Sigma[], char load[]) {
    regex_t regex;
    const char *patron = "^\\{([^,{} ]+(,[^,{} ]+)*)?\\}$";
    if (regcomp(&regex, patron, REG_EXTENDED)) return false;
    bool valid = !regexec(&regex, Gamma, 0, NULL, 0);
    regfree(&regex);
    if (!valid) {
        printf("\nError: Wrong format for Gamma\n");
        printf("Gamma: %s\n\n", Gamma);
        return false;
    }
    char copy_sigma[MAX_LINE_LENGTH];
    strcpy(copy_sigma, Sigma);
    char *content_sigma = copy_sigma + 1;
    content_sigma[strlen(content_sigma) - 1] = '\0';
    char *token_sigma = strtok(content_sigma, ",");
    while (token_sigma != NULL) {
        if (!in_set(token_sigma, Gamma)) {
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

static bool check_blank_symbol(char blank_symbol[], char Gamma[]) {
    if (in_set(blank_symbol, Gamma)) return true;
    printf("\nError: Blank symbol %s is not in Gamma\n", blank_symbol);
    printf("Gamma: %s\n\n", Gamma);
    return false;
}

static bool check_rule(char rule[], char Q[], char Gamma[], Rule *parsed) {
    regex_t regex;
    const char *patron = "^\\((q[a-zA-Z0-9]+),([^,{} ]+)\\)=\\((q[a-zA-Z0-9]+),([^,{} ]+),(L|R|K)\\)$";
    if (regcomp(&regex, patron, REG_EXTENDED)) return false;
    bool valid = !regexec(&regex, rule, 0, NULL, 0);
    regfree(&regex);
    if (!valid) {
        printf("\nError: Wrong format for rules\n");
        printf("Rule: %s\n\n", rule);
        return false;
    }
    char current_state[MAX_LINE_LENGTH];
    char current_symbol[MAX_LINE_LENGTH];
    char new_state[MAX_LINE_LENGTH];
    char new_symbol[MAX_LINE_LENGTH];
    char direction[MAX_LINE_LENGTH];
    sscanf(rule, "(%[^,],%[^)])=(%[^,],%[^,],%s)",
           current_state, current_symbol, new_state, new_symbol, direction);
    if (!in_set(current_state, Q)) {
        printf("\nError: State %s in rule is not in Q\n", current_state);
        printf("Rule: %s\n\n", rule);
        return false;
    }
    if (!in_set(current_symbol, Gamma)) {
        printf("\nError: Symbol %s in rule is not in Gamma\n", current_symbol);
        printf("Rule: %s\n\n", rule);
        return false;
    }
    if (!in_set(new_state, Q)) {
        printf("\nError: State %s in rule is not in Q\n", new_state);
        printf("Rule: %s\n\n", rule);
        return false;
    }
    if (!in_set(new_symbol, Gamma)) {
        printf("\nError: Symbol %s in rule is not in Gamma\n", new_symbol);
        printf("Rule: %s\n\n", rule);
        return false;
    }
    if (parsed != NULL) {
        strcpy(parsed->current_state, current_state);
        strcpy(parsed->current_symbol, current_symbol);
        strcpy(parsed->new_state, new_state);
        strcpy(parsed->new_symbol, new_symbol);
        parsed->direction = direction[0];
    }
    return true;
}

int parse_machine_file(const char *filename, TMDefinition *tm) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: File %s not found.", filename);
        return -1;
    }
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    tm->rule_count = 0;
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        switch (line_count) {
            case 0:
                if (!check_q(line, tm->Q)) { fclose(fp); return -1; }
                printf("\nQ format is correct\n");
                dump(tm->Q);
                printf("-------------------------------\n");
                break;
            case 1:
                if (!check_initial_state(line, tm->Q)) { fclose(fp); return -1; }
                strcpy(tm->q0, line);
                printf("\nInitial state format is correct\n");
                printf("-------------------------------\n");
                break;
            case 2:
                if (!check_f(line, tm->Q, tm->F)) { fclose(fp); return -1; }
                printf("\nF format is correct\n");
                dump(tm->F);
                printf("-------------------------------\n");
                break;
            case 3:
                if (!check_sigma(line, tm->Sigma)) { fclose(fp); return -1; }
                printf("\nSigma format is correct\n");
                dump(tm->Sigma);
                printf("-------------------------------\n");
                break;
            case 4:
                if (!check_gamma(line, tm->Sigma, tm->Gamma)) { fclose(fp); return -1; }
                printf("\nGamma format is correct\n");
                dump(tm->Gamma);
                printf("-------------------------------\n");
                break;
            case 5:
                if (!check_blank_symbol(line, tm->Gamma)) { fclose(fp); return -1; }
                strcpy(tm->blank, line);
                printf("\nBlank symbol format is correct\n");
                printf("-------------------------------\n");
                break;
            default:
                if (!check_rule(line, tm->Q, tm->Gamma, &tm->rules[tm->rule_count])) {
                    fclose(fp); return -1;
                }
                tm->rule_count++;
                break;
        }
        line_count++;
    }
    if (line_count == 0) {
        printf("Error: File %s is empty.\n", filename);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}
