#include "NumberExprAST.h"

llvm::Value *NumberExprAST::codegen() {
    // Retrieve data from constant pool;
    return llvm::ConstantFP::get(LLVMBinder::TheContext, llvm::APFloat(Val));
}