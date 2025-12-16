#pragma once
#include <queue>
#include "flit/auto_flit.h"
#include "../include/autoconfig.h"

extern std::queue<reqflit_t> RN_req_channel[NUMCORES];
extern std::queue<datflit_t> RN_dat_channel[NUMCORES];
extern std::queue<rspflit_t> RN_rsp_channel[NUMCORES];