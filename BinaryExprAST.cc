#include "BinaryExprAST.h"

llvm::Value *BinaryExprAST::codegen() {
    llvm::Value *L = LHS->codegen();
    llvm::Value *R = RHS->codegen();
    if (!L || !R)
        return nullptr;

    switch (Op) {
        case '+':
            return LLVMBinder::Builder.CreateFAdd(L, R, "add");
        case '-':
            return LLVMBinder::Builder.CreateFSub(L, R, "sub");
        case '*':
            return LLVMBinder::Builder.CreateFMul(L, R, "mul");
        case '/':
            return LLVMBinder::Builder.CreateFDiv(L, R, "div");
        case '<':
            L = LLVMBinder::Builder.CreateFCmpULT(L, R, "cmp");
            // Convert bool 0/1 to double 0.0 or 1.0, integer to float;
            return LLVMBinder::Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(LLVMBinder::TheContext), "bool");
        default:
            return ExprAST::LogErrorV("Invalid binary operator");
    }
}