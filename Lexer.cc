#include "Lexer.h"

std::string Lexer::IdentifierStr;
double Lexer::NumVal;
char Lexer::CurTok = ';';

// the implementation of "_find_token()"
int Lexer::_find_token() {
    static int LastChar = ' ';

    // skip spaces;
    while(isspace(LastChar)) {
        LastChar = getchar();
    }

    // identifier for style like: [a-zA-Z][a-zA-Z0-9]*;
    if (isalpha(LastChar)) {
        Lexer::IdentifierStr = LastChar;
        // alphanumeric condition;
        while (isalnum((LastChar = getchar()))) {
            Lexer::IdentifierStr += LastChar;
        }

        if (Lexer::IdentifierStr == "def")
            return tok_def;

        if (Lexer::IdentifierStr == "extern")
            return tok_extern;

        return tok_identifier;
    }

    // number for style like: [0-9.]+
    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        // convert a string to double
        NumVal = std::strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if (LastChar == '#') {
        // comment until end of line.
        do LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return _find_token();
    }

    // check for end of file.
    if (LastChar == EOF)
        return tok_eof;

    // otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}
