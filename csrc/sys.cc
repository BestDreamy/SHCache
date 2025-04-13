#include "sys.h"
#include "mem.h"

CPU<> cpu[NUMCORES];
Memory mem;

void sys_init(Vmodule* dut, VerilatedFstC* tfp) {
    dut->clock = 0; dut->reset = 1; dut->eval();
    tfp->dump(time_counter ++);

    // posedge clock && reset
    dut->clock = 1; dut->reset = 1; dut->eval();
    tfp->dump(time_counter ++);

    dut->reset = 0;
}