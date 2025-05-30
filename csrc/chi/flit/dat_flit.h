#pragma once
#include "auto_flit.h"
#include <iostream>
#include <iomanip>
#include "../../include/dbg.h"
#include "../../mem.h"

enum CHI_DataFlit_Opcode {
    DataLCrdReturn       = 0x0,
    SnpRespData          = 0x1,
    CopyBackWrData       = 0x2,
    NonCopyBackWrData    = 0x3,
    CompData             = 0x4,
    SnpRespDataPtl       = 0x5,
    SnpRespDataFwded     = 0x6,
    WriterDataCancel     = 0x7,
    DataSepResp          = 0xb,
    NCBWrDataCompAck     = 0xc
};

enum CHI_DataFlit_Resp {
    CompData_I = 0,
    CompData_UC = 1,
    CompData_SC = 2,
    CompData_UD_PD = 6,
    CompData_SD_PD = 7,
};

inline datflit_t createCompData_UC(const reqflit_t &req) {
    datflit_t flit;

    uint8_t size = req.Size; // Suppose Size=4
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

inline void printDataFlit(const datflit_t &flit) {
    std::cout << "================ DataFlit ================" << std::endl;
    std::cout << "Data:" << std::endl;
    for (int i = 7; i >= 0; i --) { // 256 bits / 32 = 8
        uint32_t val = 0;
        for (int b = 0; b < 32; ++b) {
            val |= (flit.Data[i * 32 + b] << b);
        }
        std::cout << "Word[" << i << "] = 0x" << std::hex << val << std::dec << std::endl;
    }
    std::cout << "BE       : 0x" << std::setw(8) << std::setfill('0') 
                << std::hex << flit.BE << std::dec << std::setfill(' ') << std::endl;
    std::cout << "RSVDC    : " << static_cast<unsigned>(flit.RSVDC) << std::endl;
    std::cout << "TraceTag : " << static_cast<unsigned>(flit.TraceTag) << std::endl;
    std::cout << "DataID   : " << static_cast<unsigned>(flit.DataID) << std::endl;
    std::cout << "CCID     : " << static_cast<unsigned>(flit.CCID) << std::endl;
    std::cout << "DBID     : " << static_cast<unsigned>(flit.DBID) << std::endl;
    std::cout << "FwdState_DataPull : " << static_cast<unsigned>(flit.FwdState_DataPull) << std::endl;
    std::cout << "Resp     : " << static_cast<unsigned>(flit.Resp) << std::endl;
    std::cout << "RespErr  : " << static_cast<unsigned>(flit.RespErr) << std::endl;
    std::cout << "Opcode   : " << static_cast<unsigned>(flit.Opcode) << std::endl;
    std::cout << "HomeNID  : " << static_cast<unsigned>(flit.HomeNID) << std::endl;
    std::cout << "TxnID    : " << static_cast<unsigned>(flit.TxnID) << std::endl;
    std::cout << "SrcID    : " << static_cast<unsigned>(flit.SrcID) << std::endl;
    std::cout << "TgtID    : " << static_cast<unsigned>(flit.TgtID) << std::endl;
    std::cout << "QoS      : " << static_cast<unsigned>(flit.QoS) << std::endl;
    std::cout << "============================================" << std::endl;
}