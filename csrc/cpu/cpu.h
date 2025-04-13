#ifndef CPU_H
#define CPU_H
#include "../include/utils.h"
#include "verilated_fst_c.h"
#include "cache.h"
#include <cassert>
#include <memory>

template <size_t NumRegisters = 32, size_t NumCacheSets = 128, size_t CacheBlockSize = 4>
struct CPU {
    // Registers
    std::map<std::string, uint32_t> reg;

    // Cache
    Cache<NumCacheSets, CacheBlockSize> cache;

    CPU() {
        reg.clear();
    }

    // Read from memory (via cache)
    bool read_memory(const uint32_t &address, uint32_t& data) {
        return cache.access(address, data);
    }

    // Write to memory (via cache)
    void write_memory(const uint32_t &address, const uint32_t& data) {
        cache.update(address, data);
    }

    void exec_once(const Operation &op) {
        OperationType opType = op.operation;
        dbg_operation(op);
        switch (opType) {
            case OperationType::LOAD: {
                uint32_t address = op.address.value();
                uint32_t data = 0;
                // Load operation
                this->read_memory(address, data);
                assert(op.rs.size() == 1);
                std::string rs = op.rs[0];
                reg[rs] = data;
                break;
            }
            case OperationType::STORE: {
                uint32_t address = op.address.value();
                uint32_t data = 0;
                if (op.result.has_value()) {
                    data = std::stoi(op.result.value(), nullptr, 16);
                } else {
                    assert(op.rs.size() == 1);
                    std::string rs = op.rs[0];
                    assert(reg.find(rs) != reg.end());
                    data = reg[rs];
                }
                // Store operation
                this->write_memory(address, data);
                break;
            }
            case OperationType::COMPUTE: {
                std::string rs1 = op.rs[0];
                std::string rs2 = op.rs[1];
                assert(reg.find(rs1) != reg.end());
                assert(reg.find(rs2) != reg.end());
                uint32_t data1 = reg[rs1];
                uint32_t data2 = reg[rs2];
                assert(op.compute_type.has_value());
                switch (op.compute_type.value()) {
                    case ComputeType::ADD:
                        reg[op.result.value()] = data1 + data2;
                        break;
                    case ComputeType::SUB:
                        reg[op.result.value()] = data1 - data2;
                        break;
                    case ComputeType::AND:
                        reg[op.result.value()] = data1 & data2;
                        break;
                    case ComputeType::OR:
                        reg[op.result.value()] = data1 | data2;
                        break;
                    case ComputeType::XOR:
                        reg[op.result.value()] = data1 ^ data2;
                        break;
                    case ComputeType::SLL:
                        reg[op.result.value()] = data1 << data2;
                        break;
                    case ComputeType::SRL:
                        reg[op.result.value()] = data1 >> data2;
                        break;
                    case ComputeType::SRA:
                        reg[op.result.value()] = (int32_t)data1 >> (int32_t)data2;
                        break;
                    case ComputeType::SLT:
                        reg[op.result.value()] = (int32_t)data1 < (int32_t)data2 ? 1 : 0;
                        break;
                    case ComputeType::SLTU:
                        reg[op.result.value()] = data1 < data2 ? 1 : 0;
                    default: assert(0);
                }
            }
            default: break;
        }
    }
};

#define NUMCORES 2
extern CPU<> cpu[NUMCORES];

#endif