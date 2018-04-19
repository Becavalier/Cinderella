#include <iostream>
#include <map>
#include <string>
#include <cstring>

#define BASE_PREFIX "[Cinderella ERROR] "


class Error {

private:
    static std::map<std::string, std::string> ErrorList;

public:
    static void ThrowErrorMessage (const std::string &key);

};