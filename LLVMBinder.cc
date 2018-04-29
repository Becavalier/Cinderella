#include "LLVMBinder.h"

std::map<std::string, std::string> LLVMBinder::ISAList = {
        std::pair<std::string, std::string>("WASM", "wasm32-unknown-unknown-wasm")
};

void LLVMBinder::wrapLLVMOptimizers() {
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    TheFPM->add(llvm::createInstructionCombiningPass());
    // Reassociate expressions.
    TheFPM->add(llvm::createReassociatePass());
    // Eliminate Common SubExpressions.
    TheFPM->add(llvm::createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    TheFPM->add(llvm::createCFGSimplificationPass());
};

bool LLVMBinder::generateTargetObjectFile(std::string triple, std::string fileName = "_t_cinderella") {
    // Initialize the target registry etc.
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    std::string TargetTriple;

    if (triple.size() == 0) {
        TargetTriple = llvm::sys::getDefaultTargetTriple();
    } else {
        TargetTriple = ISAList.find(triple)->second;
    }

    std::cout << TargetTriple << std::endl;

    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

    // Target: x86_64-apple-darwin16.6.0
    if (!Target) {
        llvm::errs() << Error;
        return false;
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    llvm::TargetMachine* TheTargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    // Specify the target and data layout;
    TheModule->setDataLayout(TheTargetMachine->createDataLayout());
    TheModule->setTargetTriple(TargetTriple);

    auto Filename = fileName;
    std::error_code EC;
    llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::F_None);

    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message();
        return false;
    }

    llvm::legacy::PassManager pass;
    auto FileType =llvm::TargetMachine::CGFT_ObjectFile;

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
        llvm::errs() << "TargetMachine can't emit a file of this type";
        return false;
    }

    pass.run(*TheModule);
    dest.flush();

    return true;
}