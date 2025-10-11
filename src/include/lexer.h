#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include <stdbool.h>

typedef struct LEXER_STRUCT
{
    char c;
    unsigned int i;
    char* contents; //source code
} lexer_T;

lexer_T* initLexer(char* contents);
void lexerAdvance(lexer_T* lexer);
void lexerSkipWhiteSpace(lexer_T* lexer);
token_T* lexerGetNextToken(lexer_T* lexer);
token_T* lexerCollectString(lexer_T* lexer);
token_T* lexerCollectID(lexer_T* lexer);
token_T* lexerAdvanceWithToken(lexer_T* lexer, token_T* token);
char* lexerGetCurrentCharAsString(lexer_T* lexer);
token_T* lexerCollectNumber(lexer_T* lexer);


#endif