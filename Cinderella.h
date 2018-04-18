#pragma once

#include "Lexer.h"
#include "Parser.h"
#include "LLVMBinder.h"
#include "IOInterface.h"
#include <cstdio>
#include <map>
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"


class Cinderella {
    Cinderella() {}

private:
    static void processSequenceCore () {
        switch (Lexer::CurTok) {
            case tok_eof:
                return;
            case ';':
                std::fprintf(stderr, "Cinderella Compiler CLI> ");
                Lexer::GetNextToken();
                // Re-Parsing;
                processSequenceCore();
                break;  // ignore top-level semicolons.
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }

public:
    // Handler
    static void HandleDefinition() {
        if (auto FnAST = Parser::ParseDefinition()) {
            if (auto *FnIR = FnAST->codegen()) {
                std::fprintf(stderr, "Read function definition:");
                FnIR->print(llvm::errs());
                std::fprintf(stderr, "\n");
            }
        } else {
            // Skip token for error recovery.
            Lexer::GetNextToken();
        }
    }

    static void HandleExtern() {
        if (auto ProtoAST = Parser::ParseExtern()) {
            if (auto *FnIR = ProtoAST->codegen()) {
                std::fprintf(stderr, "Read extern: ");
                FnIR->print(llvm::errs());
                std::fprintf(stderr, "\n");
            }
        } else {
            // Skip token for error recovery.
            Lexer::GetNextToken();
        }
    }

    static void HandleTopLevelExpression() {
        // Evaluate a top-level expression into an anonymous function.
        if (auto FnAST = Parser::ParseTopLevelExpr()) {
            if (auto *FnIR = FnAST->codegen()) {
                std::fprintf(stderr, "Read top-level expression:");
                FnIR->print(llvm::errs());
                std::fprintf(stderr, "\n");
            }
        } else {
            // Skip token for error recovery.
            Lexer::GetNextToken();
        }
    }


    // Main processing loop;
    static void MainCLILoop() {
        while (1) {
            processSequenceCore();
        }
    }

    static void MainStatic(const std::string &fileName) {
        // Set pre-payload of source code into memory;
        IOInterface::InitialBufferPayload(fileName);

        processSequenceCore();
    }

};

