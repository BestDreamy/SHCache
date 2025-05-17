#ifndef SYS_H
#define SYS_H
#include "include/trace.h"
#include "cpu/cpu.h"
#include <sys/types.h>
#include <vector>
#include "include/dbg.h"
#include "include/autoconfig.h"

#define FINISH_TIME 1e5
#define DUMP_TIME(time_counter) (time_counter < FINISH_TIME)? tfp->dump(time_counter ++): exit(0)

void sim(Vmodule* dut, VerilatedFstC* tfp, const char *filepath);

void sys_init(Vmodule* dut, VerilatedFstC* tfp);

inline bool sys_exec_once(Vmodule* dut, VerilatedFstC* tfp, const Operation& op) {
    dut->clock = 1 - dut->clock; // clock = 0
    dut->eval();
    DUMP_TIME(time_counter);

    int core_id = op.core;
    bool op_finished = cpu[core_id].exec_once(dut, tfp, op);

    dut->clock = 1 - dut->clock; // clock = 1
    dut->eval();
    DUMP_TIME(time_counter);

    return op_finished;
}

inline bool wait_exec_finished(Vmodule* dut, VerilatedFstC* tfp, const Operation &lastop, uint32_t &lastop_exec_times) {
    dut->clock = 1 - dut->clock; // clock = 0
    dut->eval();
    DUMP_TIME(time_counter);

    lastop_exec_times ++;
    Exit(lastop_exec_times < 10, "Execution time exceeded limit");

    dut->clock = 1 - dut->clock; // clock = 1
    dut->eval();
    DUMP_TIME(time_counter);
    return false;
}

inline void sys_exec(Vmodule* dut, VerilatedFstC* tfp, std::ifstream& file) {
    bool lastop_finished = true;
    uint32_t lastop_exec_times = 0;
    Operation lastop;
    
    std::string line;
    while (true) {
        if (lastop_finished == false) {
            lastop_finished = wait_exec_finished(dut, tfp, lastop, lastop_exec_times);
            continue;
        } else {
            std::getline(file, line);
            if (line.empty()) continue;

            Operation op = read_trace_one_line(line);
    
            lastop_finished = sys_exec_once(dut, tfp, op);

            if (!lastop_finished) lastop_exec_times = 0;

            lastop = op;
        }

    }
}

#endif