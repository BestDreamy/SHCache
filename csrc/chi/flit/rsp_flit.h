#pragma once
#include "auto_flit.h"

enum CHI_RspFlit_Opcode {
    RespLCrdReturn = 0x0,
    SnpResp = 0x1,
    CompAck = 0x2, // Just used for RspFlit Opcode is CompAck
    RetryAck = 0x3,
    Comp = 0x4,
    CompDBIDResp = 0x5,
    DBIDResp = 0x6,
    PCrdGrant = 0x7,
    ReadReceipt = 0x8,
    SnpRespFwded = 0x9,
    TagMatch = 0xA,
    RespSepData = 0xB,
    Persist = 0xC,
    CompPersist = 0xD,
    DBIDRespOrd = 0xE
};

// Just used for RspFlit Opcode is CompAck
enum CHI_RspFlit_Resp {
    CompAck_I = 0,
    CompAck_UC = 1,
    CompAck_SC = 2,
    CompAck_UD_PD = 6,
    CompAck_SD_PD = 7,
};

inline rspflit_t createCompAck(
    const int &TgtID,
    const int &SrcID,
    const int &TxnID,
    const int &Resp,
    const int &DBID
) {
    rspflit_t flit;
    flit.TgtID = TgtID;
    flit.SrcID = SrcID;
    flit.TxnID = TxnID;
    flit.Opcode = CompAck;
    flit.Resp = Resp;
    flit.DBID = DBID;
    return flit;
}

inline rspflit_t createCompAck(
    const datflit_t &data
) {
    int TgtID = data.HomeNID;
    int SrcID = data.TgtID;
    int TxnID = data.DBID;
    int Resp = data.Resp; // Use the response from the data flit
    int DBID = 0; // Arbitrary value, can be set as needed
    return createCompAck(TgtID, SrcID, TxnID, Resp, DBID);
}
