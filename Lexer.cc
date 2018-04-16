#include "Lexer.h"

string Cinderella::Lexer::IdentifierStr;
double Cinderella::Lexer::NumVal;
char Cinderella::Lexer::CurTok = ';';
map<char, int> Cinderella::Lexer::BinopPrecedence;

inline int Cinderella::Lexer::gettok() {
    static int LastChar = ' ';

    // skip any spaces;
    while(isspace(LastChar)) {
        LastChar = getchar();
    }

    // identifier: [a-zA-Z][a-zA-Z0-9]*;
    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar()))) {
            IdentifierStr += LastChar;
        }

        if (IdentifierStr == "def")
            return tok_def;

        if (IdentifierStr == "extern")
            return tok_extern;

        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') {   // Number: [0-9.]+
        string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if (LastChar == '#') {
        // Comment until end of line.
        do LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
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