#include "Lexer.h"

string Cinderella::Lexer::IdentifierStr;
double Cinderella::Lexer::NumVal;
char Cinderella::Lexer::CurTok = ';';
map<char, int> Cinderella::Lexer::BinopPrecedence;

// the implementation of "_find_token()"
inline int Cinderella::Lexer::_find_token() {
    static int LastChar = ' ';

    // skip spaces;
    while(isspace(LastChar)) {
        LastChar = getchar();
    }

    // identifier for style like: [a-zA-Z][a-zA-Z0-9]*;
    if (isalpha(LastChar)) {
        Cinderella::Lexer::IdentifierStr = LastChar;
        // alphanumeric condition;
        while (isalnum((LastChar = getchar()))) {
            Cinderella::Lexer::IdentifierStr += LastChar;
        }

        if (Cinderella::Lexer::IdentifierStr == "def")
            return tok_def;

        if (Cinderella::Lexer::IdentifierStr == "extern")
            return tok_extern;

        return tok_identifier;
    }

    // number for style like: [0-9.]+
    if (isdigit(LastChar) || LastChar == '.') {
        string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        // convert a string to double
        NumVal = strtod(NumStr.c_str(), 0);
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

int main(int argc, char* args[]) {
    // Set precedence list;
    Cinderella::Lexer::BinopPrecedence['<'] = 10;
    Cinderella::Lexer::BinopPrecedence['+'] = 20;
    Cinderella::Lexer::BinopPrecedence['-'] = 30;
    Cinderella::Lexer::BinopPrecedence['*'] = 40;

    // Dealing with inputs;
    Cinderella::Lexer::MainCLILoop();
}