#include "transaction.h"
#include "../mem.h"

extern "C" void chi_recv_ReadNoSnp_req(reqflit_t req, datflit_t *data) {
    data = new datflit_t;

    uint32_t mem_data = 0;
    mem.read_memory(static_cast<uint32_t>(req.Addr & 0xffffffff), mem_data);

    data->TraceTag          = req.TraceTag;
    data->DBID              = static_cast<uint8_t>(mem_data & 0xff);
    data->FwdState_DataPull = 0;
    data->Resp              = 0; 
    data->RespErr           = 0;
    data->Opcode            = 4;
    data->HomeNID           = 0;
    data->TxnID             = req.TxnID;
    data->SrcID             = req.SrcID;
    data->TgtID             = req.TgtID;
    data->QoS               = req.QoS;

    return ;
}