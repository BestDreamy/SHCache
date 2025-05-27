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
    reqflit_t() noexcept
        : RSVDC(uint8_t()),
          TraceTag(uint8_t()),
          ExpCompAck(uint8_t()),
          Excl(uint8_t()),
          LPID(uint8_t()),
          SnpAttr(uint8_t()),
          MemAttr(uint8_t()),
          PCrdType(uint8_t()),
          Order(uint8_t()),
          AllowRetry(uint8_t()),
          LikelyShared(uint8_t()),
          NS(uint8_t()),
          Addr(uint64_t()),
          Size(uint8_t()),
          Opcode(uint8_t()),
          ReturnTxnID(uint8_t()),
          StashNIDValid(uint8_t()),
          StashNID_ReturnNID(uint8_t()),
          TxnID(uint8_t()),
          SrcID(uint8_t()),
          TgtID(uint8_t()),
          QoS(uint8_t()) {}
    reqflit_t(const reqflit_t& other) noexcept {
        RSVDC = other.RSVDC;
        TraceTag = other.TraceTag;
        ExpCompAck = other.ExpCompAck;
        Excl = other.Excl;
        LPID = other.LPID;
        SnpAttr = other.SnpAttr;
        MemAttr = other.MemAttr;
        PCrdType = other.PCrdType;
        Order = other.Order;
        AllowRetry = other.AllowRetry;
        LikelyShared = other.LikelyShared;
        NS = other.NS;
        Addr = other.Addr;
        Size = other.Size;
        Opcode = other.Opcode;
        ReturnTxnID = other.ReturnTxnID;
        StashNIDValid = other.StashNIDValid;
        StashNID_ReturnNID = other.StashNID_ReturnNID;
        TxnID = other.TxnID;
        SrcID = other.SrcID;
        TgtID = other.TgtID;
        QoS = other.QoS;
    }
    reqflit_t& operator=(const reqflit_t& other) noexcept {
        if (this != &other) {
            RSVDC = other.RSVDC;
            TraceTag = other.TraceTag;
            ExpCompAck = other.ExpCompAck;
            Excl = other.Excl;
            LPID = other.LPID;
            SnpAttr = other.SnpAttr;
            MemAttr = other.MemAttr;
            PCrdType = other.PCrdType;
            Order = other.Order;
            AllowRetry = other.AllowRetry;
            LikelyShared = other.LikelyShared;
            NS = other.NS;
            Addr = other.Addr;
            Size = other.Size;
            Opcode = other.Opcode;
            ReturnTxnID = other.ReturnTxnID;
            StashNIDValid = other.StashNIDValid;
            StashNID_ReturnNID = other.StashNID_ReturnNID;
            TxnID = other.TxnID;
            SrcID = other.SrcID;
            TgtID = other.TgtID;
            QoS = other.QoS;
        }
        return *this;
    }
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
    rspflit_t() noexcept
        : TraceTag(uint8_t()),
          PCrdType(uint8_t()),
          DBID(uint8_t()),
          FwdState_DataPull(uint8_t()),
          Resp(uint8_t()),
          RespErr(uint8_t()),
          Opcode(uint8_t()),
          TxnID(uint8_t()),
          SrcID(uint8_t()),
          TgtID(uint8_t()),
          QoS(uint8_t()) {}
    rspflit_t(const rspflit_t& other) noexcept {
        TraceTag = other.TraceTag;
        PCrdType = other.PCrdType;
        DBID = other.DBID;
        FwdState_DataPull = other.FwdState_DataPull;
        Resp = other.Resp;
        RespErr = other.RespErr;
        Opcode = other.Opcode;
        TxnID = other.TxnID;
        SrcID = other.SrcID;
        TgtID = other.TgtID;
        QoS = other.QoS;
    }
    rspflit_t& operator=(const rspflit_t& other) noexcept {
        if (this != &other) {
            TraceTag = other.TraceTag;
            PCrdType = other.PCrdType;
            DBID = other.DBID;
            FwdState_DataPull = other.FwdState_DataPull;
            Resp = other.Resp;
            RespErr = other.RespErr;
            Opcode = other.Opcode;
            TxnID = other.TxnID;
            SrcID = other.SrcID;
            TgtID = other.TgtID;
            QoS = other.QoS;
        }
        return *this;
    }
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
    datflit_t() noexcept
        : Data(std::bitset<256>()),
          BE(uint32_t()),
          RSVDC(uint8_t()),
          TraceTag(uint8_t()),
          DataID(uint8_t()),
          CCID(uint8_t()),
          DBID(uint8_t()),
          FwdState_DataPull(uint8_t()),
          Resp(uint8_t()),
          RespErr(uint8_t()),
          Opcode(uint8_t()),
          HomeNID(uint8_t()),
          TxnID(uint8_t()),
          SrcID(uint8_t()),
          TgtID(uint8_t()),
          QoS(uint8_t()) {}
    datflit_t(const datflit_t& other) noexcept {
        Data = other.Data;
        BE = other.BE;
        RSVDC = other.RSVDC;
        TraceTag = other.TraceTag;
        DataID = other.DataID;
        CCID = other.CCID;
        DBID = other.DBID;
        FwdState_DataPull = other.FwdState_DataPull;
        Resp = other.Resp;
        RespErr = other.RespErr;
        Opcode = other.Opcode;
        HomeNID = other.HomeNID;
        TxnID = other.TxnID;
        SrcID = other.SrcID;
        TgtID = other.TgtID;
        QoS = other.QoS;
    }
    datflit_t& operator=(const datflit_t& other) noexcept {
        if (this != &other) {
            Data = other.Data;
            BE = other.BE;
            RSVDC = other.RSVDC;
            TraceTag = other.TraceTag;
            DataID = other.DataID;
            CCID = other.CCID;
            DBID = other.DBID;
            FwdState_DataPull = other.FwdState_DataPull;
            Resp = other.Resp;
            RespErr = other.RespErr;
            Opcode = other.Opcode;
            HomeNID = other.HomeNID;
            TxnID = other.TxnID;
            SrcID = other.SrcID;
            TgtID = other.TgtID;
            QoS = other.QoS;
        }
        return *this;
    }
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
    snpflit_t() noexcept
        : TraceTag(uint8_t()),
          RetToSrc(uint8_t()),
          DoNotDataPull(uint8_t()),
          NS(uint8_t()),
          Addr(uint64_t()),
          Opcode(uint8_t()),
          FwdTxnID(uint8_t()),
          FwdNID(uint8_t()),
          TxnID(uint8_t()),
          SrcID(uint8_t()),
          QoS(uint8_t()) {}
    snpflit_t(const snpflit_t& other) noexcept {
        TraceTag = other.TraceTag;
        RetToSrc = other.RetToSrc;
        DoNotDataPull = other.DoNotDataPull;
        NS = other.NS;
        Addr = other.Addr;
        Opcode = other.Opcode;
        FwdTxnID = other.FwdTxnID;
        FwdNID = other.FwdNID;
        TxnID = other.TxnID;
        SrcID = other.SrcID;
        QoS = other.QoS;
    }
    snpflit_t& operator=(const snpflit_t& other) noexcept {
        if (this != &other) {
            TraceTag = other.TraceTag;
            RetToSrc = other.RetToSrc;
            DoNotDataPull = other.DoNotDataPull;
            NS = other.NS;
            Addr = other.Addr;
            Opcode = other.Opcode;
            FwdTxnID = other.FwdTxnID;
            FwdNID = other.FwdNID;
            TxnID = other.TxnID;
            SrcID = other.SrcID;
            QoS = other.QoS;
        }
        return *this;
    }
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

