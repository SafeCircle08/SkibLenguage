#include "include/builtInStringUtils.h"
#include <string.h>

bool endsWithBreakLine(const char* string) {
    size_t strL = strlen(string);
    if (strL > 0 && string[strL - 1] == 'n') {
        if (string[strL - 2] == '\\') {
            return true;
        }
    }
    return false;
}

char* terminateStringBeforeBr(char* string) {
    size_t len = strlen(string);
    string[len - 2] = '\0';
    return string;
}
