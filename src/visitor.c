#include "include/visitor.h"
#include "include/error.h"
#include "include/builtInStringUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* -------------------- BUILT IN METHODS ----------------- */

static AST_T* builtInPrintMethod(visitor_T* visitor, AST_T** arguments, int argumentsN) {
    for (int i = 0; i < argumentsN; i++) {
        AST_T* visitedArg = visitorVisit(visitor, arguments[i]);
        switch (visitedArg->type) {
            case AST_STRING:
                if (endsWithBreakLine(visitedArg->stringValue)) {
                    visitedArg->stringValue = terminateStringBeforeBr(visitedArg->stringValue);
                    printf("%s\n", visitedArg->stringValue);
                } else {
                    printf("%s", visitedArg->stringValue);
                }
                break;
            case AST_NUMBER: printf("%s", visitedArg->value); break;
            default: error("<<<< Invalid datatype error >>>>\n", true);
        }
    }
    return initAST(AST_NOOP);
}

/* ------------------------------------------------------- */

/* -------------------- VISITOR UTILS -------------------- */
void visitorIncreaseVariablesDefSize(visitor_T* visitor) {
    visitor->variablesDefinitionsSize++;
    visitor->variablesDefinitions = realloc(
        visitor->variablesDefinitions,
        sizeof(AST_T*) * visitor->variablesDefinitionsSize
    );
}
/* ------------------------------------------------------- */

AST_T* visitorVisitNumber(visitor_T* visitor, AST_T* node) {
    return node;
}

visitor_T* initVisitor() {
    visitor_T* visitor = calloc(1, sizeof(visitor_T));
    visitor->variablesDefinitions = (void*)0;
    visitor->variablesDefinitionsSize = 0;
    return visitor;
}

AST_T* visitorVisit(visitor_T* visitor, AST_T* node) {
    switch (node->type) {
        case AST_VARIABLE: return visitorVisitVar(visitor, node);
        case AST_VARIABLE_DEF: return visitorVisitVarDef(visitor, node);
        case AST_FUNCTION_CALL: return visitorVisitFunctionCall(visitor, node);
        case AST_STRING: return visitorVisitString(visitor, node);
        case AST_COMPOUND: return visitorVisitCompound(visitor, node);
        case AST_NUMBER: return visitorVisitNumber(visitor, node);
        case AST_NOOP: return node;
    }
    error("<<<< Invalid node encountered error >>>>\n", true);
}

AST_T* visitorVisitVarDef(visitor_T* visitor, AST_T* node) {
    if (visitor->variablesDefinitions == (void*)0) {
        visitor->variablesDefinitions = calloc(1, sizeof(AST_T*));
        visitor->variablesDefinitions[0] = node;
        visitor->variablesDefinitionsSize += 1;
    } else {
        visitorIncreaseVariablesDefSize(visitor);
        visitor->variablesDefinitions[visitor->variablesDefinitionsSize - 1] = node;
    }
    return node;
}

AST_T* visitorVisitVar(visitor_T* visitor, AST_T* node) {
    for (int i = 0; i < visitor->variablesDefinitionsSize; i++) {
        AST_T* vardef = visitor->variablesDefinitions[i];

        if (strcmp(vardef->variableDefName, node->variableName) == 0) {
            return visitorVisit(visitor, vardef->variableDefValue);
        }
    }

    if (node->type == AST_VARIABLE) {
        const char* RED = "\x1b[31m";    // rosso
        const char* RESET = "\x1b[0m";
        printf("%s<<< Variable ' %s ' >>> is not defined.%s\n", RED, node->variableName, RESET);
        error("<<<<!!!!! WARNING:: -> FATAL ERROR SUPER HARD TO RESOLVE (undefined variable) <- !!!!!>>>>\n", true);
    }
    return node;
}


AST_T* visitorVisitFunctionCall(visitor_T* visitor, AST_T* node) {
    if (strcmp(node->functionDefName, "print") == 0) {
        AST_T** args = node->functionDefArgs;
        int argsSize = node->functionDefArgsSize;
        return builtInPrintMethod(visitor, args, argsSize);
    }
    error("<<<< Undefined method error with name: ' ", false);
    printf("%s", node->functionDefName);
    error(" ' >>>>\n", true);
}

AST_T* visitorVisitString(visitor_T* visitor, AST_T* node) {
    return node;
}



AST_T* visitorVisitCompound(visitor_T* visitor, AST_T* node) {
    for (int i = 0; i < node->compoundSize; i++) {
        visitorVisit(visitor, node->compoundValue[i]);
    }
    return initAST(AST_NOOP);
}