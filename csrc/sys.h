#ifndef SYS_H
#define SYS_H
#include "include/trace.h"
#include "cpu/cpu.h"
#include <sys/types.h>
#include <vector>
#include "include/dbg.h"
#include "include/autoconfig.h"
#include "slc/slc.h"

#define FINISH_TIME 1e4
#define DUMP_TIME(t) do { \
    if ((t) < FINISH_TIME) { \
        (t)++; \
    } else { \
        exit(0); \
    } \
} while (0)


namespace {
    struct checkTable {
        Operation *lastop;
        uint32_t lastop_exec_times;
        bool req_issued;
        bool rsp_issued;
        checkTable() : lastop(nullptr), lastop_exec_times(0), req_issued(false), rsp_issued(false) {}

        void reset(Operation *lastop) {
            this->lastop = lastop;
            this->lastop_exec_times = 0;
            this->req_issued = false;
            this->rsp_issued = false;
        }

        bool is_finished() const {
            // Assert(lastop != nullptr, "lastop should not be null");
            return (req_issued && rsp_issued) && (lastop != nullptr);
        }
    } unfinished_table;
}

void sim(const char *filepath);

void sys_init();

inline bool sys_exec_once(const Operation& op) {
    DUMP_TIME(time_counter);

    int core_id = op.core;
    return cpu[core_id].exec_once(op);
}

// #define SIM_CYCLE 2

inline bool block_rnf_exec_once(const Operation &lastop) {
    DUMP_TIME(unfinished_table.lastop_exec_times);
    Assert(unfinished_table.lastop_exec_times < 50, "Execution time exceeded limit");

    bool ok = 0;
    int coreId = lastop.core;

    if (cpu[coreId].cache.req_channel.size()) ok = 1;
    if (cpu[coreId].cache.rsp_channel.size()) ok = 1;
    if (cpu[coreId].cache.dat_channel.size()) ok = 1;

    if (ok) {
        if (!cpu[coreId].cache.req_channel.empty()){
            reqflit_t req = cpu[coreId].cache.req_channel.front();
            cpu[coreId].cache.req_channel.pop();

            slc.exec_req(req);

            unfinished_table.req_issued = true;
            DUMP_TIME(unfinished_table.lastop_exec_times);

            devLog("RN%d issued req flit\n", coreId);
        }
        // Data from memory or L3 cache put in RN_dat_channel
        else if (!cpu[coreId].cache.dat_channel.empty()) {
            datflit_t dat = cpu[coreId].cache.dat_channel.front();
            cpu[coreId].cache.dat_channel.pop();

            cpu[coreId].update_cache(dat);
            DUMP_TIME(unfinished_table.lastop_exec_times);

            devLog("RN%d reveive dat flit\n", coreId);
        }
        // 1. Already reveive data
        // 2. Data belong RN
        else {
            rspflit_t rsp = cpu[coreId].cache.rsp_channel.front();
            cpu[coreId].cache.rsp_channel.pop();

            slc.exec_rsp(rsp);

            unfinished_table.rsp_issued = true;
            DUMP_TIME(unfinished_table.lastop_exec_times);

            devLog("RN%d issued rsp flit\n", coreId);
        }
    } else {
        Assert(0, "No request or response flit in RN channel, but still in block_rnf_exec_once");
    }

    return unfinished_table.is_finished();
    // return false;
}

inline void sys_exec(std::ifstream& file) {
    bool lastop_finished = true;
    unfinished_table.reset(nullptr);
    Operation lastop;
    
    std::string line;
    while (true) {
        puts(lastop_finished? "cpu run once": "l3 run once");

        if (lastop_finished == false) {
            lastop_finished = block_rnf_exec_once(lastop);
            
            if (lastop_finished) {
                unfinished_table.reset(nullptr);

                sys_exec_once(lastop); // Load or Store complete, update Local Cache
            }
        } else { // cpu run once
            if (!std::getline(file, line)) break;
            if (line.empty()) continue;

            Operation op = read_trace_one_line(line);
            dbg_operation(op, logFile);
            Assert(op.operation != OperationType::OTHER, "Invalid operation type in trace line");
    
            lastop_finished = sys_exec_once(op);

            // When load or store have started, we should not reset lastop_exec_times
            if (!lastop_finished) unfinished_table.reset(&op);

            lastop = op;

            // dbg_operation(lastop, logFile);
        }

        cpu[lastop.core].show_cache();

        slc.show_cache();
        slc.show_snoop();
    }
}

#endif