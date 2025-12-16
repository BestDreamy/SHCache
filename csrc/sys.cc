#include "sys.h"
#include "chi/rnf_utils.h"
#include "mem.h"
#include "slc/slc.h"
#include "include/autoconfig.h"

Config config;
CPU<> cpu[NUMCORES];
Memory mem;
SystemCache<> slc;

void sys_init() {
    for (int i = 0; i < NUMCORES; i ++) {
        // cpu[i] = CPU<>(config.RNId[i]);
        cpu[i].set_RN_id(config.RNId[i]);
    }

    DUMP_TIME(time_counter);
}