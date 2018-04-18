#include "IOInterface.h"

char IOInterface::buffer[1024];

int IOInterface::bufferPointer = -1;

std::string IOInterface::CompileSourceName = "";

char IOInterface::ReadCharacterSource (void) {
    if (CompileSourceName != "") {
        char _t = buffer[++bufferPointer];
        std::cout << _t << std::endl;
        return _t;
    } else {
        // CLI mode;
        return getchar();
    }
}

void IOInterface::InitialBufferPayload (const std::string &fileName) {
    // Compiling mode;
    std::ifstream in(fileName);

    CompileSourceName = fileName;

    char c;

    int counter = 0;
    if(in.is_open()) {
        while(in.good()) {
            in.get(c);
            // Play with the data
            buffer[counter] = c;
            std::cout << c << std::endl;
            counter++;
        }
        buffer[counter] = EOF;
    }

    if(!in.eof() && in.fail()) {
        std::cout << "[Cinderella] Error reading " << CompileSourceName << std::endl;
    }

    in.close();

}