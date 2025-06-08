#pragma once
#include "flow_utils.h"
#include "../flit/rsp_flit.h"

inline rspflit_t chi_issue_CompAck_rsp(
    Vmodule* dut, VerilatedFstC* tfp, 
    const datflit_t &data
) {
    rspflit_t rsp = createCompAck(data);
    uint32_t srcID = rsp.SrcID;

    RN_rsp_channel[srcID].push(rsp);

    return rsp;
}
