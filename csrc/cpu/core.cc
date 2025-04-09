#include "../inlcude/core.h"
#include "verilated_fst_c.h"
#include <sys/types.h>

int32_t time_counter = 0;

void exec_once(Vmodule* dut, VerilatedFstC* tfp) {
    dut->clock = 1 - dut->clock; 
    dut->eval();
    tfp->dump(time_counter ++);

    dut->clock = 1 - dut->clock; 
    dut->eval();
    tfp->dump(time_counter ++);
}

void cpu_exec(Vmodule* dut, VerilatedFstC* tfp, uint32_t n) {
    for (int i = 0; i < n; i ++) {
        if (time_counter > FINISH_TIME) {
            break;
        }

        exec_once(dut, tfp);
    }
}

void cpu_init(Vmodule* dut, VerilatedFstC* tfp) {
    dut->clock = 0; dut->reset = 1; dut->eval();
    tfp->dump(time_counter ++);
    dut->clock = 1; dut->reset = 1; dut->eval();
    tfp->dump(time_counter ++);

    dut->reset = 0;

    // cpu_exec(dut, tfp, -1);
}