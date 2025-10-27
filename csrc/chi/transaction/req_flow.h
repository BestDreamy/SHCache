#pragma once
#include "../rnf_utils.h"
#include "../flit/req_flit.h"
#include "../../include/autoconfig.h"

inline reqflit_t chi_issue_ReadUnique_req(
    const uint32_t &srcID, const uint32_t &Addr, const uint32_t &Size
) {
    reqflit_t req = createReadUnique(config.HNId[0], srcID, 0, Addr, Size);

    RN_req_channel[srcID].push(req);
    
    return req;
}