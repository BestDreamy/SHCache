#ifndef TRACE_H
#define TRACE_H
#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <sstream>

enum OperationType {
    OTHER,
    STORE,
    LOAD,
    COMPUTE
};

enum ComputeType {
    INVALID,
    ADD,
    SUB,
    AND,
    OR,
    XOR,
    SLL,
    SRL,
    SRA,
    SLT,
    SLTU
};

inline ComputeType stocompute_type(const std::string& str) {
    if (str == "add") return ComputeType::ADD;
    else if (str == "sub") return ComputeType::SUB;
    else if (str == "and") return ComputeType::AND;
    else if (str == "or") return ComputeType::OR;
    else if (str == "xor") return ComputeType::XOR;
    else if (str == "sll") return ComputeType::SLL;
    else if (str == "srl") return ComputeType::SRL;
    else if (str == "sra") return ComputeType::SRA;
    else if (str == "slt") return ComputeType::SLT;
    else if (str == "sltu") return ComputeType::SLTU;
    else return ComputeType::INVALID;
}

struct Operation {
    int core;                           // core id (must)
    OperationType operation;            // (must)
    std::vector<std::string> rs;        // rs1 for load and store, rs1 and rs2 for compute (must)
    std::optional<uint32_t> address;    // Just for load and store (optional)
    std::optional<std::string> result;  // rd for compute and store (optional)
    std::optional<ComputeType> compute_type; // Just for compute (optional)
};

inline void dbg_operation(const Operation& op) {
    std::cout << "core: " << op.core << ", operation: ";
    switch (op.operation) {
        case STORE: std::cout << "store"; break;
        case LOAD: std::cout << "load"; break;
        case COMPUTE: std::cout << "compute"; break;
        default: std::cout << "other"; break;
    }
    std::cout << ", rs: ";
    for (const auto& r : op.rs) {
        std::cout << r << " ";
    }
    if (op.address) {
        std::cout << ", address: 0x" << std::hex << *op.address;
    }
    if (op.result) {
        std::cout << ", result: " << *op.result;
    }
    if (op.compute_type) {
        std::cout << ", compute_type: " << *op.compute_type;
    }
    std::cout << std::endl;
}

inline Operation read_trace_one_line(std::string &line) {
    std::stringstream linestream(line);
    std::string token;

    Operation op = {OTHER};

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

#endif // TRACE_H