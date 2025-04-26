#include "sys.h"
#include "mem.h"

Config config;
CPU<> cpu[NUMCORES];
Memory mem;

void sys_init(Vmodule* dut, VerilatedFstC* tfp) {
    dut->clock = 0; dut->reset = 1; dut->eval();
    tfp->dump(time_counter ++);

    // posedge clock && reset
    dut->clock = 1; dut->reset = 1; dut->eval();
    tfp->dump(time_counter ++);

    for (int i = 0; i < config.numRNs; ++i) 
        config.RNId[i] = Node_RNF[i];
    for ( int i = 0; i < config.numHNs; ++i) 
        config.HNId[i] = Node_HNF[i];
    for (int i = 0; i < config.numSNs; ++i)
        config.SNId[i] = Node_SNF[i];

    dut->reset = 0;
}