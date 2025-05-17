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
