#include "auto_flit.h"
#include "flit.h"
#include "transaction.h"
#include <cassert>
#include "../cpu/cpu.h"
#include "../include/utils.h"

extern "C" void chi_recv_ReadNoSnp_req(reqflit_t req, datflit_t *data) {
    data = new datflit_t;
    *data = createCompData_UC(req);
    return ;
}

extern "C" void chi_DMT_ReadNoSnp_req(const svBitVecVal* req_bits) {
    std::bitset<125> bits;
    for (int i = 0; i < 8; ++i) { // 4 words * 32 = 128 bits
        uint32_t word = req_bits[i];
        printf("%08x\n", word);
        if (i & 1) {
            Assert(word == 0, "Padding word should be 0");
            continue;
        }
        for (int j = 0; j < 32 && (i * 16 + j) < 125; ++j) {
            bits[i * 16 + j] = (word >> j) & 1;
        }
    }

    reqflit_t req = decode_req_from_bitset(bits);
    datflit_t data = createCompData_UC(req);
    
    bool validTgtID = false;
    for (int i = 0; i < config.numRNs; i++) {
        if (req.TgtID == config.RNId[i]) {
            validTgtID = true;
            break;
        }
    }
    // dbg(req.TgtID);
    // printReqFlit(req);
    // printDataFlit(data);
    Exit(validTgtID, "Invalid TgtID");
    puts("DMT");

    cpu[req.TgtID].update_cache(data);
}