#ifndef FLIT_H
#define FLIT_H
#include <cstdint>
#include "../include/autoconfig.h"

#define OP_ReadNoSnp               0x4
#define OP_ReadUnique              0x7
#define OP_MakeUnique              0xC
#define OP_Evict                   0xD
#define OP_WriteBackFull           0x1B
#define OP_WriteNoSnpFull          0x1D
#define OP_ReadNotSharedDirty      0x26

struct reqFlit {
    uint32_t TgtID;
    uint32_t SrcID;
    uint32_t TxnID;
    uint32_t Opcode;
    uint32_t Addr;
    uint32_t Size;
    uint32_t ExpCompAck;
};

struct dataFlit {
    uint32_t TgtID;
    uint32_t SrcID;
    uint32_t TxnID;
    uint32_t HomeID;
    uint32_t Opcode;
    uint32_t Resp;
    uint32_t DBID;
    uint32_t CCID;
    uint32_t DataID;
    uint32_t Data[8];
    uint32_t BE[8];
};

struct snpFlit {
    uint32_t SrcID;
    uint32_t TxnID;
    uint32_t Opcode;
    uint32_t Addr;
};

struct rspFlit {
    uint32_t TgtID;
    uint32_t SrcID;
    uint32_t TxnID;
    uint32_t Opcode;
    uint32_t Resp;
    uint32_t DBID;
};

enum chiCompX {
    Comp_I = 0,
    Comp_SC = 1,
    Comp_UC = 2,
    Comp_UD_PD = 6,
    Comp_SD_PD = 7,
};

enum chiCompDataX {
    CompData_I = 0,
    CompData_SC = 1,
    CompData_UC = 2,
    CompData_UD_PD = 6,
    CompData_SD_PD = 7,
};

inline reqFlit createReadUnique(
    const int &SrcID, const uint32_t &Addr, const uint32_t &Size
) {
    reqFlit flit;
    flit.TgtID = config.HNId[0];
    flit.SrcID = SrcID;
    flit.TxnID = 0;
    flit.Opcode = OP_ReadUnique;
    flit.Addr = Addr;
    flit.Size = Size;
    flit.ExpCompAck = 1;

    return flit;
}

#endif
