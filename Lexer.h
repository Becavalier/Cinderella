#pragma once

#include "llvm/ADT/STLExtras.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <map>
#include <memory>
#include <utility>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "ExprAST.h"
#include "PrototypeAST.h"
#include "FunctionAST.h"
#include "NumberExprAST.h"
#include "VariableExprAST.h"
#include "CallExprAST.h"
#include "BinaryExprAST.h"


namespace Cinderella {

    using namespace std;

    enum Token {
        tok_eof = -1,
        tok_def = -2,
        tok_extern = -3,
        tok_identifier = -4,
        tok_number = -5
    };

    class Lexer {
        Lexer () {}

    private:
        static string IdentifierStr;
        static double NumVal;
        static char CurTok;

        inline static int getNextToken() {
            return CurTok = _find_token();
        };

        inline static int _find_token();

        // Error handling;
        static unique_ptr<ExprAST> LogError(const char *Str) {
            fprintf(stderr, "LogError: %s\n", Str);
            return nullptr;
        }
        static unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
            LogError(Str);
            return nullptr;
        }

        /// numberexpr ::= number
        inline static unique_ptr<ExprAST> ParseNumberExpr() {
            auto Result = llvm::make_unique<NumberExprAST>(NumVal);
            Lexer::getNextToken();
            return move(Result);
        }

        /// parenexpr ::= '(' expression ')'
        inline static unique_ptr<ExprAST> ParseParenExpr() {
            Lexer::getNextToken();
            // recursion -> ParseParenExpr()
            auto V = ParseExpression();
            if (!V)
                return nullptr;

            if (CurTok != ')') {
                return LogError("expected ')'");
            }
            Lexer::getNextToken();
            return V;
        }

        inline static unique_ptr<ExprAST> ParseIdentifierExpr() {
            string IdName = IdentifierStr;

            Lexer::getNextToken();

            if (CurTok != '(')
                return llvm::make_unique<VariableExprAST>(IdName);

            Lexer::getNextToken();

            vector<unique_ptr<ExprAST>> Args;
            if (CurTok != ')') {
                while(1) {
                    if (auto Arg = ParseExpression())
                        Args.push_back(move(Arg));
                    else
                        return nullptr;

                    if (CurTok == ')') break;

                    if (CurTok != ',')
                        return LogError("Expected ')' or ',' in argument list");

                    Lexer::getNextToken();
                }
            }

            Lexer::getNextToken();

            return llvm::make_unique<CallExprAST>(IdName, move(Args));
        }

        inline static unique_ptr<ExprAST> ParsePrimary() {
            switch (CurTok) {
                case tok_identifier:
                    return ParseIdentifierExpr();
                case tok_number:
                    return ParseNumberExpr();
                case '(':
                    return ParseParenExpr();
                default: return LogError("unknown token when expecting an expression");
            }
        }

        inline static int GetTokPrecedence () {
            if (!isascii(CurTok)) {
                return -1;
            }
            int TokPrec = BinopPrecedence[CurTok];
            if (TokPrec <= 0)
                return -1;
            return TokPrec;
        }

        inline static unique_ptr<ExprAST> ParseExpression() {
            // [binop, primaryexpr]
            auto LHS = ParsePrimary();
            if (!LHS) return nullptr;

            return ParseBinOpRHS(0, move(LHS));
        }

        inline static unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, unique_ptr<ExprAST> LHS) {
            while(1) {
                int TokPrec = GetTokPrecedence();

                if (TokPrec < ExprPrec)
                    return LHS;

                int BinOp = CurTok;
                Lexer::getNextToken();

                auto RHS = ParsePrimary();
                if (!RHS) return nullptr;

                int NextPrec = GetTokPrecedence();
                if (TokPrec < NextPrec) {
                    RHS = ParseBinOpRHS(TokPrec + 1, move(RHS));
                    if (!RHS) return nullptr;
                }

                LHS = llvm::make_unique<BinaryExprAST>(BinOp, std::move(LHS),
                                                       std::move(RHS));
            }
        }

        inline static unique_ptr<PrototypeAST> ParsePrototype() {
            if (CurTok != tok_identifier)
                return move(LogErrorP("Expected function name in prototype"));

            string FnName = IdentifierStr;
            Lexer::getNextToken();

            if (CurTok != '(')
                return LogErrorP("Expected '(' in prototype");

            // Read the list of argument names.
            vector<string> ArgNames;
            while (Lexer::getNextToken() == tok_identifier)
                ArgNames.push_back(IdentifierStr);
            if (CurTok != ')')
                return LogErrorP("Expected ')' in prototype");

            // success.
            Lexer::getNextToken();  // eat ')'.

            return llvm::make_unique<PrototypeAST>(FnName, move(ArgNames));
        }

        /// definition ::= 'def' prototype expression
        inline static unique_ptr<FunctionAST> ParseDefinition() {
            cout << "Cinderella: " << "keyword 'def' recognize ..." << endl;
            Lexer::getNextToken();  // eat def.
            auto Proto = ParsePrototype();
            if (!Proto) return nullptr;

            if (auto E = ParseExpression())
                return llvm::make_unique<FunctionAST>(move(Proto), move(E));
            return nullptr;
        }

        /// external ::= 'extern' prototype
        inline static unique_ptr<PrototypeAST> ParseExtern() {
            cout << "Cinderella: " << "keyword 'extern' recognize ..." << endl;
            Lexer::getNextToken();  // eat extern.
            return ParsePrototype();
        }

        /// toplevelexpr ::= expression
        inline static unique_ptr<FunctionAST> ParseTopLevelExpr() {
            cout << "Cinderella: " << "literal expression recognize ..." << endl;
            if (auto E = ParseExpression()) {
                // Make an anonymous proto.
                auto Proto = llvm::make_unique<PrototypeAST>("", vector<string>());
                return llvm::make_unique<FunctionAST>(move(Proto), move(E));
            }
            return nullptr;
        }

        static void HandleDefinition() {
            if (ParseDefinition()) {
                fprintf(stderr, "Parsed a function definition.\n");
            } else {
                // Skip token for error recovery.
                getNextToken();
            }
        }

        static void HandleExtern() {
            if (ParseExtern()) {
                fprintf(stderr, "Parsed an extern\n");
            } else {
                // Skip token for error recovery.
                getNextToken();
            }
        }

        static void HandleTopLevelExpression() {
            // Evaluate a top-level expression into an anonymous function.
            if (ParseTopLevelExpr()) {
                fprintf(stderr, "Parsed a top-level expr\n");
            } else {
                // Skip token for error recovery.
                getNextToken();
            }
        }


    public:
        // Binary Expression Parsing;
        static map<char, int> BinopPrecedence;

        static void MainCLILoop() {
            while (1) {
                fprintf(stderr, "Cinderella Compiler CLI> ");
                // getchar();
                switch (CurTok) {
                    case tok_eof:
                        return;
                    case ';':
                        Lexer::getNextToken();
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
        }
    };
}
