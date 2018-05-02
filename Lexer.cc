#include "Lexer.h"

std::string Lexer::IdentifierStr;
double Lexer::NumVal;
char Lexer::CurTok = tok_expr_end;

// the implementation of "_find_token()"
int Lexer::_find_token() {

    static int LastChar = ' ';

    // skip spaces;
    while(isspace(LastChar)) {
        LastChar = IOInterface::ReadCharacterSource();
    }

    // identifier for style like: [a-zA-Z][a-zA-Z0-9]*;
    if (isalpha(LastChar)) {
        Lexer::IdentifierStr = LastChar;
        // alphanumeric condition;
        while (isalnum((LastChar = IOInterface::ReadCharacterSource()))) {
            Lexer::IdentifierStr += LastChar;
        }

        if (Lexer::IdentifierStr == "def") {
            return tok_def;
        }

        if (Lexer::IdentifierStr == "extern")
            return tok_extern;

        return tok_identifier;
    }

    // number for style like: [0-9.]+
    if (isdigit(LastChar)) {
        std::string NumStr;
        bool validNumStr = true;
        do {
            if (std::count(NumStr.begin(), NumStr.end(), '.') > 1)
                validNumStr = false;

            NumStr += LastChar;
            LastChar = IOInterface::ReadCharacterSource();
        } while (isdigit(LastChar) || LastChar == '.');

        if (validNumStr) {
            // convert a string to double
            NumVal = std::strtod(NumStr.c_str(), 0);
            return tok_number;
        }
    }

    if (LastChar == '#') {
        // comment until end of line.
        do LastChar = IOInterface::ReadCharacterSource();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return _find_token();
    }

    std::vector<char> vec{'(', ')', ',', ';'};
    if (std::find(vec.begin(), vec.end(), LastChar) != vec.end()) {
        char t;
        switch (LastChar) {
            case '(':
                t = tok_left_paren;
                break;
            case ')':
                t = tok_right_paren;
                break;
            case ',':
                t = tok_separator;
                break;
            case ';':
                t = tok_expr_end;
                break;
        }
        LastChar = IOInterface::ReadCharacterSource();
        return t;
    }

    // check for end of file.
    if (LastChar == EOF)
        return tok_eof;

    // otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = IOInterface::ReadCharacterSource();
    return ThisChar;
}
