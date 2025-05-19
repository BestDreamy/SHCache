#pragma once
#include <cstdint>
#include <bitset>


struct reqflit_t {
    uint8_t RSVDC;
    uint8_t TraceTag;
    uint8_t ExpCompAck;
    uint8_t Excl;
    uint8_t LPID;
    uint8_t SnpAttr;
    uint8_t MemAttr;
    uint8_t PCrdType;
    uint8_t Order;
    uint8_t AllowRetry;
    uint8_t LikelyShared;
    uint8_t NS;
    uint64_t Addr;
    uint8_t Size;
    uint8_t Opcode;
    uint8_t ReturnTxnID;
    uint8_t StashNIDValid;
    uint8_t StashNID_ReturnNID;
    uint8_t TxnID;
    uint8_t SrcID;
    uint8_t TgtID;
    uint8_t QoS;
};

struct rspflit_t {
    uint8_t TraceTag;
    uint8_t PCrdType;
    uint8_t DBID;
    uint8_t FwdState_DataPull;
    uint8_t Resp;
    uint8_t RespErr;
    uint8_t Opcode;
    uint8_t TxnID;
    uint8_t SrcID;
    uint8_t TgtID;
    uint8_t QoS;
};

struct datflit_t {
    std::bitset<256> Data;
    uint32_t BE;
    uint8_t RSVDC;
    uint8_t TraceTag;
    uint8_t DataID;
    uint8_t CCID;
    uint8_t DBID;
    uint8_t FwdState_DataPull;
    uint8_t Resp;
    uint8_t RespErr;
    uint8_t Opcode;
    uint8_t HomeNID;
    uint8_t TxnID;
    uint8_t SrcID;
    uint8_t TgtID;
    uint8_t QoS;
};

struct snpflit_t {
    uint8_t TraceTag;
    uint8_t RetToSrc;
    uint8_t DoNotDataPull;
    uint8_t NS;
    uint64_t Addr;
    uint8_t Opcode;
    uint8_t FwdTxnID;
    uint8_t FwdNID;
    uint8_t TxnID;
    uint8_t SrcID;
    uint8_t QoS;
};
