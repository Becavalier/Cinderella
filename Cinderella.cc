#include "Cinderella.h"

using namespace Support;

std::map<char, int> Lexer::BinopPrecedence;


std::map<std::string, llvm::Value*> LLVMBinder::NamedValues;

llvm::LLVMContext LLVMBinder::TheContext;

llvm::IRBuilder<> LLVMBinder::Builder(LLVMBinder::TheContext);

std::unique_ptr<llvm::Module> LLVMBinder::TheModule;

std::unique_ptr<llvm::legacy::FunctionPassManager> LLVMBinder::TheFPM;


int main(int argc, const char* argv[]) {

    // Set precedence list;
    Lexer::BinopPrecedence['<'] = 10;
    Lexer::BinopPrecedence['+'] = 20;
    Lexer::BinopPrecedence['-'] = 30;
    Lexer::BinopPrecedence['*'] = 40;
    Lexer::BinopPrecedence['/'] = 40;


    // Get current module;
    LLVMBinder::TheModule = llvm::make_unique<llvm::Module>("Cinderella", LLVMBinder::TheContext);

    // Create a new pass manager attached to it.
    LLVMBinder::TheFPM = llvm::make_unique<llvm::legacy::FunctionPassManager>(LLVMBinder::TheModule.get());

    // Make the module, which holds all the code.
    LLVMBinder::wrapLLVMOptimizers();

    Options options("Cinderella", "A compiler toolchain for Cinderella language.");
    options.add("--output", "-o", "Output file (stdout if not specified)", Options::Arguments::One,
             [](Options *o, const std::string& argument) {
                 o->extra["output"] = argument;
             })
            .add("--target", "-t", "Select aim target.", Options::Arguments::One,
             [](Options *o, const std::string& argument) {
                 o->extra["target"] = argument;
             })
            .add_positional("INFILE", Options::Arguments::One,
             [](Options *o, const std::string& argument) {
                 o->extra["infile"] = argument;
             });
    options.parse(argc, argv);

    if (options.extra.size() > 0) {
        if (options.extra["infile"].size() > 0 && options.extra["output"].size() > 0) {
            const std::string oFile = options.extra["output"];
            const std::string iFile = options.extra["infile"];
            if (Checker::CheckGivenSourceFileType(iFile)) {
                // Dealing with the first section (seperated by ";");
                Cinderella::MainStatic(iFile);

                bool saved;

                if (options.extra["target"].size() > 0) {
                    saved = LLVMBinder::generateTargetObjectFile(options.extra["target"], oFile);
                } else {
                    saved = LLVMBinder::generateTargetObjectFile("", oFile);
                }

                cout << (saved ? "Object file [" + oFile + "] saved succeed!" : "Object file [" + oFile + "] saved failed!") << endl;
            }
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