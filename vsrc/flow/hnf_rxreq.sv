
// Receive request flit from the RNF
module hnf_rxreq(
    input  reqflit_t RXREQFLIT, // rxreq channel
    input  logic     RXREQFLITV,
    input  logic     RXREQFLITPEND,
    output logic     RXREQLCRDV,

    output reqflit_t rxreq_posq_first_entry,
    output logic     rxreq_posq_first_entry_valid,
    input  logic     rxreq_posq_first_entry_ready,
    input            clock,
    input            reset
);

    reg rxreqflitv_q; // ensure L-credit for chi
    
    always @(posedge clock) begin: rxreqflitv_ff
        if(reset)
            rxreqflitv_q <= 1'b0;
        else if (RXREQFLITPEND == 1'b1)
            rxreqflitv_q <= 1'b1;
        else
            rxreqflitv_q <= 1'b0;
    end

    wire rxreqflit_recv_en = RXREQFLITV & rxreqflitv_q;

    parameter int rxreq_posq_size = numCreditsForHNReq[0];

    wire rxreq_posq_is_full;
    wire rxreq_posq_is_empty;
    
    sfifo #(
        .WIDTH($bits(reqflit_t)),
        .DEPTH(rxreq_posq_size)
    ) rxreq_posq (
        .clk(clock),
        .rst(reset),
        .winc(rxreqflit_recv_en),
        .rinc(rxreq_posq_first_entry_ready),
        .wdata(RXREQFLIT),
        // output
        .wfull(rxreq_posq_is_full),
        .rempty(rxreq_posq_is_empty),
        .rdata(rxreq_posq_first_entry)
    );

    assign rxreq_posq_first_entry_valid = ~rxreq_posq_is_empty;

    assign RXREQLCRDV = rxreq_posq_is_full ? 1'b0 : 1'b1;

    /*************************************************************/

    wire [3:0]  RSVDC              = RXREQFLIT.RSVDC;
    wire        TraceTag           = RXREQFLIT.TraceTag;
    wire        ExpCompAck         = RXREQFLIT.ExpCompAck;
    wire        Excl               = RXREQFLIT.Excl;
    wire [4:0]  LPID               = RXREQFLIT.LPID;
    wire        SnpAttr            = RXREQFLIT.SnpAttr;
    wire [3:0]  MemAttr            = RXREQFLIT.MemAttr;
    wire [3:0]  PCrdType           = RXREQFLIT.PCrdType;
    wire [1:0]  Order              = RXREQFLIT.Order;
    wire        AllowRetry         = RXREQFLIT.AllowRetry;
    wire        LikelyShared       = RXREQFLIT.LikelyShared;
    wire        NS                 = RXREQFLIT.NS;
    wire [47:0] Addr               = RXREQFLIT.Addr;
    wire [2:0]  Size               = RXREQFLIT.Size;
    wire [5:0]  Opcode             = RXREQFLIT.Opcode;
    // wire [7:0]  StashLPID          = RXREQFLIT.StashLPID;
    // wire        StashNIDValid      = RXREQFLIT.StashNIDValid;
    wire [6:0]  StashNID_ReturnNID = RXREQFLIT.StashNID_ReturnNID;
    wire [7:0]  TxnID              = RXREQFLIT.TxnID;
    wire [6:0]  SrcID              = RXREQFLIT.SrcID;
    wire [6:0]  TgtID              = RXREQFLIT.TgtID;
    wire [3:0]  QoS                = RXREQFLIT.QoS;

    always_comb begin
        assert(rxreqflit_recv_en & (TgtID[`CHI_REQ_SRCID_RANGE] == HNId[0][`CHI_REQ_SRCID_RANGE])) else
            $error("HNF RXREQ: Received a request with TgtID != HNF");

        assert(rxreqflit_recv_en & (TxnID[`CHI_REQ_TXNID_RANGE] < rxreq_posq_size)) else
            $error("Each RNF can only send %0d requests to HNF at a time", rxreq_posq_size);
    end

endmodule
