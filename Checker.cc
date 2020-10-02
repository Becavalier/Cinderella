#include "Checker.h"

bool Checker::CheckGivenSourceFileType(const std::string &fileName) {
    std::string delimiter = ".";
    std::string token = fileName.substr(fileName.rfind(delimiter), fileName.size());
    if (token == ".hs") {
        return true;
    } else {
        Error::ThrowErrorMessage("ERROR_INVALID_POSTFIX");
        return false;
    }
}