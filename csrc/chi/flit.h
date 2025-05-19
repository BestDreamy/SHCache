#ifndef FLIT_H
#define FLIT_H
#include <cstdint>
#include "../include/autoconfig.h"
#include "auto_flit.h"
#include "../mem.h"
#include "../include/dbg.h"
#include <iostream>

inline void printReqFlit(const reqflit_t &req) {
    puts("*******************cpp*******************");
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
}

// struct reqFlit {
    //     uint32_t TgtID;
    //     uint32_t SrcID;
    //     uint32_t TxnID;
    //     uint32_t Opcode;
    //     uint32_t Addr;
    //     uint32_t Size;
    //     uint32_t ExpCompAck;
    // };
    
inline void printDataFlit(const datflit_t &flit) {
    puts("*******************cpp*******************");
    puts("*******************cpp*******************");
    std::cout << "dataFlit:" << std::endl;
    std::cout << "  TgtID  : " << flit.TgtID.to_ulong() << std::endl;
    std::cout << "  SrcID  : " << flit.SrcID.to_ulong() << std::endl;
    std::cout << "  TxnID  : " << flit.TxnID.to_ulong() << std::endl;
    std::cout << "  HomeID : " << flit.HomeNID.to_ulong() << std::endl;
    std::cout << "  Opcode : " << flit.Opcode.to_ulong() << std::endl;
    std::cout << "  Resp   : " << flit.Resp.to_ulong() << std::endl;
    std::cout << "  DBID   : " << flit.DBID.to_ulong() << std::endl;
    std::cout << "  CCID   : " << flit.CCID.to_ulong() << std::endl;
    std::cout << "  DataID : " << flit.DataID.to_ulong() << std::endl;
}
// struct dataFlit {
//     uint32_t TgtID;
//     uint32_t SrcID;
//     uint32_t TxnID;
//     uint32_t HomeID;
//     uint32_t Opcode;
//     uint32_t Resp;
//     uint32_t DBID;
//     uint32_t CCID;
//     uint32_t DataID;
//     uint32_t Data[8];
//     uint32_t BE[8];
// };

// struct snpFlit {
//     uint32_t SrcID;
//     uint32_t TxnID;
//     uint32_t Opcode;
//     uint32_t Addr;
// };

// struct rspFlit {
//     uint32_t TgtID;
//     uint32_t SrcID;
//     uint32_t TxnID;
//     uint32_t Opcode;
//     uint32_t Resp;
//     uint32_t DBID;
// };

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

enum CHI_DataFlit_Opcode {
    DataLCrdReturn       = 0x0,
    SnpRespData          = 0x1,
    CopyBackWrData       = 0x2,
    NonCopyBackWrData    = 0x3,
    CompData             = 0x4,
    SnpRespDataPtl       = 0x5,
    SnpRespDataFwded     = 0x6,
    WriterDataCancel     = 0x7
};

// enum chiCompX {
//     Comp_I = 0,
//     Comp_SC = 1,
//     Comp_UC = 2,
//     Comp_UD_PD = 6,
//     Comp_SD_PD = 7,
// };

enum CHI_DataFlit_Resp {
    CompData_I = 0,
    CompData_SC = 1,
    CompData_UC = 2,
    CompData_UD_PD = 6,
    CompData_SD_PD = 7,
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

inline datflit_t createCompData_UC(const reqflit_t &req) {
    datflit_t flit;

    uint8_t size = req.Size; // Suppose Size=4
    printReqFlit(req);
    Assert(req.Addr % 4 == 0, "Address must be aligned to 4 bytes");
    Exit(size < 6, "Size must be less than 6");
    
    int num_bytes = 1 << size; // 16 bytes
    int num_words = num_bytes / 4;
    uint32_t word_data = 0;
    for (int i = 0; i < num_words; i++) {
        uint64_t word_addr = static_cast<uint64_t>((req.Addr & ~0x3UL) + i * 4);
        mem.read_memory(word_addr, word_data);
        
        // DataFlit include 256 bits (32 bytes)
        // Each size < 256 bits req just use 1 DataFlit
        for (int bit = 0; bit < 32; bit++) {
            bool bit_val = ((word_data >> bit) & 1) != 0;
            flit.Data.set(i * 32 + bit, bit_val);
        }
        
        flit.BE |= (0xF << (i * 4));
    }
    flit.DataID            = 0;
    flit.CCID              = 0;
    flit.DBID              = req.TxnID;
    flit.Resp              = CompData_UC;
    flit.Opcode            = CompData;
    flit.HomeNID           = req.SrcID;
    flit.TxnID             = req.ReturnTxnID;
    flit.SrcID             = req.TgtID;
    flit.TgtID             = req.StashNID_ReturnNID;

    return flit;
}

#endif
