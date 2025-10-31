#pragma once
#include "auto_flit.h"
#include <iostream>

enum CHI_ReqFlit_Opcode {
    ReadNoSnp          = 0x4,
    ReadUnique         = 0x7,
    MakeUnique         = 0xC,
    Evict              = 0xD,
    WriteBackFull      = 0x1B,
    WriteNoSnpFull     = 0x1D,
    ReadNotSharedDirty = 0x26
};

enum CHI_ReqFlit_Size {
    Size_1B = 0,
    Size_2B = 1,
    Size_4B = 2,
    Size_8B = 3,
    Size_16B = 4,
    Size_32B = 5,
    Size_64B = 6,
    Size_128B = 7
};

inline reqflit_t createReadUnique(
    const int &TgtID,
    const int &SrcID,
    const int &TxnID,
    const uint64_t &Addr, 
    const uint32_t &Size=4
) {
    reqflit_t flit;
    flit.TgtID = TgtID;
    flit.SrcID = SrcID;
    flit.TxnID = TxnID;
    flit.Opcode = ReadUnique;
    flit.Addr = Addr;
    flit.Size = Size;
    flit.ExpCompAck = 1;

    return flit;
}

inline void printReqFlit(const reqflit_t &req) {
    std::string log = "ResFlit: TgtID=" + std::to_string(req.TgtID) +
        " SrcID=" + std::to_string(req.SrcID) +
        " TxnID=" + std::to_string(req.TxnID) +
        " Opcode=" + std::to_string(req.Opcode) +
        " Addr=" + std::to_string(req.Addr) +
        " Size=" + std::to_string(req.Size) +
        " }";
    devLog("%s", log.c_str());
}