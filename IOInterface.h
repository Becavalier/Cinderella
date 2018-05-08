#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>

class IOInterface {

private:
    static char buffer[1024];
    static int bufferPointer;

public:
    IOInterface () = default;
    ~IOInterface () = default;

    static std::string CompileSourceName;

    static char ReadCharacterSource (void);
    static void InitialBufferPayload (const std::string &fileName);

};