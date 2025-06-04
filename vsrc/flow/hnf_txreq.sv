// import "DPI-C" function void chi_recv_ReadNoSnp_req(
//     input reqflit_t req,
//     output datflit_t data
//     );

import "DPI-C" function void chi_DMT_ReadNoSnp_req(
    input reqflit_t req
    );

// Transmit request flit to the SNF
module hnf_txreq(
    output reqflit_t TXREQFLIT, // txreq channel
    output logic     TXREQFLITV,
    output logic     TXREQFLITPEND,
    input  logic     TXREQLCRDV,

    input  reqflit_t txreqflit,
    input  logic     txreq_valid,
    output logic     txreq_ready,

    input            clock,
    input            reset
);
    assign TXREQFLITPEND = TXREQLCRDV;
    assign TXREQFLIT = txreqflit;
    assign TXREQFLITV = txreq_valid;

    assign txreq_ready = TXREQLCRDV;

    datflit_t CompData_q, CompData_d;
    always@(posedge clock) begin: CompData_ff
        if (reset) begin
            CompData_q <= '0;
        end else if (TXREQFLITV) begin
            CompData_q <= 'b1;
            chi_DMT_ReadNoSnp_req(TXREQFLIT);
        end
    end

    /*************************************************************/

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

// always_comb begin
//     if (TXREQFLITV) begin 
//         $display("******************verilog debug******************");
//         $display("reqFlit:");
//         $display("  TgtID      : %h", TXREQFLIT.TgtID);
//         $display("  SrcID      : %h", TXREQFLIT.SrcID);
//         $display("  TxnID      : %h", TXREQFLIT.TxnID);
//         $display("  Opcode     : %h", TXREQFLIT.Opcode);
//         $display("  Addr       : %h", TXREQFLIT.Addr);
//         $display("  Size       : %h", TXREQFLIT.Size);
//         $display("  ExpCompAck : %b", TXREQFLIT.ExpCompAck);
//     end
// end

endmodule
