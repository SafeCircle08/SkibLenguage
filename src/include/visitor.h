#ifndef VISITOR_H
#define VISITOR_H
#include "AST.h"
#include <stdlib.h>

typedef struct VISITOR_STRUCT {
    AST_T** variablesDefinitions;
    size_t variablesDefinitionsSize;
} visitor_T ;

visitor_T* initVisitor();
void visitorIncreaseVariablesDefSize(visitor_T* visitor);
AST_T* visitorVisit(visitor_T* visitor, AST_T* node);
AST_T* visitorVisitVar(visitor_T* visitor, AST_T* node);
AST_T* visitorVisitVarDef(visitor_T* visitor, AST_T* node);
AST_T* visitorVisitFunctionCall(visitor_T* visitor, AST_T* node);
AST_T* visitorVisitString(visitor_T* visitor, AST_T* node);
AST_T* visitorVisitCompound(visitor_T* visitor, AST_T* node);
AST_T* visitorVisitBinOp(visitor_T* visitor, AST_T* node);

#endif //VISITOR_H