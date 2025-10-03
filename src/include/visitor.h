#ifndef VISITOR_H
#define VISITOR_H

#include "AST.h"

AST_T* visitorVisit(AST_T* node);

AST_T* visitorVisitVar(AST_T* node);
AST_T* visitorVisitVarDef(AST_T* node);
AST_T* visitorVisitFunctionCall(AST_T* node);
AST_T* visitorVisitString(AST_T* node);
AST_T* visitorVisitCompound(AST_T* node);

#endif //VISITOR_H