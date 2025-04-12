#ifndef SYS_H
#define SYS_H
#include "cpu/cpu.h"
#include "include/trace.h"
#include "include/utils.h"

#define FINISH_TIME 1e5
#define NUMCORES 2

extern int32_t time_counter;
extern CPU<> cpu[NUMCORES];

void sim(Vmodule* dut, VerilatedFstC* tfp, std::ifstream& file);

inline void sys_init(Vmodule* dut, VerilatedFstC* tfp) {
    dut->clock = 0; dut->reset = 1; dut->eval();
    tfp->dump(time_counter ++);

    // posedge clock && reset
    dut->clock = 1; dut->reset = 1; dut->eval();
    tfp->dump(time_counter ++);

    dut->reset = 0;
}

inline void sys_exec(Vmodule* dut, VerilatedFstC* tfp,  std::ifstream& file) {
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        Operation op = read_trace_one_line(line);
    }
}

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
#endif