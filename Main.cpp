#include <iostream>
#include "TestGenerator.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::fprintf(stderr, "Usage: %s <test file>\n", argv[0]);
        return -1;
    }

    TestGenerator gen;
    if (gen.load(argv[1]) != 0) {
        std::fprintf(stderr, "Loading failed.\n");
        return -1;
    }

    gen.startTest();

    if (gen.writeStatus() != 0) {
        std::fprintf(stderr, "Failed to write status.\n");
        return -1;
    }
       
    return 0;
}
