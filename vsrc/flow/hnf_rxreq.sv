`include "autoconfig.v"
`include "chi_flit.vh"

// Receive request flit from the RNF
module hnf_rxreq(
    input  reqflit_t rxreqflit, // rxreq channel
    input  logic     rxreqflitv,
    input  logic     rxreqflitpend,
    output logic     rxreqlcrdv,

    output reqflit_t rxreq_posq_first_entry,
    output logic     rxreq_posq_first_entry_v,
    input  logic     rxreq_posq_first_entry_dis,
    input            clock,
    input            reset
);

    reg rxreqflitv_q; // ensure L-credit for chi
    
    always @(posedge clock) begin: rxreqflitv_ff
        if(reset == 1'b1)
            rxreqflitv_q <= 1'b0;
        else if (rxreqflitpend == 1'b1)
            rxreqflitv_q <= 1'b1;
        else
            rxreqflitv_q <= 1'b0;
    end

    wire rxreqflit_recv_en = rxreqflitv & rxreqflitv_q;

    parameter int rxreq_posq_size = numCreditsForHNReq[0];

    wire rxreq_posq_is_full;
    wire rxreq_posq_is_empty;
    
    sfifo #(
        .WIDTH($bits(reqflit_t)),
        .DEPTH(rxreq_posq_size)
    ) rxreq_posq (
        .clk(clock),
        .rst_n(~reset),
        .winc(rxreqflit_recv_en),
        .rinc(rxreq_posq_first_entry_dis),
        .wdata(rxreqflit),
        // output
        .wfull(rxreq_posq_is_full),
        .rempty(rxreq_posq_is_empty),
        .rdata(rxreq_posq_first_entry)
    );

    assign rxreq_posq_first_entry_v = rxreq_posq_is_empty ? 1'b0 : 1'b1;

    assign rxreqlcrdv = rxreq_posq_is_full ? 1'b0 : 1'b1;

    /*************************************************************/

    wire [5:0]  ldid               = rxreqflit.ldid;
    wire [3:0]  SrcType            = rxreqflit.SrcType;
    wire [3:0]  RSVDC              = rxreqflit.RSVDC;
    wire        TraceTag           = rxreqflit.TraceTag;
    wire        ExpCompAck         = rxreqflit.ExpCompAck;
    wire        Excl               = rxreqflit.Excl;
    wire [4:0]  LPID               = rxreqflit.LPID;
    wire        SnpAttr            = rxreqflit.SnpAttr;
    wire [3:0]  MemAttr            = rxreqflit.MemAttr;
    wire [3:0]  PCrdType           = rxreqflit.PCrdType;
    wire [1:0]  Order              = rxreqflit.Order;
    wire        AllowRetry         = rxreqflit.AllowRetry;
    wire        LikelyShared       = rxreqflit.LikelyShared;
    wire        NS                 = rxreqflit.NS;
    wire [47:0] Addr               = rxreqflit.Addr;
    wire [2:0]  Size               = rxreqflit.Size;
    wire [5:0]  Opcode             = rxreqflit.Opcode;
    // wire [7:0]  StashLPID          = rxreqflit.StashLPID;
    // wire        StashNIDValid      = rxreqflit.StashNIDValid;
    wire [6:0]  StashNID_ReturnNID = rxreqflit.StashNID_ReturnNID;
    wire [7:0]  TxnID              = rxreqflit.TxnID;
    wire [6:0]  SrcID              = rxreqflit.SrcID;
    wire [6:0]  TgtID              = rxreqflit.TgtID;
    wire [3:0]  QoS                = rxreqflit.QoS;

    always_comb begin
        assert(rxreqflit_recv_en & (TgtID[`CHI_MAX_SRCID_RANGE] == HNId[0][`CHI_MAX_SRCID_RANGE])) else
            $error("HNF RXREQ: Received a request with TgtID != HNF");
    end

endmodule