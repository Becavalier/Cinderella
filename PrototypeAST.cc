#include "PrototypeAST.h"

llvm::Function *PrototypeAST::codegen() {
    // Make the function type:  double(double,double) etc.
    std::vector<llvm::Type *> Doubles(Args.size(), llvm::Type::getDoubleTy(LLVMBinder::TheContext));

    /*
        FunctionType * FunctionType::get (Type * Result, ArrayRef<Type *> Params, bool isVarArg);
    */
    llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(LLVMBinder::TheContext), Doubles, false);

    // Create a "Fucntion" LLVM IR;
    llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, LLVMBinder::TheModule.get());

    // Set the name of each arguments;
    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(Args[Idx++]);

    return F;
}