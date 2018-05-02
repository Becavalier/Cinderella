#pragma once

#include "IOInterface.h"
#include <string>
#include <map>
#include <cstdio>
#include <cstdlib>
#include "llvm/ADT/STLExtras.h"


enum Token {
    tok_eof,
    tok_def,
    tok_extern,
    tok_identifier,
    tok_number,
    tok_separator,
    tok_left_paren,
    tok_right_paren,
    tok_expr_end
};

class Lexer {
    Lexer () {}

public:
    static std::string IdentifierStr;
    static double NumVal;
    static char CurTok;

    static int _find_token();


    // Get token precedence;
    inline static int GetTokPrecedence () {
        if (!isascii(CurTok)) {
            return -1;
        }
        int TokPrec = BinopPrecedence[CurTok];
        if (TokPrec <= 0) {
            return -1;
        }
        return TokPrec;
    }

    static int GetNextToken() {
        return CurTok = _find_token();
    };

    // Binary Expression Parsing;
    static std::map<char, int> BinopPrecedence;

};
