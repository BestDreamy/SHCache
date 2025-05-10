`include "autoconfig.v"
`include "chi_flit.vh"

// Transmit request flit to the SNF
module hnf_txreq(
    output reqflit_t txreqflit, // txreq channel
    output logic     txreqflitv,
    output logic     txreqflitpend,
    input  logic     txreqlcrdv,

    input            clock,
    input            reset
);

    reg txreqflitv_q; // ensure L-credit for chi

    always @(posedge clock) begin: txreqflitv_ff
        if(reset == 1'b1)
            txreqflitv_q <= 1'b0;
        else if (txreqlcrdv & txreqflitpend)
            txreqflitv_q <= 1'b1;
        else
            txreqflitv_q <= 1'b0;
    end

    assign txreqflitv = txreqflitv_q;



    /*************************************************************/

    wire [5:0]  ldid               = txreqflit.ldid;
    wire [3:0]  SrcType            = txreqflit.SrcType;
    wire [3:0]  RSVDC              = txreqflit.RSVDC;
    wire        TraceTag           = txreqflit.TraceTag;
    wire        ExpCompAck         = txreqflit.ExpCompAck;
    wire        Excl               = txreqflit.Excl;
    wire [4:0]  LPID               = txreqflit.LPID;
    wire        SnpAttr            = txreqflit.SnpAttr;
    wire [3:0]  MemAttr            = txreqflit.MemAttr;
    wire [3:0]  PCrdType           = txreqflit.PCrdType;
    wire [1:0]  Order              = txreqflit.Order;
    wire        AllowRetry         = txreqflit.AllowRetry;
    wire        LikelyShared       = txreqflit.LikelyShared;
    wire        NS                 = txreqflit.NS;
    wire [47:0] Addr               = txreqflit.Addr;
    wire [2:0]  Size               = txreqflit.Size;
    wire [5:0]  Opcode             = txreqflit.Opcode;
    wire [7:0]  StashLPID          = txreqflit.StashLPID;
    wire        StashNIDValid      = txreqflit.StashNIDValid;
    wire [6:0]  StashNID_ReturnNID = txreqflit.StashNID_ReturnNID;
    wire [7:0]  TxnID              = txreqflit.TxnID;
    wire [6:0]  SrcID              = txreqflit.SrcID;
    wire [6:0]  TgtID              = txreqflit.TgtID;
    wire [3:0]  QoS                = txreqflit.QoS;

    // always_comb begin
    //     assert(txreqflit_recv_en & (TgtID[CHI_MAX_SRCID_RANGE] == HNId[0][CHI_MAX_SRCID_RANGE])) else
    //         $error("HNF RXREQ: Received a request with TgtID != HNF");
    // end

endmodule