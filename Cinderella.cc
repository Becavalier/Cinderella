#include "Cinderella.h"

std::map<char, int> Lexer::BinopPrecedence;


std::map<std::string, llvm::Value*> LLVMBinder::NamedValues;

llvm::LLVMContext LLVMBinder::TheContext;

llvm::IRBuilder<> LLVMBinder::Builder(LLVMBinder::TheContext);

std::unique_ptr<llvm::Module> LLVMBinder::TheModule;

std::unique_ptr<llvm::legacy::FunctionPassManager> LLVMBinder::TheFPM;


int main(int argc, char* args[]) {

    // Set precedence list;
    Lexer::BinopPrecedence['<'] = 10;
    Lexer::BinopPrecedence['+'] = 20;
    Lexer::BinopPrecedence['-'] = 30;
    Lexer::BinopPrecedence['*'] = 40;


    // Get current module;
    LLVMBinder::TheModule = llvm::make_unique<llvm::Module>("Cinderella", LLVMBinder::TheContext);

    // Create a new pass manager attached to it.
    LLVMBinder::TheFPM = llvm::make_unique<llvm::legacy::FunctionPassManager>(LLVMBinder::TheModule.get());

    // Make the module, which holds all the code.
    LLVMBinder::wrapLLVMOptimizers();

    if (argc > 1) {
        std::string executeMode(args[1]);
        if (executeMode == "-o") {
            std::string oFile(args[2]);
            std::string iFile(args[3]);

            // Check given source file first;
            if (Checker::CheckGivenSourceFileType(iFile)) {
                // Dealing with the first section (seperated by ";");
                Cinderella::MainStatic(iFile);

                bool saved = LLVMBinder::generateTargetObjectFile(oFile);
                cout << (saved ? "Object file [" + oFile + "] saved succeed!" : "Object file [" + oFile + "] saved failed!") << endl;
            }
        } else {
            // Prime the first token.
            fprintf(stderr, "Cinderella Compiler CLI> ");
            Lexer::GetNextToken();

            // Dealing with inputs;
            Cinderella::MainCLILoop();
        }
    } else {// Prime the first token.
        fprintf(stderr, "Cinderella Compiler CLI> ");
        Lexer::GetNextToken();

        // Dealing with inputs;
        Cinderella::MainCLILoop();
    }

    // Print out all of the generated code.
    // LLVMBinder::TheModule->print(llvm::errs(), nullptr);

    return 0;
}