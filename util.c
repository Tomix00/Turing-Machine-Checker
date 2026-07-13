#include <string.h>
#include "util.h"

bool in_set(const char *elem, const char *set) {
    char copy[MAX_LINE_LENGTH];
    strcpy(copy, set);
    char *content = copy + 1;
    content[strlen(content) - 1] = '\0';
    char *token = strtok(content, ",");
    while (token != NULL) {
        if (strcmp(token, elem) == 0) {
            return true;
        }
        token = strtok(NULL, ",");
    }
    return false;
}
