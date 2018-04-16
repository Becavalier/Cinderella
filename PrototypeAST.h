#pragma once

#include <string>
#include <vector>

using namespace std;

class PrototypeAST {
    string Name;
    vector<string> Args;

public:
    PrototypeAST (const string &name, const vector<string> &args)
            :Name(name), Args(args) {}
};