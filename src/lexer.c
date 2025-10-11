#include "include/lexer.h"
#include "include/token.h"
#include "include/lexer_utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

//---------------------------------UTILS--------------------------------------------

bool lexerCanAdvance(const lexer_T* lexer) {
    return ((lexer->c != '\0') && (lexer->i < strlen(lexer->contents)));
}

bool lexerActualCharIsWhiteSpace(const lexer_T* lexer) {
    return ((lexer->c == ' ') || (lexer->c == 10)); //10 = 0x0A = '\n'
}

bool lexerActualCharIsQuote(lexer_T* lexer) {
    return (lexer->c == '"');
}

bool lexerActualCharIsAlnum(const lexer_T* lexer) {
    return isalnum(lexer->c);
}

bool lexerNextCharIs(const lexer_T* lexer,const char c) {
    return (lexer->c == c);
}

bool lexerActualCharIsNum(const lexer_T* lexer) {
    char value = lexer->c;
    if (value >= '0' && value <= '9') { return true; }
    return false;
}

//----------------------------------------------------------------------------------

lexer_T* initLexer(char* contents) {
    lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];
    return lexer;
}

void lexerAdvance(lexer_T* lexer) {
    if (lexerCanAdvance(lexer)) {
        lexer->i++;
        lexer->c = lexer->contents[lexer->i];
    }
}

void lexerSkipWhiteSpace(lexer_T* lexer) {
    while (lexerActualCharIsWhiteSpace(lexer)) {
        lexerAdvance(lexer);
    }
}

token_T* lexerGetNextToken(lexer_T* lexer) {
    while (lexerCanAdvance(lexer)) {

        if (lexerActualCharIsWhiteSpace(lexer)) {
            lexerSkipWhiteSpace(lexer);
        }

        if (lexerActualCharIsNum(lexer)) {
            return lexerCollectNumber(lexer);
        }

        if (lexerActualCharIsAlnum(lexer)) {
            return lexerCollectID(lexer);
        }

        if (lexerActualCharIsQuote(lexer)) {
            return lexerCollectString(lexer);
        }

        switch (lexer->c) {
            case '=': return lexerAdvanceWithToken(lexer, initToken(TOKEN_EQUALS, lexerGetCurrentCharAsString(lexer)));
            case ';': return lexerAdvanceWithToken(lexer, initToken(TOKEN_SEMI, lexerGetCurrentCharAsString(lexer)));
            case '(': return lexerAdvanceWithToken(lexer, initToken(TOKEN_LPAREN, lexerGetCurrentCharAsString(lexer)));
            case ')': return lexerAdvanceWithToken(lexer, initToken(TOKEN_RPAREN, lexerGetCurrentCharAsString(lexer)));
            case '*': return lexerAdvanceWithToken(lexer, initToken(TOKEN_PTR, lexerGetCurrentCharAsString(lexer)));
            case '!': return lexerAdvanceWithToken(lexer, initToken(TOKEN_EXCL, lexerGetCurrentCharAsString(lexer)));
            case '?': return lexerAdvanceWithToken(lexer, initToken(TOKEN_INTER, lexerGetCurrentCharAsString(lexer)));
            case ',': return lexerAdvanceWithToken(lexer, initToken(TOKEN_COMMA, lexerGetCurrentCharAsString(lexer)));
            case '+': return lexerAdvanceWithToken(lexer, initToken(TOKEN_PLUS, lexerGetCurrentCharAsString(lexer)));
            default: return initToken(TOKEN_EOF, NULL);
        }
    }
}

token_T* lexerCollectNumber(lexer_T* lexer) {
    char buffer[256];
    int i = 0;
    while (lexerActualCharIsNum(lexer)) {
        buffer[i++] = lexer->c;
        lexerAdvance(lexer);
    }
    buffer[i] = '\0';

    if (lexerActualCharIsWhiteSpace(lexer)) {
        lexerSkipWhiteSpace(lexer);
    }
    char* value = strdup(buffer);
    return initToken(TOKEN_NUMBER, value);
}

token_T* lexerAdvanceWithToken(lexer_T* lexer, token_T* token) {
    lexerAdvance(lexer);
    return token;
}

token_T* lexerCollectString(lexer_T* lexer) {
    lexerAdvance(lexer); //skips the first quote
    char* parseString = calloc(1, sizeof(char));
    parseString[0] = '\0';

    while (!lexerActualCharIsQuote(lexer)) {
        char* c = lexerGetCurrentCharAsString(lexer);
        parseString = realloc(parseString, (strlen(parseString) + strlen(c) + 1) * sizeof(char));
        strcat(parseString, c);
        lexerAdvance(lexer);
    }
    lexerAdvance(lexer); //skips the final <">
    return initToken(TOKEN_STRING,  parseString);
}

token_T* lexerCollectID(lexer_T* lexer) {
    char* parseID = calloc(1, sizeof(char));
    parseID[0] = '\0';
    while (lexerActualCharIsAlnum(lexer)) {
        char* c = lexerGetCurrentCharAsString(lexer);
        parseID = realloc(parseID, (strlen(parseID) + strlen(c) + 1) * sizeof(char));
        strcat(parseID, c);
        lexerAdvance(lexer);
    }
    return initToken(TOKEN_ID,  parseID);
}

char* lexerGetCurrentCharAsString(lexer_T* lexer) {
    char* string = calloc(2, sizeof(char));
    string[0] = lexer->c;
    string[1] = '\0';
    return string;
}