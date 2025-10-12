#include "include/fileRead.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* readFileLines(char* fileDirectory, char* fileName) {
    char* buffer = 0;
    long length;

    char filePath[256];

    strcpy(filePath, fileDirectory);
    strcat(filePath, "\\");
    strcat(filePath, fileName);

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