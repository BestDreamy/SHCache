#pragma once
#include "req_flow.h"

// inline void chi_recv_ReadNoSnp_req(
//     Vmodule* dut, VerilatedFstC* tfp, 
//     const int &srcID, const uint32_t &Addr, const uint32_t &Size
// ) {
//     reqflit_t req = createReadNoSnp(srcID, Addr, Size);
//     while (true) {
//         if (dut->rxreqlcrdv == 1) {
//             dut->rxreqflitpend = 1;
//             sim_one_cycle(dut, tfp); // @posedge

//             dut->rxreqflitpend = 0;
//             dut->rxreqflitv = 1;
//             bind_chi_req_flit(dut, req);

//             sim_one_cycle(dut, tfp); // @posedge
//             dut->rxreqflitv = 0;
//             break;
//         } else {
//             sim_one_cycle(dut, tfp); // @posedge
//         }
//     }
// }
