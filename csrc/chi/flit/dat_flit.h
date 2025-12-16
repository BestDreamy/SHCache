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

// inline datflit_t createCompData(
//     const int &TgtID,
//     const int &SrcID,
//     const int &TxnID,
//     const int &HomeNID,
//     const int &Resp,
//     const int &DBID,
//     const int &CCID,
//     const int &DataID,
//     const int &Data[8],
//     const int &BE[8]
// ) {
//     datflit_t flit;
//     flit.TgtID = TgtID;
//     flit.SrcID = SrcID;
//     flit.TxnID = TxnID;
//     flit.Addr = Addr;
//     flit.Size = Size;
//     flit.Opcode = CompData;
//     flit.Resp = CompData_UC; // Default response
//     flit.BE = 0; // Byte Enable, can be set later

//     return flit;
// }

inline datflit_t createCompData_UC(const reqflit_t &req);

inline void printDatFlit(const datflit_t &flit) {
    std::string log = "DataFlit: ";
    log += "TgtID=" + std::to_string(flit.TgtID) +
        " SrcID=" + std::to_string(flit.SrcID) +
        " TxnID=" + std::to_string(flit.TxnID) +
        " HomeNID=" + std::to_string(flit.HomeNID) +
        " Opcode=" + std::to_string(flit.Opcode) +
        " Resp=" + std::to_string(flit.Resp);
    
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for (int i = 7; i >= 0; i--) {
        uint32_t val = 0;
        for (int b = 0; b < 32; ++b) {
            val |= (flit.Data[i * 32 + b] << b);
        }
        oss << "Word[" << i << "] = 0x"
            << std::setw(8) << val << "\n";
    }

    oss << "BE      : 0x"
        << std::setw(8) << static_cast<uint32_t>(flit.BE) << "\n";

    log += "\n" + oss.str();
    devLog("%s", log.c_str());
}