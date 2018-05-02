#pragma once

#include "llvm/ADT/STLExtras.h"
#include "Lexer.h"
#include "ExprAST.h"
#include "PrototypeAST.h"
#include "FunctionAST.h"
#include "NumberExprAST.h"
#include "VariableExprAST.h"
#include "CallExprAST.h"
#include "BinaryExprAST.h"
#include <iostream>
#include <memory>
#include <cstdio>
#include <string>
#include <vector>


class Parser {
    Parser () {}

public:
    // Parser functions;
    static std::unique_ptr<ExprAST> ParseExpression() {
        // [binop, primaryexpr]
        auto LHS = ParsePrimary();
        if (!LHS) {
            return nullptr;
        }

        return ParseBinOpRHS(0, std::move(LHS));
    }

    static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
        while(1) {
            int TokPrec = Lexer::GetTokPrecedence();
            if (TokPrec < ExprPrec) {
                return LHS;
            }

            int BinOp = Lexer::CurTok;

            Lexer::GetNextToken();

            auto RHS = ParsePrimary();
            if (!RHS) {
                return nullptr;
            }

            int NextPrec = Lexer::GetTokPrecedence();

            if (TokPrec < NextPrec) {
                RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
                if (!RHS) {
                    return nullptr;
                }
            }

            LHS = llvm::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
        }
    }

    static std::unique_ptr<PrototypeAST> ParsePrototype() {
        if (Lexer::CurTok != tok_identifier)
            return std::move(ExprAST::LogErrorP("Expected function name in prototype"));

        std::string FnName = Lexer::IdentifierStr;

        Lexer::GetNextToken();
        if (Lexer::CurTok != tok_left_paren)
            return ExprAST::LogErrorP("Expected '(' in prototype");

        // Read the list of argument names.
        vector<std::string> ArgNames;
        while (true) {
            const char tok = Lexer::GetNextToken();
            if (tok == tok_identifier) {
                ArgNames.push_back(Lexer::IdentifierStr);
            } else if (tok == tok_separator) {
                continue;
            } else {
                break;
            }
        }

        if (Lexer::CurTok != tok_right_paren)
            return ExprAST::LogErrorP("Expected ')' in prototype");

        // success.
        Lexer::GetNextToken();  // eat ')'.

        return llvm::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
    }

    /// definition ::= 'def' prototype expression
    static std::unique_ptr<FunctionAST> ParseDefinition() {
        cout << "Cinderella: " << "keyword 'def' recognize ..." << endl;
        Lexer::GetNextToken();  // eat def.
        auto Proto = ParsePrototype();
        if (!Proto) return nullptr;

        if (auto E = ParseExpression())
            return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
        return nullptr;
    }

    /// external ::= 'extern' prototype
    static std::unique_ptr<PrototypeAST> ParseExtern() {
        cout << "Cinderella: " << "keyword 'extern' recognize ..." << endl;
        Lexer::GetNextToken();  // eat extern.
        return ParsePrototype();
    }

    /// toplevelexpr ::= expression
    static std::unique_ptr<FunctionAST> ParseTopLevelExpr() {
        cout << "Cinderella: " << "literal top-level expression recognize ..." << endl;
        if (auto E = ParseExpression()) {
            // Make an anonymous proto.
            auto Proto = llvm::make_unique<PrototypeAST>("__anon_expr", vector<std::string>());
            return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
        }
        return nullptr;
    }

    /// numberexpr ::= number
    static std::unique_ptr<ExprAST> ParseNumberExpr() {
        auto Result = llvm::make_unique<NumberExprAST>(Lexer::NumVal);
        Lexer::GetNextToken();
        return std::move(Result);
    }

    /// parenexpr ::= '(' expression ')'
    static std::unique_ptr<ExprAST> ParseParenExpr() {
        Lexer::GetNextToken();
        // recursion -> ParseParenExpr()
        auto V = ParseExpression();
        if (!V)
            return nullptr;

        if (Lexer::CurTok != tok_right_paren) {
            return ExprAST::LogError("expected ')'");
        }
        Lexer::GetNextToken();
        return V;
    }

    static std::unique_ptr<ExprAST> ParseIdentifierExpr() {
        std::string IdName = Lexer::IdentifierStr;

        Lexer::GetNextToken();

        if (Lexer::CurTok != tok_left_paren)
            return llvm::make_unique<VariableExprAST>(IdName);

        Lexer::GetNextToken();

        vector<std::unique_ptr<ExprAST>> Args;
        if (Lexer::CurTok != tok_right_paren) {
            while(1) {
                if (auto Arg = ParseExpression()) {
                    Args.push_back(std::move(Arg));
                } else {
                    return nullptr;
                }

                if (Lexer::CurTok == tok_right_paren) {
                    break;
                }

                if (Lexer::CurTok != tok_separator) {
                    return ExprAST::LogError("Expected ')' or ',' in argument list");
                }

                Lexer::GetNextToken();
            }
        }

        Lexer::GetNextToken();

        return llvm::make_unique<CallExprAST>(IdName, std::move(Args));
    }

    static std::unique_ptr<ExprAST> ParsePrimary() {
        switch (Lexer::CurTok) {
            case tok_identifier:
                return ParseIdentifierExpr();
            case tok_number:
                return ParseNumberExpr();
            case tok_left_paren:
                return ParseParenExpr();
            default:
                return ExprAST::LogError("unknown token when expecting an expression");
        }
    }
};
