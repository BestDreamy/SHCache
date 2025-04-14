#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <getopt.h>
#include "trace.h"

inline Operation read_trace_one_line(std::string &line) {
    std::stringstream linestream(line);
    std::string token;

    Operation op;
    while (std::getline(linestream, token, ',')) {
        std::stringstream field(token);
        std::string key, value;
        if (std::getline(field, key, ':') && std::getline(field, value)) {
            key.erase(0, key.find_first_not_of(" \t"));
            value.erase(0, value.find_first_not_of(" \t"));
            
            if (key == "core") {
                op.core = std::stoi(value);
            } else if (key == "operation") {
                if (value == "store") op.operation = OperationType::STORE;
                else if (value == "load") op.operation = OperationType::LOAD;
                else if (value == "compute") op.operation = OperationType::COMPUTE;
                else op.operation = OperationType::OTHER;
            } else if (key == "reg") {
                op.rs.push_back(value);
            } else if (key == "address") {
                op.address = std::stoul(value, nullptr, 16);
            } else if (key == "result" || key == "value") {
                op.result = value;
            } else { // Just for compute operation
                op.compute_type = stocompute_type(key);

                std::stringstream rs_stream(value);
                std::string rs;
                while (std::getline(rs_stream, rs, ' ')) {
                    op.rs.push_back(rs);
                }
            }
        }
    }
    // dbg_operation(op);
    return op;
}

#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Vswitch.h"
#include "verilated_fst_c.h"

typedef Vswitch Vmodule;

#endif