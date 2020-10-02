#include <iostream>
#include <map>
#include <string>
#include <cstring>

#define BASE_PREFIX "[Cinderella ERROR] "

class Error {
    static std::map<std::string, std::string> ErrorList;
public:
    Error () = default;
    ~Error () = default;

    static void ThrowErrorMessage (const std::string &key);

};