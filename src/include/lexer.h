#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include <stdbool.h>

typedef struct LEXER_STRUCT
{
    char c;
    unsigned int i;
    int line;
    char* contents; //source code
} lexer_T;

lexer_T* initLexer(char* contents);
token_T* lexerCollectComment(lexer_T* lexer);
token_T* lexerGetNextToken(lexer_T* lexer);
token_T* lexerCollectString(lexer_T* lexer);
token_T* lexerCollectID(lexer_T* lexer);
token_T* lexerCollectNumber(lexer_T* lexer);
token_T* lexerAdvanceWithToken(lexer_T* lexer, token_T* token);

char* lexerGetCurrentCharAsString(lexer_T* lexer);
char lexerPeek(lexer_T* lexer, int offSet);

void lexerAdvance(lexer_T* lexer);
void lexerSkipWhiteSpace(lexer_T* lexer);
void lexerIncrLine(lexer_T* lexer);
void lexerSkipComments(lexer_T* lexer);

bool lexerActualCharIsAlnum(const lexer_T* lexer);
bool lexerActualCharIsNum(const lexer_T* lexer);
bool lexerPeekCheck(lexer_T* lexer, int offSet, const char checkC);
bool lexerActualCharIs(lexer_T* lexer, const char checkC);


#endif