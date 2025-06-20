#pragma once
#include "flow_utils.h"
#include "../../include/autoconfig.h"

inline reqflit_t chi_issue_ReadUnique_req(
    Vmodule* dut, VerilatedFstC* tfp, 
    const uint32_t &srcID, const uint32_t &Addr, const uint32_t &Size
) {
    reqflit_t req = createReadUnique(config.HNId[0], srcID, 0, Addr, Size);

    RN_req_channel[srcID].push(req);
    // while (true) {
    //     if (dut->RXREQLCRDV == 1) {
    //         Assert(dut->clock == 0, "Clock must be low when issuing a response");
    //         sim_half_cycle(dut, tfp); // @posedge
    //         dut->RXREQFLITPEND = 1;
    //         dut->eval();
    //         sim_half_cycle(dut, tfp);

    //         sim_half_cycle(dut, tfp); // @posedge
    //         dut->RXREQFLITPEND = 0; // Special for verilator (Real should be 0)
    //         dut->RXREQFLITV = 1;
    //         encode_chi_req_flit(dut, req);
    //         dut->eval();
    //         sim_half_cycle(dut, tfp);

    //         sim_half_cycle(dut, tfp); // @posedge
    //         dut->RXREQFLITV = 0;
    //         dut->eval();
    //         sim_half_cycle(dut, tfp);
    //         break;
    //     } else {
    //         sim_one_cycle(dut, tfp); // @posedge
    //     }
    // }
    return req;
}

extern "C" {
    // void chi_recv_ReadNoSnp_req(reqflit_t req, datflit_t *data);

    void chi_DMT_ReadNoSnp_req(const svBitVecVal* req);
}