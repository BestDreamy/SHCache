#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "trace.h"

#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "VSHCache.h"
#include "verilated_fst_c.h"

typedef VSHCache Vmodule;

extern int32_t time_counter;

extern VerilatedFstC* tfp;
extern Vmodule* dut;

inline uint32_t ceil_div(uint32_t a, uint32_t b) {
    return (a + b - 1) / b;
}

inline uint32_t round_up(uint32_t a, uint32_t b) {
    return ceil_div(a, b) * b;
}
