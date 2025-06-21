#ifndef DIFFTEST_H
#define DIFFTEST_H

// #include <assert.h>
#include "../cpu/cpu.h"

inline bool check_all_reduce() {
    // Check if all cores have the same value in the register
    bool ok = true;
    if (cpu[0].reg["a1"] != 7 or cpu[0].reg["a2"] != 2 or cpu[0].reg["a0"] != 10) {
        ok = false;
    }
    if (cpu[1].reg["a1"] != 3 or cpu[1].reg["a2"] != 4 or cpu[0].reg["a0"] != 10) {
        ok = false;
    }
    
    Exit(ok, "All-reduce does not match");
    return ok;
}

inline bool check_file(const char *filepath) {
    std::string str = std::string(filepath);
    size_t pos = str.find_last_of("/\\");
    std::string filename = str.substr(pos + 1);

    if ("all-reduce.txt" == filename) {
        return check_all_reduce();
    } else {
        std::cerr << "Unknown test file: " << filename << std::endl;
        return false;
    }
}

#endif