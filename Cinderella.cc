#include "Cinderella.h"

std::map<char, int> Lexer::BinopPrecedence;


std::map<std::string, llvm::Value*> LLVMBinder::NamedValues;

llvm::LLVMContext LLVMBinder::TheContext;

llvm::IRBuilder<> LLVMBinder::Builder(LLVMBinder::TheContext);

std::unique_ptr<llvm::Module> LLVMBinder::TheModule;

int main(int argc, char* args[]) {
    // Set precedence list;
    Lexer::BinopPrecedence['<'] = 10;
    Lexer::BinopPrecedence['+'] = 20;
    Lexer::BinopPrecedence['-'] = 30;
    Lexer::BinopPrecedence['*'] = 40;


    // Prime the first token.
    fprintf(stderr, "ready> ");
    Lexer::GetNextToken();


    // Make the module, which holds all the code.
    LLVMBinder::TheModule = llvm::make_unique<llvm::Module>("Cinderella", LLVMBinder::TheContext);

    // Dealing with inputs;
    Cinderella::MainCLILoop();

    // Print out all of the generated code.
    LLVMBinder::TheModule->print(llvm::errs(), nullptr);

    return 0;
}