#include "sys.h"
#include "mem.h"

Config config;
CPU<> cpu[NUMCORES];
Memory mem;

void sys_init(Vmodule* dut, VerilatedFstC* tfp) {
    dut->clock = 0; dut->reset = 1; dut->eval();
    DUMP_TIME(time_counter);

    // posedge clock && reset
    dut->clock = 1; dut->reset = 1; dut->eval();
    DUMP_TIME(time_counter);

    dut->reset = 0;
}