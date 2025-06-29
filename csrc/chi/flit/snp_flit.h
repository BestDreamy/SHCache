#pragma once
#include "auto_flit.h"

enum CHI_SnpFlit_Opcode {
    SnpLCrdReturn = 0x0,
    SnpShared = 0x1,
    SnpClean = 0x2,
    SnpOnce = 0x3,
    SnpNotSharedDirty = 0x4,
    SnpUniqueStash = 0x5,
    SnpMakeInvalidStash = 0x6,
    SnpUnique = 0x7,
    SnpCleanShared = 0x8,
    SnpCleanInvalid = 0x9,
    SnpMakeInvalid = 0xA,
    SnpStashUnique = 0xB,
    SnpStashShared = 0xC,
    SnpDVMOp = 0xD
};

inline snpflit_t createSnpUniqueFwd(
    const int &SrcID,
    const int &TxnID,
    const int &FwdNID,
    const int &FwdTxnID,
    const uint64_t &Addr, 
    const int RetToSrc
) {
    snpflit_t flit;
    flit.RetToSrc = 0;

    return flit;
}