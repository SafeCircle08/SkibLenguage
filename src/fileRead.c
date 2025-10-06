#include "include/fileRead.h"
#include <stdlib.h>
#include <stdio.h>

char* readFileLines(const char* filePath) {
    char* buffer = 0;
    long length;

    FILE* f = fopen(filePath, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = calloc(length, length);

        if (buffer) {
            fread(buffer, 1, length, f);
        }
        fclose(f);
        return buffer;
    }
    printf("Error reading file <<< %s >>>\n", filePath);
    exit(2);
}