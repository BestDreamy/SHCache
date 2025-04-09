#ifndef CORE_H
#define CORE_H
#include "../utils.h"
#include "verilated_fst_c.h"
#include <sys/types.h>

#define FINISH_TIME 1e5

extern int32_t time_counter;

void cpu_init(Vmodule* dut, VerilatedFstC* tfp);
#endif