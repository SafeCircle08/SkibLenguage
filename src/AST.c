#include "include/AST.h"
#include <stdlib.h>

AST_T* initAST(int type) {
    AST_T* newAst = calloc(1, sizeof(AST_T));
    newAst->type = type;

    /*  VARIABLE DEFINITION */
    newAst->variableDefName = (void*)0;
    newAst->variableDefValue = (void*)0;;

    /*  VARIABLE */
    newAst->variableName = (void*)0;;

    /* FUNCTION CALL */
    newAst->functionDefName = (void*)0;;
    newAst->functionDefArgs = (void*)0;;
    newAst->functionDefArgsSize = 0; //not (void*)0 because it is size_t

    /* STRING */
    newAst->stringValue = (void*)0;

    /* COMPOUND */
    newAst->compoundValue = (void*)0;
    newAst->compoundSize = 0;

    return newAst;
}