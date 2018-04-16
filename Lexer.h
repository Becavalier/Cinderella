#pragma once

#include <string>
#include <vector>
#include <cctype>
#include <map>
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
            return CurTok = gettok();
        };

        inline static int gettok();

        // Error handling;
        inline static ExprAST *Error(const char *Str) {
            fprintf(stderr, "Error: %s\n", Str);
            return 0;
        }
        inline static PrototypeAST *ErrorP(const char *Str) {
            Error(Str);
            return 0;
        }
        inline static FunctionAST *ErrorF(const char *Str) {
            Error(Str);
            return 0;
        }

        /// numberexpr ::= number
        inline static ExprAST *ParseNumberExpr() {
            ExprAST *Result = new NumberExprAST(NumVal);
            getNextToken();
            return Result;
        }

        /// parenexpr ::= '(' expression ')'
        inline static ExprAST *ParseParenExpr() {
            getNextToken();
            ExprAST *V = ParseExpression();
            if (!V) return 0;

            if (CurTok != ')') {
                return Error("expected ')'");
            }
            getNextToken();
            return V;
        }

        inline static ExprAST *ParseIdentifierExpr() {
            string IdName = IdentifierStr;

            getNextToken();

            if (CurTok != '(')
                return new VariableExprAST(IdName);

            getNextToken();
            vector<ExprAST*> Args;
            if (CurTok != ')') {
                while(1) {
                    ExprAST *Arg = ParseExpression();
                    if (!Arg) return 0;

                    if (CurTok != ')') break;
                    if (CurTok != ',')
                        return Error("Expected ')' or ',' in argument list");

                    Lexer::getNextToken();
                }
            }

            getNextToken();
            return new CallExprAST(IdName, Args);
        }

        inline static ExprAST *ParsePrimary() {
            switch (CurTok) {
                case tok_identifier: return ParseIdentifierExpr();
                case tok_number:     return ParseNumberExpr();
                case '(':            return ParseParenExpr();
                default: return Error("unknown token when expecting an expression");
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

        inline static ExprAST *ParseExpression() {
            // [binop, primaryexpr]
            ExprAST *LHS = ParsePrimary();
            if (!LHS) return 0;

            return ParseBinOpRHS(0, LHS);
        }

        inline static ExprAST *ParseBinOpRHS(int ExprPrec, ExprAST *LHS) {
            while(1) {
                int TokPrec = GetTokPrecedence();
                if (TokPrec < ExprPrec)
                    return LHS;

                int BinOp = CurTok;
                getNextToken();

                ExprAST *RHS = ParsePrimary();
                if (!RHS) return 0;

                int NextPrec = GetTokPrecedence();
                if (TokPrec < NextPrec) {
                    RHS = ParseBinOpRHS(TokPrec + 1, RHS);
                    if (RHS == 0) return 0;
                }

                LHS = new BinaryExprAST(BinOp, LHS, RHS);
            }
        }

        inline static PrototypeAST *ParsePrototype() {
            if (CurTok != tok_identifier)
                return ErrorP("Expected function name in prototype");

            string FnName = IdentifierStr;
            getNextToken();

            if (CurTok != '(')
                return ErrorP("Expected '(' in prototype");

            // Read the list of argument names.
            vector<string> ArgNames;
            while (getNextToken() == tok_identifier)
                ArgNames.push_back(IdentifierStr);
            if (CurTok != ')')
                return ErrorP("Expected ')' in prototype");

            // success.
            getNextToken();  // eat ')'.

            return new PrototypeAST(FnName, ArgNames);
        }

        /// definition ::= 'def' prototype expression
        inline static FunctionAST *ParseDefinition() {
            cout << "Cinderella: " << "keyword 'def' recognize ..." << endl;
            getNextToken();  // eat def.
            PrototypeAST *Proto = ParsePrototype();
            if (Proto == 0) return 0;

            if (ExprAST *E = ParseExpression())
                return new FunctionAST(Proto, E);
            return 0;
        }

        /// external ::= 'extern' prototype
        inline static PrototypeAST *ParseExtern() {
            cout << "Cinderella: " << "keyword 'extern' recognize ..." << endl;
            getNextToken();  // eat extern.
            return ParsePrototype();
        }

        /// toplevelexpr ::= expression
        inline static FunctionAST *ParseTopLevelExpr() {
            cout << "Cinderella: " << "literal expression recognize ..." << endl;
            if (ExprAST *E = ParseExpression()) {
                // Make an anonymous proto.
                PrototypeAST *Proto = new PrototypeAST("", std::vector<std::string>());
                return new FunctionAST(Proto, E);
            }
            return 0;
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
                        getNextToken();
                        break;  // ignore top-level semicolons.
                    case tok_def:
                        ParseDefinition();
                        break;
                    case tok_extern:
                        ParseExtern();
                        break;
                    default:
                        ParseTopLevelExpr();
                        break;
                }
            }
        }
    };
}
