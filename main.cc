#include "csrc/sys.h"

std::fstream logFile("cache.log");

int main(int argc, char** argv) {

    sim(argv[1]);

    return 0;
}