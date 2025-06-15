#ifndef SYS_H
#define SYS_H
#include "include/trace.h"
#include "cpu/cpu.h"
#include <sys/types.h>
#include <vector>
#include "include/dbg.h"
#include "include/autoconfig.h"
#include "include/utils.h"

#define FINISH_TIME 1e4
#define DUMP_TIME(time_counter) (time_counter < FINISH_TIME)? tfp->dump(time_counter ++): exit(0)

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

// bool check_flow_finished(Vmodule* dut, VerilatedFstC* tfp) {
//     return dut->pocq_is_finished == 1;
// }

inline bool block_rnf_exec_once(Vmodule* dut, VerilatedFstC* tfp, const Operation &lastop) {
    unfinished_table.lastop_exec_times ++;
    Exit(unfinished_table.lastop_exec_times < 50, "Execution time exceeded limit");

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
        dut->RXRSPFLITPEND = 1;
        dut->eval();
        DUMP_TIME(time_counter);

        dut->clock = 1 - dut->clock; // clock = 1
        dut->eval();
        DUMP_TIME(time_counter);

        dut->clock = 1 - dut->clock; // clock = 0
        dut->RXREQFLITPEND = 0;
        dut->RXRSPFLITPEND = 0;
        if (!RN_req_channel[coreId].empty()){
            dut->RXREQFLITV = 1;
            reqflit_t req = RN_req_channel[coreId].front();
            encode_chi_req_flit(dut, req);
            RN_req_channel[coreId].pop();

            unfinished_table.req_issued = true;
        }
        if (!RN_rsp_channel[coreId].empty()){
            dut->RXRSPFLITV = 1;
            rspflit_t rsp = RN_rsp_channel[coreId].front();
            encode_chi_rsp_flit(dut, rsp);
            RN_rsp_channel[coreId].pop();

            unfinished_table.rsp_issued = true;
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
        dut->RXRSPFLITV = 0;
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

    return dut->pocq_is_empty == 1 and unfinished_table.is_finished();
}

inline void sys_exec(Vmodule* dut, VerilatedFstC* tfp, std::ifstream& file) {
    bool lastop_finished = true;
    unfinished_table.reset(nullptr);
    Operation lastop;
    
    std::string line;
    while (true) {
        if (lastop_finished == false) {
            lastop_finished = block_rnf_exec_once(dut, tfp, lastop);
            
            if (lastop_finished) unfinished_table.reset(nullptr);
            
            continue;
        } else {
            if (!std::getline(file, line)) break;
            if (line.empty()) continue;

            Operation op = read_trace_one_line(line);
            Assert(op.operation != OperationType::OTHER, "Invalid operation type in trace line");
    
            lastop_finished = sys_exec_once(dut, tfp, op);

            // When transaction have started, we should not reset lastop_exec_times
            if (!lastop_finished) unfinished_table.reset(&op);

            lastop = op;
        }
    }
}

#endif