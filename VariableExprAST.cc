#include "VariableExprAST.h"

llvm::Value *VariableExprAST::codegen() {
    // Look this variable up in the function (local variables later).
    llvm::Value *V = LLVMBinder::NamedValues[Name];
    if (!V)
        ExprAST::LogErrorV("Unknown variable name");
    return V;
}