#ifndef AST_H
#define AST_H

#include <stddef.h>

typedef struct AST_STRUCT {
    enum {
        AST_VARIABLE,
        AST_VARIABLE_DEF,
        AST_FUNCTION_CALL,
        AST_STRING,
        AST_COMPOUND, //list of statements
        AST_BINOP,
        AST_NUMBER,
        AST_NOOP
    } type;

    /*  VARIABLE DEFINITION */
    char* variableDefName;
    char* value;
    char op;
    struct AST_STRUCT* variableDefValue;
    struct AST_STRUCT* left;
    struct AST_STRUCT* right;

    /*  VARIABLE */
    char* variableName;

    /* FUNCTION CALL */
    char* functionDefName;
    struct AST_STRUCT** functionDefArgs;
    size_t functionDefArgsSize;

    /* STRING */
    char* stringValue;

    /* COMPOUND */
    struct AST_STRUCT** compoundValue; //the actual list of statements
    size_t compoundSize; //number of statements
} AST_T ;

AST_T* initAST(int type);

#endif