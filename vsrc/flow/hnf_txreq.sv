`include "autoconfig.v"
`include "chi_flit.vh"

// Transmit request flit to the SNF
module hnf_txreq(
    output reqflit_t txreqflit, // txreq channel
    output logic     txreqflitv,
    output logic     txreqflitpend,
    input  logic     txreqlcrdv,

    input  reqflit_t rxreq_pocq_first_entry,
    input  logic     rxreq_pocq_first_entry_v,
    input            clock,
    input            reset
);

    reg txreqflitv_q;
    
    always @(posedge clock) begin: txreqflitv_ff
        if(reset == 1'b1)
            txreqflitv_q <= 1'b0;
        else if (txreqflitpend == 1'b1)
            txreqflitv_q <= 1'b1;
        else
            txreqflitv_q <= 1'b0;
    end

    wire txreqflit_recv_en = txreqflitv & txreqflitv_q;

    parameter int rxreq_pocq_size = numCreditsForHNReq[0];

    wire rxreq_pocq_is_full;
    wire rxreq_pocq_is_empty;
    
    sfifo #(
        .WIDTH($bits(reqflit_t)),
        .DEPTH(rxreq_pocq_size)
    ) rxreq_pocq (
        .clk(clock),
        .rst_n(~reset),
        .winc(txreqflit_recv_en),
        .rinc(/* 读取使能信号 */),
        .wdata(txreqflit),
        // output
        .wfull(rxreq_pocq_is_full),
        .rempty(rxreq_pocq_is_empty),
        .rdata(rxreq_pocq_first_entry)
    );

    assign rxreq_pocq_first_entry_v = rxreq_pocq_is_empty ? 1'b0 : 1'b1;

    assign rxreqlcrdv = rxreq_pocq_is_full ? 1'b0 : 1'b1;

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

    always_comb begin
        assert(txreqflit_recv_en & (TgtID[CHI_MAX_SRCID_RANGE] == HNId[0][CHI_MAX_SRCID_RANGE])) else
            $error("HNF RXREQ: Received a request with TgtID != HNF");
    end

endmodule