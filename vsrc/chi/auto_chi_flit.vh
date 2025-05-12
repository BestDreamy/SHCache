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
