#include "sys.h"
#include "mem.h"

Config config;
CPU<> cpu[NUMCORES];
std::queue<reqflit_t> RN_req_channel[NUMCORES];
std::queue<datflit_t> RN_dat_channel[NUMCORES];
std::queue<rspflit_t> RN_rsp_channel[NUMCORES];
std::queue<snpflit_t> RN_snp_channel[NUMCORES];
Memory mem;

void sys_init(Vmodule* dut, VerilatedFstC* tfp) {
    dut->clock = 0; dut->reset = 1; dut->eval();
    DUMP_TIME(time_counter);

    // posedge clock && reset
    dut->clock = 1; dut->reset = 1; dut->eval();
    dut->TXREQLCRDV = 1;
    DUMP_TIME(time_counter);

    dut->reset = 0;
}
