#ifndef SYS_H
#define SYS_H
#include "include/trace.h"
#include "cpu/cpu.h"
#include <sys/types.h>
#include <vector>
#include "include/dbg.h"

#define FINISH_TIME 1e5

struct Config {
    uint32_t numRNs;
    uint32_t numHNs;
    uint32_t numSNs;

    std::vector<uint32_t> numCreditsForHNReq;
    std::vector<uint32_t> numCreditsForSNReq;

    std::vector<uint32_t> RNId;
    std::vector<uint32_t> HNId;
    std::vector<uint32_t> SNId;

    Config(uint32_t numRNs=2, uint32_t numHNs=1, uint32_t numSNs=1,
              std::vector<uint32_t> numCreditsForHNReq={4, 4}, 
              std::vector<uint32_t> numCreditsForSNReq={4})
        : numRNs(numRNs), numHNs(numHNs), numSNs(numSNs),
          numCreditsForHNReq(numCreditsForHNReq),
          numCreditsForSNReq(numCreditsForSNReq),
          RNId(numRNs, 0), HNId(numHNs, 0), SNId(numSNs, 0) {}
};

constexpr uint32_t Node_SNF[] = {0x24};
constexpr uint32_t Node_HNF[] = {0x28};
constexpr uint32_t Node_RNF[] = {0x0C, 0x2C, 0x3C, 0x4C};

extern Config config;

extern int32_t time_counter;

void sim(Vmodule* dut, VerilatedFstC* tfp, const char *filepath);

void sys_init(Vmodule* dut, VerilatedFstC* tfp);

inline bool sys_exec_once(Vmodule* dut, VerilatedFstC* tfp, const Operation& op) {
    dut->clock = 1 - dut->clock; // clock = 0
    dut->eval();
    tfp->dump(time_counter ++);

    int core_id = op.core;
    bool op_finished = cpu[core_id].exec_once(op);

    dut->clock = 1 - dut->clock; // clock = 1
    dut->eval();
    tfp->dump(time_counter ++);

    return op_finished;
}

inline bool wait_exec_finished(const Operation &lastop, uint32_t &lastop_exec_times) {
    lastop_exec_times ++;
    Assert(lastop_exec_times < 5, "Execution time exceeded limit");
    return true;
}

inline void sys_exec(Vmodule* dut, VerilatedFstC* tfp,  std::ifstream& file) {
    bool lastop_finished = true;
    uint32_t lastop_exec_times = 0;
    Operation lastop;
    
    std::string line;
    while (true) {
        if (lastop_finished == false) {
            lastop_finished = wait_exec_finished(lastop, lastop_exec_times);
            continue;
        } else {
            std::getline(file, line);
            if (line.empty()) continue;

            Operation op = read_trace_one_line(line);
    
            lastop_finished = sys_exec_once(dut, tfp, op);

            lastop = op;
        }

    }
}

#endif