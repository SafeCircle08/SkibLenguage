#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "error.h"

void error(const char* errorMsg, bool _exit) {
    printf(errorMsg);
    if (_exit) { exit(-1); }
}