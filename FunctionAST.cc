#include "FunctionAST.h"

llvm::Function *FunctionAST::codegen() {
    // Check extern function?
    llvm::Function *TheFunction = LLVMBinder::TheModule->getFunction(Proto->getName());

    // Generate from its prototype;
    if (!TheFunction) {
        TheFunction = Proto->codegen();
    }

    if (!TheFunction) {
        return nullptr;
    }

    if (!TheFunction->empty()) {
        return (llvm::Function*) ExprAST::LogErrorV("Function cannot be redefined.");
    }

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(LLVMBinder::TheContext, "entry", TheFunction);
    LLVMBinder::Builder.SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    LLVMBinder::NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        LLVMBinder::NamedValues[Arg.getName()] = &Arg;

    // Eval the value of function body (binary expression);
    if (llvm::Value *RetVal = Body->codegen()) {
        // Finish off the function.
        LLVMBinder::Builder.CreateRet(RetVal);

        // Validate the generated code, checking for consistency.
        llvm::verifyFunction(*TheFunction);

        return TheFunction;
    }
    // Error reading body, remove function.
    TheFunction->eraseFromParent();
    return nullptr;
}
