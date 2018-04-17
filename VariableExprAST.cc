#include "VariableExprAST.h"

Value *VariableExprAST::Codegen() {
    // Look this variable up in the function.
    Value *V = NamedValues[Name];
    return V ? V : ErrorV("Unknown variable name");
}