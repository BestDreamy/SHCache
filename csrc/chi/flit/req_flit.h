#pragma once
#include "auto_flit.h"
#include <iostream>
#include "../../include/autoconfig.h"

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
    const int &SrcID, const uint32_t &Addr, const uint32_t &Size=4
) {
    reqflit_t flit;
    flit.TgtID = config.HNId[0];
    flit.SrcID = SrcID;
    flit.TxnID = 0;
    flit.Opcode = ReadUnique;
    flit.Addr = Addr;
    flit.Size = Size;
    flit.ExpCompAck = 1;

    return flit;
}

inline reqflit_t createReadNoSnp(
    const int &SrcID, const uint32_t &Addr, const uint32_t &Size=4
) {
    reqflit_t flit;
    flit.TgtID = config.HNId[0];
    flit.SrcID = SrcID;
    flit.TxnID = 0;
    flit.Opcode = ReadNoSnp;
    flit.Addr = Addr;
    flit.Size = Size;
    flit.ExpCompAck = 1;

    return flit;
}

inline void printReqFlit(const reqflit_t &req) {
    std::cout << "================ Req Flit ================" << std::endl;
    std::cout << "reqflit_t:" << std::endl;
    std::cout << "  RSVDC              : " << static_cast<unsigned>(req.RSVDC) << std::endl;
    std::cout << "  TraceTag           : " << static_cast<unsigned>(req.TraceTag) << std::endl;
    std::cout << "  ExpCompAck         : " << static_cast<unsigned>(req.ExpCompAck) << std::endl;
    std::cout << "  Excl               : " << static_cast<unsigned>(req.Excl) << std::endl;
    std::cout << "  LPID               : " << static_cast<unsigned>(req.LPID) << std::endl;
    std::cout << "  SnpAttr            : " << static_cast<unsigned>(req.SnpAttr) << std::endl;
    std::cout << "  MemAttr            : " << static_cast<unsigned>(req.MemAttr) << std::endl;
    std::cout << "  PCrdType           : " << static_cast<unsigned>(req.PCrdType) << std::endl;
    std::cout << "  Order              : " << static_cast<unsigned>(req.Order) << std::endl;
    std::cout << "  AllowRetry         : " << static_cast<unsigned>(req.AllowRetry) << std::endl;
    std::cout << "  LikelyShared       : " << static_cast<unsigned>(req.LikelyShared) << std::endl;
    std::cout << "  NS                 : " << static_cast<unsigned>(req.NS) << std::endl;
    std::cout << "  Addr               : " << static_cast<uint64_t>(req.Addr) << std::endl;
    std::cout << "  Size               : " << static_cast<unsigned>(req.Size) << std::endl;
    std::cout << "  Opcode             : " << static_cast<unsigned>(req.Opcode) << std::endl;
    std::cout << "  ReturnTxnID        : " << static_cast<unsigned>(req.ReturnTxnID) << std::endl;
    std::cout << "  StashNIDValid      : " << static_cast<unsigned>(req.StashNIDValid) << std::endl;
    std::cout << "  StashNID_ReturnNID : " << static_cast<unsigned>(req.StashNID_ReturnNID) << std::endl;
    std::cout << "  TxnID              : " << static_cast<unsigned>(req.TxnID) << std::endl;
    std::cout << "  SrcID              : " << static_cast<unsigned>(req.SrcID) << std::endl;
    std::cout << "  TgtID              : " << static_cast<unsigned>(req.TgtID) << std::endl;
    std::cout << "  QoS                : " << static_cast<unsigned>(req.QoS) << std::endl;
    std::cout << "============================================" << std::endl;
}