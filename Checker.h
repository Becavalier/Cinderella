#include <string>
#include <iostream>
#include "Error.h"

class Checker {

public:
    Checker () = default;
    ~Checker () = default;

    static bool CheckGivenSourceFileType (const std::string &fileName);
};