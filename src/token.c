#include <stdlib.h>
#include "include/token.h"

token_T* initToken(int type, char* value, int line) {
    token_T* token = calloc(1, sizeof(struct TOKENS_STRUCT));
    token->type = type;
    token->value = value;
    token->tokenLine = line;
    return token;
}