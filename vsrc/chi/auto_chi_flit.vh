typedef struct packed {
    logic [3:0] RSVDC;
    logic TraceTag;
    logic ExpCompAck;
    logic Excl;
    logic [4:0] LPID;
    logic SnpAttr;
    logic [3:0] MemAttr;
    logic [3:0] PCrdType;
    logic [1:0] Order;
    logic AllowRetry;
    logic LikelyShared;
    logic NS;
    logic [47:0] Addr;
    logic [2:0] Size;
    logic [5:0] Opcode;
    logic [7:0] ReturnTxnID;
    logic StashNIDValid;
    logic [6:0] StashNID_ReturnNID;
    logic [7:0] TxnID;
    logic [6:0] SrcID;
    logic [6:0] TgtID;
    logic [3:0] QoS;
} reqflit_t;

parameter int reqflit_width = 125;

parameter int reqflit_RSVDC_width = 4;
parameter int reqflit_TraceTag_width = 1;
parameter int reqflit_ExpCompAck_width = 1;
parameter int reqflit_Excl_width = 1;
parameter int reqflit_LPID_width = 5;
parameter int reqflit_SnpAttr_width = 1;
parameter int reqflit_MemAttr_width = 4;
parameter int reqflit_PCrdType_width = 4;
parameter int reqflit_Order_width = 2;
parameter int reqflit_AllowRetry_width = 1;
parameter int reqflit_LikelyShared_width = 1;
parameter int reqflit_NS_width = 1;
parameter int reqflit_Addr_width = 48;
parameter int reqflit_Size_width = 3;
parameter int reqflit_Opcode_width = 6;
parameter int reqflit_ReturnTxnID_width = 8;
parameter int reqflit_StashNIDValid_width = 1;
parameter int reqflit_StashNID_ReturnNID_width = 7;
parameter int reqflit_TxnID_width = 8;
parameter int reqflit_SrcID_width = 7;
parameter int reqflit_TgtID_width = 7;
parameter int reqflit_QoS_width = 4;


typedef struct packed {
    logic TraceTag;
    logic [3:0] PCrdType;
    logic [7:0] DBID;
    logic [2:0] FwdState_DataPull;
    logic [2:0] Resp;
    logic [1:0] RespErr;
    logic [3:0] Opcode;
    logic [7:0] TxnID;
    logic [6:0] SrcID;
    logic [6:0] TgtID;
    logic [3:0] QoS;
} rspflit_t;

parameter int rspflit_width = 51;

parameter int rspflit_TraceTag_width = 1;
parameter int rspflit_PCrdType_width = 4;
parameter int rspflit_DBID_width = 8;
parameter int rspflit_FwdState_DataPull_width = 3;
parameter int rspflit_Resp_width = 3;
parameter int rspflit_RespErr_width = 2;
parameter int rspflit_Opcode_width = 4;
parameter int rspflit_TxnID_width = 8;
parameter int rspflit_SrcID_width = 7;
parameter int rspflit_TgtID_width = 7;
parameter int rspflit_QoS_width = 4;


typedef struct packed {
    logic [255:0] Data;
    logic [31:0] BE;
    logic [3:0] RSVDC;
    logic TraceTag;
    logic [1:0] DataID;
    logic [1:0] CCID;
    logic [7:0] DBID;
    logic [2:0] FwdState_DataPull;
    logic [2:0] Resp;
    logic [1:0] RespErr;
    logic [2:0] Opcode;
    logic [6:0] HomeNID;
    logic [7:0] TxnID;
    logic [6:0] SrcID;
    logic [6:0] TgtID;
    logic [3:0] QoS;
} datflit_t;

parameter int datflit_width = 349;

parameter int datflit_Data_width = 256;
parameter int datflit_BE_width = 32;
parameter int datflit_RSVDC_width = 4;
parameter int datflit_TraceTag_width = 1;
parameter int datflit_DataID_width = 2;
parameter int datflit_CCID_width = 2;
parameter int datflit_DBID_width = 8;
parameter int datflit_FwdState_DataPull_width = 3;
parameter int datflit_Resp_width = 3;
parameter int datflit_RespErr_width = 2;
parameter int datflit_Opcode_width = 3;
parameter int datflit_HomeNID_width = 7;
parameter int datflit_TxnID_width = 8;
parameter int datflit_SrcID_width = 7;
parameter int datflit_TgtID_width = 7;
parameter int datflit_QoS_width = 4;


typedef struct packed {
    logic TraceTag;
    logic RetToSrc;
    logic DoNotDataPull;
    logic NS;
    logic [47:0] Addr;
    logic [4:0] Opcode;
    logic [7:0] FwdTxnID;
    logic [6:0] FwdNID;
    logic [7:0] TxnID;
    logic [6:0] SrcID;
    logic [3:0] QoS;
} snpflit_t;

parameter int snpflit_width = 91;

parameter int snpflit_TraceTag_width = 1;
parameter int snpflit_RetToSrc_width = 1;
parameter int snpflit_DoNotDataPull_width = 1;
parameter int snpflit_NS_width = 1;
parameter int snpflit_Addr_width = 48;
parameter int snpflit_Opcode_width = 5;
parameter int snpflit_FwdTxnID_width = 8;
parameter int snpflit_FwdNID_width = 7;
parameter int snpflit_TxnID_width = 8;
parameter int snpflit_SrcID_width = 7;
parameter int snpflit_QoS_width = 4;

