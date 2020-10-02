#include "Error.h"

std::map<std::string, std::string> Error::ErrorList = {
    {"ERROR_INVALID_POSTFIX", BASE_PREFIX + (std::string) "Invalid postfix of the given source file!"}
};

void Error::ThrowErrorMessage (const std::string &key) {
    std::cout << ErrorList[key] << std::endl;
}