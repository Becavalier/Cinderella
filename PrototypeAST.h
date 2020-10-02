#pragma once

#include "LLVMBinder.h"
#include <string>
#include <vector>
#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/DerivedTypes.h"

using namespace std;

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
public:
    PrototypeAST (const std::string &name, const std::vector<std::string> Args)
            :Name(name), Args(move(Args)) {}

    const std::string &getName() const {
        return Name;
    }

    // Used for generating LLVM IR;
    llvm::Function *codegen();
};