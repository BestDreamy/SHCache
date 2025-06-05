#include "flit/flit.h"
#include "transaction/flow.h"
#include <cassert>
#include <cstddef>
#include "../cpu/cpu.h"
#include "../include/utils.h"

// Simulate SN
// extern "C" void chi_recv_ReadNoSnp_req(reqflit_t req, datflit_t *data) {
//     data = new datflit_t;
//     *data = createCompData_UC(req);
//     return ;
// }

extern "C" void chi_DMT_ReadNoSnp_req(const svBitVecVal* req_bits) {
    // 1. SN receive ReadNoSnp request and decode it
    std::bitset<reqflit_width> bits;
    Assert(sizeof(size_t) == 8, "ABI must be 64 bits");
    for (int i = 0; i < ceil_div(reqflit_width, 32) * 2; ++i) { // 4 words * 32 = 128 bits
        uint32_t word = req_bits[i];
        if (i & 1) {
            Assert(word == 0, "Padding word should be 0");
            continue;
        }
        for (int j = 0; j < 32 && (i * 16 + j) < reqflit_width; ++j) {
            bits[i * 16 + j] = (word >> j) & 1;
        }
    }

    reqflit_t req = decode_req_from_bitset(bits);
    // 2. SN build a DataFlit according to the request
    datflit_t data = createCompData_UC(req);
    
    bool validTgtID = false;
    for (int i = 0; i < config.numRNs; i++) {
        if (data.TgtID == config.RNId[i]) {
            validTgtID = true;
            break;
        }
    }

    devLog("print req and data");
    printReqFlit(req);
    printDatFlit(data);
    Exit(validTgtID, "Invalid TgtID");
    // 3. RN receive the DataFlit and update its cache
    cpu[data.TgtID].update_cache(data);
}