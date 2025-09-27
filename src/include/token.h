#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKENS_STRUCT
{
    enum {
        TOKEN_ID,
        TOKEN_SEMI,
        TOKEN_STRING,
        TOKEN_EQUALS,
        TOKEN_LPAREN,
        TOKEN_RPAREN
    } type;

    char* value;
} token_T;

token_T* initToken(int type, char* value);

#endif