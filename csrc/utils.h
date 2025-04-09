#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <getopt.h>

inline void read_trace(std::string &line) {
    std::stringstream linestream(line);
    std::string token;

    while (std::getline(linestream, token, ',')) {
        std::stringstream field(token);
        std::string key, value;
        if (std::getline(field, key, ':') && std::getline(field, value)) {
            key.erase(0, key.find_first_not_of(" \t"));
            value.erase(0, value.find_first_not_of(" \t"));
            std::cout << "  " << key << " = " << value << '\n';
        }
    }
    std::cout << '\n';
}

#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Vswitch.h"
#include "verilated_fst_c.h"

typedef Vswitch Vmodule;

#endif