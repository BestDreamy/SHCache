#ifndef SYS_H
#define SYS_H
#include "inlcude/core.h"

inline void sys_init(Vmodule* dut, VerilatedFstC* tfp, std::ifstream& file) {
    // cpu_init(dut, tfp);

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        read_trace(line);
    }
}

#endif