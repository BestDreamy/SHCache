#ifndef SYS_H
#define SYS_H
#include "include/trace.h"
#include "cpu/cpu.h"

#define FINISH_TIME 1e5

extern int32_t time_counter;

void sim(Vmodule* dut, VerilatedFstC* tfp, const char *filepath);

void sys_init(Vmodule* dut, VerilatedFstC* tfp);

inline void sys_exec_once(Vmodule* dut, VerilatedFstC* tfp, const Operation& op) {
    dut->clock = 1 - dut->clock; // clock = 0
    dut->eval();
    tfp->dump(time_counter ++);

    int core_id = op.core;
    cpu[core_id].exec_once(op);

    dut->clock = 1 - dut->clock; // clock = 1
    dut->eval();
    tfp->dump(time_counter ++);
}

inline void sys_exec(Vmodule* dut, VerilatedFstC* tfp,  std::ifstream& file) {
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        Operation op = read_trace_one_line(line);

        sys_exec_once(dut, tfp, op);
    }
}

#endif