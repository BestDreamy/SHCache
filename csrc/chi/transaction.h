#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <cstdint>
#include "../include/utils.h"

inline void chi_read_unique(
    Vmodule* dut, VerilatedFstC* tfp, 
    const uint32_t &address
) {
    dut->clock = 1 - dut->clock; // clock = 1
    dut->eval();
    tfp->dump(time_counter ++);

    // dbg();

    dut->clock = 1 - dut->clock; // clock = 0
    dut->eval();
    tfp->dump(time_counter ++);
}

#endif // TRANSACTION_H