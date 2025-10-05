#include "include/lexer.h"
#include "include/token.h"
#include "include/lexer_utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

//---------------------------------UTILS--------------------------------------------

bool lexerCanAdvance(const lexer_T* lexer) {
    return ((lexer->c != '0') && (lexer->i < strlen(lexer->contents)));
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

        if (lexerActualCharIsAlnum(lexer)) {
            return lexerCollectID(lexer);
        }

        if (lexerActualCharIsQuote(lexer)) {
            return lexerCollectString(lexer);
        }

        switch (lexer->c) {
            case '=': return lexerAdvanceWithToken(lexer, initToken(TOKEN_EQUALS, lexerGetCurrentCharAsString(lexer))); break;
            case ';': return lexerAdvanceWithToken(lexer, initToken(TOKEN_SEMI, lexerGetCurrentCharAsString(lexer))); break;
            case '(': return lexerAdvanceWithToken(lexer, initToken(TOKEN_LPAREN, lexerGetCurrentCharAsString(lexer))); break;
            case ')': return lexerAdvanceWithToken(lexer, initToken(TOKEN_RPAREN, lexerGetCurrentCharAsString(lexer))); break;
            case '*': return lexerAdvanceWithToken(lexer, initToken(TOKEN_PTR, lexerGetCurrentCharAsString(lexer))); break;
            case '!': return lexerAdvanceWithToken(lexer, initToken(TOKEN_EXCL, lexerGetCurrentCharAsString(lexer))); break;
            case '?': return lexerAdvanceWithToken(lexer, initToken(TOKEN_INTER, lexerGetCurrentCharAsString(lexer))); break;
            case ',': return lexerAdvanceWithToken(lexer, initToken(TOKEN_COMMA, lexerGetCurrentCharAsString(lexer))); break;

            default: return initToken(TOKEN_EOF, NULL);
        }
    }
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