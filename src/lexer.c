#include "include/lexer.h"
#include "include/token.h"
#include "include/lexer_utils.h"
#include "include/keyWords.h"
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

bool lexerActualCharIsNum(const lexer_T* lexer) {
    char value = lexer->c;
    if ((value >= '0' && value <= '9') || (value == '-') ) { return true; }
    return false;
}

void lexerIncrLine(lexer_T* lexer) {
    if (lexer->c == '\n') {
        lexer->line++;
    } else if (lexer->c == '\r') {
        lexer->line++;
    }
}

char lexerPeek(lexer_T* lexer, int offSet) {
    return lexer->contents[lexer->i + offSet];
}

bool lexerPeekCheck(lexer_T* lexer, int offSet, const char checkC) {
    char actualC = lexer->contents[lexer->i + offSet];
    if (actualC == checkC) { return true; }
    return false;
}

bool lexerActualCharIs(lexer_T* lexer, const char checkC) {
    if (lexer->c == checkC) { return true; }
    return false;
}

void lexerSkipComments(lexer_T* lexer) {
    while (lexerActualCharIs(lexer, COMMENT_KW) &&
        lexerPeekCheck(lexer, 1, COMMENT_KW) &&
        !lexerPeekCheck(lexer, 1, '\0')) {
        lexerAdvance(lexer);
        lexerAdvance(lexer);

        while (!lexerActualCharIs(lexer, '\n') &&
            !lexerActualCharIs(lexer, '\r') &&
            !lexerActualCharIs(lexer, '\0')) {
            lexerAdvance(lexer);
        }
        if (lexerActualCharIs(lexer, '\r')) { lexerIncrLine(lexer); lexerAdvance(lexer); }
        if (lexerActualCharIs(lexer, '\n')) { lexerIncrLine(lexer); lexerAdvance(lexer); }
    }
}

//----------------------------------------------------------------------------------

lexer_T* initLexer(char* contents) {
    lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->line = 1;
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

        lexerIncrLine(lexer);

        while (lexerActualCharIs(lexer, COMMENT_KW) &&
            lexerPeekCheck(lexer, 1, COMMENT_KW)) { lexerSkipComments(lexer); }

        if (lexerActualCharIsWhiteSpace(lexer)) {
            lexerSkipWhiteSpace(lexer);
            continue;
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
            case '=': return lexerAdvanceWithToken(lexer,
                initToken(TOKEN_EQUALS, lexerGetCurrentCharAsString(lexer), lexer->line));
            case ';': return lexerAdvanceWithToken(lexer,
                initToken(TOKEN_SEMI, lexerGetCurrentCharAsString(lexer), lexer->line));
            case '(': return lexerAdvanceWithToken(lexer,
                initToken(TOKEN_LPAREN, lexerGetCurrentCharAsString(lexer), lexer->line));
            case ')': return lexerAdvanceWithToken(lexer,
                initToken(TOKEN_RPAREN, lexerGetCurrentCharAsString(lexer), lexer->line));
            case '*': return lexerAdvanceWithToken(lexer,
                initToken(TOKEN_PTR, lexerGetCurrentCharAsString(lexer), lexer->line));
            case '!': return lexerAdvanceWithToken(lexer,
                initToken(TOKEN_EXCL, lexerGetCurrentCharAsString(lexer), lexer->line));
            case '?': return lexerAdvanceWithToken(lexer,
                initToken(TOKEN_INTER, lexerGetCurrentCharAsString(lexer), lexer->line));
            case ',': return lexerAdvanceWithToken(lexer,
                initToken(TOKEN_COMMA, lexerGetCurrentCharAsString(lexer), lexer->line));
            case '+': return lexerAdvanceWithToken(lexer,
                initToken(TOKEN_PLUS, lexerGetCurrentCharAsString(lexer), lexer->line));
            default: return initToken(TOKEN_EOF, NULL, lexer->line);
        }
    }
}

token_T* lexerCollectNumber(lexer_T* lexer) {
    char buffer[256];
    int i = 0;
    bool isNegative = false;
    bool hasDot = false;

    while (lexerActualCharIsNum(lexer) || lexer->c == '.' || lexer->c == '-') {

        if (lexer->c == '.' && hasDot == true) {
            printf("Error declaring a decimal number at line <%d>\n", lexer->line);
            printf("Decimal values only accept ONE 'point' ( . ) symbol.\n");
            exit(1);
        }
        if (lexer->c == '-' && isNegative == true) {
            printf("Error declaring a number at line <%d>\n", lexer->line);
            printf("Negative numbers only accept one 'sign' ( - ) symbol.\n");
            exit(1);
        }

        if (lexer->c == '.') { hasDot = true; }
        if (lexer->c == '-') { isNegative = true; }
        buffer[i++] = lexer->c;
        lexerAdvance(lexer);
    }
    buffer[i] = '\0';

    if (lexerActualCharIsWhiteSpace(lexer)) {
        lexerSkipWhiteSpace(lexer);
    }
    char* value = strdup(buffer);
    return initToken(TOKEN_NUMBER, value, lexer->line);
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
    return initToken(TOKEN_STRING,  parseString, lexer->line);
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
    return initToken(TOKEN_ID,  parseID, lexer->line);
}

char* lexerGetCurrentCharAsString(lexer_T* lexer) {
    char* string = calloc(2, sizeof(char));
    string[0] = lexer->c;
    string[1] = '\0';
    return string;
}