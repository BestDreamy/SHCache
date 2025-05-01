#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <cstdint>
#include "../include/utils.h"
#include "flit.h"

inline void chi_read_unique(
    Vmodule* dut, VerilatedFstC* tfp, 
    const int &srcID, const uint32_t &Addr, const uint32_t &Size
) {
    dut->clock = 1 - dut->clock; // clock = 1
    dut->eval();
    tfp->dump(time_counter ++);

    reqFlit req = createReadUnique(srcID, Addr, Size);

    dut->clock = 1 - dut->clock; // clock = 0
    dut->eval();
    tfp->dump(time_counter ++);
}

#endif // TRANSACTION_H