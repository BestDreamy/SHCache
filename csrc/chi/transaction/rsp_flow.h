#pragma once
#include "flow_utils.h"
#include "../flit/rsp_flit.h"

inline rspflit_t chi_issue_CompAck_rsp(
    Vmodule* dut, VerilatedFstC* tfp, 
    const datflit_t &data
) {
    rspflit_t rsp = createCompAck(data);
    while (true) {
        if (dut->RXRSPLCRDV == 1) {
            sim_half_cycle(dut, tfp); // @posedge
            dut->RXRSPFLITPEND = 1;
            dut->eval();
            sim_half_cycle(dut, tfp);

            sim_half_cycle(dut, tfp); // @posedge
            dut->RXRSPFLITPEND = 0; // Special for verilator (Real should be 0)
            dut->RXRSPFLITV = 1;
            encode_chi_rsp_flit(dut, rsp);
            dut->eval();
            sim_half_cycle(dut, tfp);

            sim_half_cycle(dut, tfp); // @posedge
            dut->RXRSPFLITV = 0;
            dut->eval();
            sim_half_cycle(dut, tfp);
            break;
        } else {
            sim_one_cycle(dut, tfp); // @posedge
        }
    }
    return rsp;
}
