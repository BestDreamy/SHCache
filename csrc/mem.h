#ifndef MEM_H
#define MEM_H

#include <map>

struct Memory {
    std::map<uint32_t, uint32_t> mem;

    Memory() {
        mem.clear();
    }

    // Read from memory
    bool read_memory(const uint32_t &address, uint32_t& data) {
        auto it = mem.find(address);
        if (it == mem.end()) {
            mem[address] = 0;
        }
        data = it->second;
        return true;
    }

    // Write to memory
    void write_memory(const uint32_t &address, const uint32_t &data) {
        mem[address] = data;
    }
};

extern Memory mem;
#endif