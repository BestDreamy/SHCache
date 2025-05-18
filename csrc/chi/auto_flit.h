#pragma once
#include <cstdint>
#include <bitset>


struct reqflit_t {
    std::bitset<4> RSVDC;
    std::bitset<1> TraceTag;
    std::bitset<1> ExpCompAck;
    std::bitset<1> Excl;
    std::bitset<5> LPID;
    std::bitset<1> SnpAttr;
    std::bitset<4> MemAttr;
    std::bitset<4> PCrdType;
    std::bitset<2> Order;
    std::bitset<1> AllowRetry;
    std::bitset<1> LikelyShared;
    std::bitset<1> NS;
    std::bitset<48> Addr;
    std::bitset<3> Size;
    std::bitset<6> Opcode;
    std::bitset<8> ReturnTxnID;
    std::bitset<1> StashNIDValid;
    std::bitset<7> StashNID_ReturnNID;
    std::bitset<8> TxnID;
    std::bitset<7> SrcID;
    std::bitset<7> TgtID;
    std::bitset<4> QoS;
};

struct rspflit_t {
    std::bitset<1> TraceTag;
    std::bitset<4> PCrdType;
    std::bitset<8> DBID;
    std::bitset<3> FwdState_DataPull;
    std::bitset<3> Resp;
    std::bitset<2> RespErr;
    std::bitset<4> Opcode;
    std::bitset<8> TxnID;
    std::bitset<7> SrcID;
    std::bitset<7> TgtID;
    std::bitset<4> QoS;
};

struct datflit_t {
    std::bitset<256> Data;
    std::bitset<32> BE;
    std::bitset<4> RSVDC;
    std::bitset<1> TraceTag;
    std::bitset<2> DataID;
    std::bitset<2> CCID;
    std::bitset<8> DBID;
    std::bitset<3> FwdState_DataPull;
    std::bitset<3> Resp;
    std::bitset<2> RespErr;
    std::bitset<3> Opcode;
    std::bitset<7> HomeNID;
    std::bitset<8> TxnID;
    std::bitset<7> SrcID;
    std::bitset<7> TgtID;
    std::bitset<4> QoS;
};

struct snpflit_t {
    std::bitset<1> TraceTag;
    std::bitset<1> RetToSrc;
    std::bitset<1> DoNotDataPull;
    std::bitset<1> NS;
    std::bitset<48> Addr;
    std::bitset<5> Opcode;
    std::bitset<8> FwdTxnID;
    std::bitset<7> FwdNID;
    std::bitset<8> TxnID;
    std::bitset<7> SrcID;
    std::bitset<4> QoS;
};
