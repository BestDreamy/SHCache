#ifndef SYS_H
#define SYS_H
#include "include/trace.h"
#include "cpu/cpu.h"
#include <sys/types.h>
#include <vector>
#include "include/dbg.h"
#include "include/autoconfig.h"

#define FINISH_TIME 1e5

void sim(Vmodule* dut, VerilatedFstC* tfp, const char *filepath);

void sys_init(Vmodule* dut, VerilatedFstC* tfp);

inline bool sys_exec_once(Vmodule* dut, VerilatedFstC* tfp, const Operation& op) {
    dut->clock = 1 - dut->clock; // clock = 0
    dut->eval();
    tfp->dump(time_counter ++);

    int core_id = op.core;
    bool op_finished = cpu[core_id].exec_once(dut, tfp, op);

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