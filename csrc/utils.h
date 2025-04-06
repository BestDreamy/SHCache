#ifndef UTIL_H
#define UTIL_H

// core 0, operation: load, address: 0x1000, value: 1
// core 0, operation: load, address: 0x1004, value: 2
// core 0, operation: compute, expression: 1 + 2, result: 3
// core 0, operation: store, address: 0x1008, value: 3

void read_trace() {
    std::ifstream file("trace.txt");
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string word;
        int core;
        std::string op, addr, expr;
        int val;

        ss >> word >> core;   // e.g. core 0
        ss >> word >> op;      // e.g. operation: load
        if (op == "load" || op == "store") {
            ss >> word >> addr; // e.g. address: 0x1000
        }
        if (op == "compute") {
            ss >> word >> expr; // e.g. expression: 1 + 2
        }
        ss >> word >> val;    // e.g. value: 1

        std::cout << "Core " << core << ", Operation: " << op;
        if (op == "load" || op == "store") {
            std::cout << ", Address: " << addr;
        } else if (op == "compute") {
            std::cout << ", Expression: " << expr;
        }
        std::cout << ", Value: " << val << std::endl;
    }
}

#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#endif