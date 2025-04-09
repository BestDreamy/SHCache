#ifndef TRACE_H
#define TRACE_H
#include <string>
#include <vector>
#include <optional>

enum OperationType {
    STORE,
    LOAD,
    COMPUTE,
    OTHER
};

// 通用数据结构
struct Operation {
    int core;                          // core id (must)
    OperationType operation;           // (must)
    std::vector<std::string> rs;       // rs1 for load and store, rs1 and rs2 for compute (must)
    std::optional<uint32_t> address;   // Just for load and store (optional)
    std::optional<std::string> result; // rd for compute (optional)
};

// std::vector<Operation> operations = {
//     {1, OperationType::STORE, "a0", 0x2000, std::nullopt, std::nullopt},
//     {0, OperationType::LOAD, "a1", 0x2000, std::nullopt, std::nullopt},
//     {1, OperationType::LOAD, "a1", 0x1000, std::nullopt, std::nullopt},
//     {0, OperationType::COMPUTE, std::nullopt, std::nullopt, "a1 a0", "a0"}
// };

#endif // TRACE_H