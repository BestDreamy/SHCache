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
    reqflit_t() :
        RSVDC(0),
        TraceTag(0),
        ExpCompAck(0),
        Excl(0),
        LPID(0),
        SnpAttr(0),
        MemAttr(0),
        PCrdType(0),
        Order(0),
        AllowRetry(0),
        LikelyShared(0),
        NS(0),
        Addr(0),
        Size(0),
        Opcode(0),
        ReturnTxnID(0),
        StashNIDValid(0),
        StashNID_ReturnNID(0),
        TxnID(0),
        SrcID(0),
        TgtID(0),
        QoS(0) {}
};

constexpr size_t reqflit_width = 125;

constexpr size_t reqflit_RSVDC_width = 4;
constexpr size_t reqflit_TraceTag_width = 1;
constexpr size_t reqflit_ExpCompAck_width = 1;
constexpr size_t reqflit_Excl_width = 1;
constexpr size_t reqflit_LPID_width = 5;
constexpr size_t reqflit_SnpAttr_width = 1;
constexpr size_t reqflit_MemAttr_width = 4;
constexpr size_t reqflit_PCrdType_width = 4;
constexpr size_t reqflit_Order_width = 2;
constexpr size_t reqflit_AllowRetry_width = 1;
constexpr size_t reqflit_LikelyShared_width = 1;
constexpr size_t reqflit_NS_width = 1;
constexpr size_t reqflit_Addr_width = 48;
constexpr size_t reqflit_Size_width = 3;
constexpr size_t reqflit_Opcode_width = 6;
constexpr size_t reqflit_ReturnTxnID_width = 8;
constexpr size_t reqflit_StashNIDValid_width = 1;
constexpr size_t reqflit_StashNID_ReturnNID_width = 7;
constexpr size_t reqflit_TxnID_width = 8;
constexpr size_t reqflit_SrcID_width = 7;
constexpr size_t reqflit_TgtID_width = 7;
constexpr size_t reqflit_QoS_width = 4;


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
    rspflit_t() :
        TraceTag(0),
        PCrdType(0),
        DBID(0),
        FwdState_DataPull(0),
        Resp(0),
        RespErr(0),
        Opcode(0),
        TxnID(0),
        SrcID(0),
        TgtID(0),
        QoS(0) {}
};

constexpr size_t rspflit_width = 51;

constexpr size_t rspflit_TraceTag_width = 1;
constexpr size_t rspflit_PCrdType_width = 4;
constexpr size_t rspflit_DBID_width = 8;
constexpr size_t rspflit_FwdState_DataPull_width = 3;
constexpr size_t rspflit_Resp_width = 3;
constexpr size_t rspflit_RespErr_width = 2;
constexpr size_t rspflit_Opcode_width = 4;
constexpr size_t rspflit_TxnID_width = 8;
constexpr size_t rspflit_SrcID_width = 7;
constexpr size_t rspflit_TgtID_width = 7;
constexpr size_t rspflit_QoS_width = 4;


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
    datflit_t() :
        Data(0),
        BE(0),
        RSVDC(0),
        TraceTag(0),
        DataID(0),
        CCID(0),
        DBID(0),
        FwdState_DataPull(0),
        Resp(0),
        RespErr(0),
        Opcode(0),
        HomeNID(0),
        TxnID(0),
        SrcID(0),
        TgtID(0),
        QoS(0) {}
};

constexpr size_t datflit_width = 349;

constexpr size_t datflit_Data_width = 256;
constexpr size_t datflit_BE_width = 32;
constexpr size_t datflit_RSVDC_width = 4;
constexpr size_t datflit_TraceTag_width = 1;
constexpr size_t datflit_DataID_width = 2;
constexpr size_t datflit_CCID_width = 2;
constexpr size_t datflit_DBID_width = 8;
constexpr size_t datflit_FwdState_DataPull_width = 3;
constexpr size_t datflit_Resp_width = 3;
constexpr size_t datflit_RespErr_width = 2;
constexpr size_t datflit_Opcode_width = 3;
constexpr size_t datflit_HomeNID_width = 7;
constexpr size_t datflit_TxnID_width = 8;
constexpr size_t datflit_SrcID_width = 7;
constexpr size_t datflit_TgtID_width = 7;
constexpr size_t datflit_QoS_width = 4;


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
    snpflit_t() :
        TraceTag(0),
        RetToSrc(0),
        DoNotDataPull(0),
        NS(0),
        Addr(0),
        Opcode(0),
        FwdTxnID(0),
        FwdNID(0),
        TxnID(0),
        SrcID(0),
        QoS(0) {}
};

constexpr size_t snpflit_width = 91;

constexpr size_t snpflit_TraceTag_width = 1;
constexpr size_t snpflit_RetToSrc_width = 1;
constexpr size_t snpflit_DoNotDataPull_width = 1;
constexpr size_t snpflit_NS_width = 1;
constexpr size_t snpflit_Addr_width = 48;
constexpr size_t snpflit_Opcode_width = 5;
constexpr size_t snpflit_FwdTxnID_width = 8;
constexpr size_t snpflit_FwdNID_width = 7;
constexpr size_t snpflit_TxnID_width = 8;
constexpr size_t snpflit_SrcID_width = 7;
constexpr size_t snpflit_QoS_width = 4;

