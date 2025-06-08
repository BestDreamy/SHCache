#ifndef SYS_H
#define SYS_H
#include "include/trace.h"
#include "cpu/cpu.h"
#include <sys/types.h>
#include <vector>
#include "include/dbg.h"
#include "include/autoconfig.h"

#define FINISH_TIME 1e5
#define DUMP_TIME(time_counter) (time_counter < FINISH_TIME)? tfp->dump(time_counter ++): exit(0)

void sim(Vmodule* dut, VerilatedFstC* tfp, const char *filepath);

void sys_init(Vmodule* dut, VerilatedFstC* tfp);

inline bool sys_exec_once(Vmodule* dut, VerilatedFstC* tfp, const Operation& op) {
    dut->clock = 1 - dut->clock; // clock = 0
    dut->eval();
    DUMP_TIME(time_counter);

    int core_id = op.core;
    bool op_finished = cpu[core_id].exec_once(dut, tfp, op);

    dut->clock = 1 - dut->clock; // clock = 1
    dut->eval();
    DUMP_TIME(time_counter);

    return op_finished;
}

inline bool block_rnf_exec_once(Vmodule* dut, VerilatedFstC* tfp, const Operation &lastop, uint32_t &lastop_exec_times) {
    lastop_exec_times ++;
    Exit(lastop_exec_times < 50, "Execution time exceeded limit");

    bool ok = 0;
    int coreId = lastop.core;

    if (RN_req_channel[coreId].size()) ok = 1;
    if (RN_rsp_channel[coreId].size()) ok = 1;
    if (RN_dat_channel[coreId].size()) ok = 1;

    if (dut->RXREQLCRDV == 0) ok = 0;
    if (dut->RXRSPLCRDV == 0) ok = 0;
    // if (dut->RXDATLCRDV == 0) ok = 0;

    if (ok) {
        dut->clock = 1 - dut->clock; // clock = 0
        dut->RXREQFLITPEND = 1;
        dut->eval();
        DUMP_TIME(time_counter);

        dut->clock = 1 - dut->clock; // clock = 1
        dut->eval();
        DUMP_TIME(time_counter);

        dut->clock = 1 - dut->clock; // clock = 0
        dut->RXREQFLITPEND = 0;
        if (!RN_req_channel[coreId].empty()){
            dut->RXREQFLITV = 1;
            reqflit_t req = RN_req_channel[coreId].front();
            encode_chi_req_flit(dut, req);
            RN_req_channel[coreId].pop();
        }
        if (!RN_rsp_channel[coreId].empty()){
            dut->RXRSPFLITV = 1;
            rspflit_t rsp = RN_rsp_channel[coreId].front();
            encode_chi_rsp_flit(dut, rsp);
            RN_rsp_channel[coreId].pop();
        }
        if (!RN_dat_channel[coreId].empty()){
            dut->RXDATFLITV = 1;
            // TODO
        }
        dut->eval();
        DUMP_TIME(time_counter);

        dut->clock = 1 - dut->clock; // clock = 1
        dut->eval();
        DUMP_TIME(time_counter);
        
        dut->clock = 1 - dut->clock; // clock = 0
        dut->RXREQFLITV = 0;
        dut->eval();
        DUMP_TIME(time_counter);

        dut->clock = 1 - dut->clock; // clock = 1
        dut->eval();
        DUMP_TIME(time_counter);
    } else {
        dut->clock = 1 - dut->clock; // clock = 0
        dut->eval();
        DUMP_TIME(time_counter);

        dut->clock = 1 - dut->clock; // clock = 1
        dut->eval();
        DUMP_TIME(time_counter);
    }
    return false;
}

inline void sys_exec(Vmodule* dut, VerilatedFstC* tfp, std::ifstream& file) {
    bool lastop_finished = true;
    uint32_t lastop_exec_times = 0;
    Operation lastop;
    
    std::string line;
    while (true) {
        if (lastop_finished == false) {
            lastop_finished = block_rnf_exec_once(dut, tfp, lastop, lastop_exec_times);
            continue;
        } else {
            std::getline(file, line);
            if (line.empty()) continue;

            Operation op = read_trace_one_line(line);
            if (op.operation == OperationType::OTHER) break;
    
            lastop_finished = sys_exec_once(dut, tfp, op);

            if (!lastop_finished) lastop_exec_times = 0;

            lastop = op;
        }

    }
}

#endif