#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "AST.h"

typedef struct PARSER_STRUCT {
    lexer_T* lexer;
    token_T* currentToken;
    token_T* prevToken;
} parser_T;

parser_T* initParser(lexer_T* lexer);

void parserEatExpectedToken(parser_T* parser, int tokenType);

AST_T* parserParse(parser_T* parser);
AST_T* parserParseStatement(parser_T* parser);
AST_T* parserParseStatements(parser_T* parser);

AST_T* parserParseExpr(parser_T* parser);
AST_T* parserParseFactor(parser_T* parser);
AST_T* parserParseTerm(parser_T* parser);

AST_T* parserParseId(parser_T* parser);
AST_T* parserParseFunctionCall(parser_T* parser);
AST_T* parserParseVariable(parser_T* parser);
AST_T* parserParseVarDef(parser_T* parser);
AST_T* parserParseString(parser_T* parser);
AST_T* parserParseOperation(parser_T* parser);

#endif