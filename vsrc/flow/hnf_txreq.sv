import "DPI-C" function dataflit_t chi_recv_ReadNoSnp_req(input reqflit_t req);

// Transmit request flit to the SNF
module hnf_txreq(
    output reqflit_t TXREQFLIT, // txreq channel
    output logic     TXREQFLITV,
    output logic     TXREQFLITPEND,
    input  logic     TXREQLCRDV,

    input            clock,
    input            reset
);

    reg txreqflitv_q; // ensure L-credit for chi

    always @(posedge clock) begin: TXREQFLITV_ff
        if(reset == 1'b1)
            txreqflitv_q <= 1'b0;
        else if (TXREQLCRDV & TXREQFLITPEND)
            txreqflitv_q <= 1'b1;
        else
            txreqflitv_q <= 1'b0;
    end

    assign TXREQFLITV = txreqflitv_q;

    dataflit_t CompData;
    always@(posedge clock) begin: CompData_ff
        if (reset) begin
            CompData <= '0;
        end else if (TXREQFLITV) begin
            CompData <= chi_recv_ReadNoSnp_req(TXREQFLIT);
        end
    end

    /*************************************************************/

    wire [5:0]  ldid               = TXREQFLIT.ldid;
    wire [3:0]  SrcType            = TXREQFLIT.SrcType;
    wire [3:0]  RSVDC              = TXREQFLIT.RSVDC;
    wire        TraceTag           = TXREQFLIT.TraceTag;
    wire        ExpCompAck         = TXREQFLIT.ExpCompAck;
    wire        Excl               = TXREQFLIT.Excl;
    wire [4:0]  LPID               = TXREQFLIT.LPID;
    wire        SnpAttr            = TXREQFLIT.SnpAttr;
    wire [3:0]  MemAttr            = TXREQFLIT.MemAttr;
    wire [3:0]  PCrdType           = TXREQFLIT.PCrdType;
    wire [1:0]  Order              = TXREQFLIT.Order;
    wire        AllowRetry         = TXREQFLIT.AllowRetry;
    wire        LikelyShared       = TXREQFLIT.LikelyShared;
    wire        NS                 = TXREQFLIT.NS;
    wire [47:0] Addr               = TXREQFLIT.Addr;
    wire [2:0]  Size               = TXREQFLIT.Size;
    wire [5:0]  Opcode             = TXREQFLIT.Opcode;
    // wire [7:0]  StashLPID          = TXREQFLIT.StashLPID;
    // wire        StashNIDValid      = TXREQFLIT.StashNIDValid;
    wire [6:0]  StashNID_ReturnNID = TXREQFLIT.StashNID_ReturnNID;
    wire [7:0]  TxnID              = TXREQFLIT.TxnID;
    wire [6:0]  SrcID              = TXREQFLIT.SrcID;
    wire [6:0]  TgtID              = TXREQFLIT.TgtID;
    wire [3:0]  QoS                = TXREQFLIT.QoS;

    // always_comb begin
    //     assert(txreqflit_recv_en & (TgtID[CHI_MAX_SRCID_RANGE] == HNId[0][CHI_MAX_SRCID_RANGE])) else
    //         $error("HNF RXREQ: Received a request with TgtID != HNF");
    // end

endmodule
