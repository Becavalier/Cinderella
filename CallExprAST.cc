#include "CallExprAST.h"

llvm::Value *CallExprAST::codegen() {
    // Look up the name in the global module table.
    llvm::Function *CalleeF = LLVMBinder::TheModule->getFunction(Callee);
    if (!CalleeF)
        return ExprAST::LogErrorV("Unknown function referenced");

    // Verify argument size;
    if (CalleeF->arg_size() != Args.size())
        return ExprAST::LogErrorV("Incorrect # arguments passed");

    std::vector<llvm::Value *> ArgsV;
    for (unsigned i = 0, e = Args.size(); i != e; ++i) {
        // Build argument bundle (corresponding LLVM IR code);
        ArgsV.push_back(CallExprAST::Args[i]->codegen());
        if (!ArgsV.back())
            return nullptr;
    }

    return LLVMBinder::Builder.CreateCall(CalleeF, ArgsV, "call");
}