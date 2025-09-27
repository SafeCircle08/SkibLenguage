#ifndef LEXER_UTILS_H
#define LEXER_UTILS_H
#include <stdbool.h>
#include <string.h>

bool lexerCanAdvance(const lexer_T* lexer);
bool lexerActualCharIsWhiteSpace(const lexer_T* lexer);
bool lexerActualCharIsQuote(lexer_T* lexer);

#endif //LEXER_UTILS_H
