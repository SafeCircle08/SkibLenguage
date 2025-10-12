#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKENS_STRUCT
{
    enum {
        TOKEN_COMMENT,
        TOKEN_ID,
        TOKEN_SEMI,
        TOKEN_STRING,
        TOKEN_EQUALS,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_PTR,
        TOKEN_ADR,
        TOKEN_EXCL,
        TOKEN_INTER,
        TOKEN_QUOTE,
        TOKEN_COMMA,
        TOKEN_PLUS,
        TOKEN_NUMBER,
        TOKEN_EOF
    } type;
    int tokenLine;
    char* value;
} token_T;

token_T* initToken(int type, char* value, int line);

#endif