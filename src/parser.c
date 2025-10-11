#include "include/parser.h"
#include "include/parserUtils.h"
#include "include/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*------------------- PARSER UTILS -------------------*/
bool parserTypeIsTheWanted(parser_T* parser, int tokenType) {
    return (parser->currentToken->type == tokenType);
}

void parserSetPrevToken(parser_T* parser) {
    parser->prevToken = parser->currentToken;
}

void parserGetNextToken(parser_T* parser) {
    parser->currentToken = lexerGetNextToken(parser->lexer);
};
/*-----------------------------------------------------*/

/*------------------ PARSER FUNCTIONS -----------------*/

parser_T* initParser(lexer_T* lexer) {
    parser_T* parser = calloc(1, sizeof(parser_T));
    parser->lexer = lexer;
    parser->currentToken = lexerGetNextToken(lexer);
    parser->prevToken = parser->currentToken;
    return parser;
}

void parserEatExpectedToken(parser_T* parser, int tokenType) {
    if (parserTypeIsTheWanted(parser, tokenType)) {
        parserSetPrevToken(parser);
        parserGetNextToken(parser);
    } else {
        printf(
            "Unexpected token %s with type %d\n",
            parser->currentToken->value,
            parser->currentToken->type
        );
        exit(1);
    }
}

AST_T* parserParse(parser_T* parser) {
    return parserParseStatements(parser);
}

AST_T* parserParseStatement(parser_T* parser) {
    switch (parser->currentToken->type) {
        case TOKEN_ID: return parserParseId(parser);
        default: return initAST(AST_NOOP);
    }
}

//This function returns a list of nodes (compound)
AST_T* parserParseStatements(parser_T* parser) {
    AST_T* compound = initAST(AST_COMPOUND);
    compound->compoundValue = calloc(1, sizeof(struct AST_T*));

    AST_T* astStatement = parserParseStatement(parser);
    compound->compoundValue[0] = astStatement;
    compound->compoundSize++;

    //The first check after the first statement is parsed
    if (!parserTypeIsTheWanted(parser, TOKEN_SEMI)) {
        printf("Forgot a ';' somewhere...\n");
        exit(-1);
    }

    //While it finds semis
    while (parserTypeIsTheWanted(parser, TOKEN_SEMI)) {
        parserEatExpectedToken(parser, TOKEN_SEMI);
        //----------------------------------
        AST_T* astStatement = parserParseStatement(parser);
        if (astStatement) {
            compound->compoundSize++;
            compound->compoundValue = realloc(
                compound->compoundValue,
                compound->compoundSize * sizeof(AST_T*)
            );
            compound->compoundValue[compound->compoundSize - 1] = astStatement;
        }
    }

    //If it doesnt find semi, it means the prev token is not semi, so it
    //exit the program (error)
    if (parser->prevToken->type != TOKEN_SEMI) {
        printf("prev token: %d\n", parser->prevToken->type);
        printf("Forgot a ';' somewhere...\n");
        exit(-1);
    }
    return compound;
}
/*-----------------------------------------------------*/

/*---------- PARSER PARSER SPECIFIC TOKENS-------------*/

AST_T* parserParseId(parser_T* parser) {
    if (strcmp(parser->currentToken->value, "var") == 0) {
        return parserParseVarDef(parser);
    } else {
        return parserParseVariable(parser);
    }
}

AST_T* parserParseVarDef(parser_T* parser) {
    parserEatExpectedToken(parser, TOKEN_ID); //var
    char* varName = parser->currentToken->value;
    parserEatExpectedToken(parser, TOKEN_ID); //var name
    parserEatExpectedToken(parser, TOKEN_EQUALS); // var name =
    AST_T* varValue = parserParseExpr(parser); //var name = expr
    AST_T* varDef = initAST(AST_VARIABLE_DEF);
    varDef->variableDefName = varName;
    varDef->variableDefValue = varValue;
    return varDef;
}

AST_T* parserParseVariable(parser_T* parser) {
    char* varName = parser->currentToken->value;
    parserEatExpectedToken(parser, TOKEN_ID);
    if (parserTypeIsTheWanted(parser, TOKEN_LPAREN)) {
        return parserParseFunctionCall(parser);
    }

    AST_T* astVariable = initAST(AST_VARIABLE);
    astVariable->variableName = varName;
    return astVariable;
}

AST_T* parserParseString(parser_T* parser) {
    AST_T* astString = initAST(AST_STRING);
    astString->stringValue = parser->currentToken->value;
    parserEatExpectedToken(parser, TOKEN_STRING);
    return astString;
}

AST_T* parserParseFunctionCall(parser_T* parser) {
    AST_T* funcCallNode = initAST(AST_FUNCTION_CALL);
    char* funcName = parser->prevToken->value;
    funcCallNode->functionDefName = funcName;

    parserEatExpectedToken(parser, TOKEN_LPAREN);

    funcCallNode->functionDefArgs = calloc(1, sizeof(AST_T*));

    AST_T* astExpr = parserParseExpr(parser);
    funcCallNode->functionDefArgs[0] = astExpr;
    funcCallNode->functionDefArgsSize++;

    while (parserTypeIsTheWanted(parser, TOKEN_COMMA)) {
        parserEatExpectedToken(parser, TOKEN_COMMA);
        AST_T* astExpr = parserParseExpr(parser);

        funcCallNode->functionDefArgsSize++;
        funcCallNode->functionDefArgs = realloc(
            funcCallNode->functionDefArgs,
            funcCallNode->functionDefArgsSize * sizeof(AST_T*)
        );
        funcCallNode->functionDefArgs[funcCallNode->functionDefArgsSize - 1] = astExpr;
    }
    parserEatExpectedToken(parser, TOKEN_RPAREN);
    return funcCallNode;
}

AST_T* parserParseExpr(parser_T* parser) {
    switch (parser->currentToken->type) {
        case TOKEN_STRING: return parserParseString(parser);
        case TOKEN_ID: return parserParseId(parser);
        case TOKEN_NUMBER: return parserParseOperation(parser);
        default:
            const char* GREEN = "\x1b[31m";
            const char* RESET = "\x1b[0m";
            printf("%sInvalid expression!\nBecause of token type <%d>!%s\n", GREEN, parser->prevToken->type,RESET);
            exit(1); //be careful here
            return initAST(AST_NOOP);
    }
}

AST_T* parserParseOperation(parser_T* parser) {
    AST_T* left = parserParseFactor(parser);
    while (parser->currentToken->type == TOKEN_PLUS) {
        parserEatExpectedToken(parser, TOKEN_PLUS);
        AST_T* right = parserParseFactor(parser);
        AST_T* sum = calloc(1, sizeof(AST_T));
        sum->type =  AST_NUMBER;
        sum->op = '+';
        sum->left = left;
        sum->right = right;
        left = sum;
    }

    return left;
}

AST_T* parserParseSum(parser_T* parser) {

}


AST_T* parserParseFactor(parser_T* parser) {
    if (parser->currentToken->type == TOKEN_NUMBER) {
        parserEatExpectedToken(parser, TOKEN_NUMBER);
        AST_T* astNum = calloc(1, sizeof(AST_T));
        astNum->type = AST_NUMBER;
        astNum->value = parser->prevToken->value;
        return astNum;
    }

    printf("Expecting a number!\n");
    exit(1);
}
AST_T* parserParseTerm(parser_T* parser) {

}