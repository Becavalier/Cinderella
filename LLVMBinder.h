#pragma once

#include <map>
#include <system_error>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"


class LLVMBinder {

public:
    // Symbol table;
    static std::map<std::string, llvm::Value*> NamedValues;

    // Own core LLVM data structures, opaque value;
    static llvm::LLVMContext TheContext;

    // Used for generating LLVM instructions;
    static llvm::IRBuilder<> Builder;

    // Hold global functions and variables;
    static std::unique_ptr<llvm::Module> TheModule;

    // Used for optimizers <FunctionPassManager>;
    static std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;


    static void wrapLLVMOptimizers();
    static bool generateTargetObjectFile(std::string fileName);
};