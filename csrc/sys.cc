#include "sys.h"
#include "chi/rnf_utils.h"
#include "mem.h"

Config config;
CPU<> cpu[NUMCORES];
std::queue<reqflit_t> RN_req_channel[NUMCORES];
std::queue<datflit_t> RN_dat_channel[NUMCORES];
std::queue<rspflit_t> RN_rsp_channel[NUMCORES];
std::queue<snpflit_t> RN_snp_channel[NUMCORES];
Memory mem;

void sys_init() {
    for (int i = 0; i < NUMCORES; i ++) {
        cpu[i] = CPU<>(config.RNId[i]);
    }

    DUMP_TIME(time_counter);
}