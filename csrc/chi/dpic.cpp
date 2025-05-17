#include "auto_flit.h"
#include "transaction.h"
#include <cassert>
#include "../cpu/cpu.h"

extern "C" void chi_recv_ReadNoSnp_req(reqflit_t req, datflit_t *data) {
    data = new datflit_t;
    *data = createCompData_UC(req);
    return ;
}

extern "C" void chi_DMT_ReadNoSnp_req(reqflit_t req) {
    datflit_t data = createCompData_UC(req);
    
    bool validTgtID = false;
    for (int i = 0; i < config.numRNs; i++) {
        if (req.TgtID == config.RNId[i]) {
            validTgtID = true;
            break;
        }
    }
    Assert(validTgtID, "Invalid TgtID");

    cpu[req.TgtID].update_cache(data);
}