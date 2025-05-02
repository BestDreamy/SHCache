`include "autoconfig.v"
`include "chi_flit.vh"

module hnf_rxreq(
    input  reqflit_t rxreqflit, // rxreq channel
    input  logic     rxreqflitv,
    input  logic     rxreqflitpend,
    output logic     rxreqlcrdv,

    output reqflit_t rxreq_pocq_first_entry,
    input  clock,
    input  reset
);

    reg rxreqflitv_q;
    
    always @(posedge clock) begin: rxreqflitv_ff
        if(reset == 1'b1)
            rxreqflitv_q <= 1'b0;
        else if (rxreqflitpend == 1'b1)
            rxreqflitv_q <= 1'b1;
        else
            rxreqflitv_q <= 1'b0;
    end

    parameter int rxreq_pocq_size = numCreditsForHNReq[0];

    wire rxreq_pocq_is_full;
    wire rxreq_pocq_is_empty;
    // wire [$bits(reqFlit_t)-1:0] flat_wire;

    // assign rxreq_pocq_first_entry = flat_wire;
    
    sfifo #(
        .WIDTH($bits(reqflit_t)),
        .DEPTH(rxreq_pocq_size)
    ) rxreq_pocq (
        .clk(clock),
        .rst_n(~reset),
        .winc(rxreqflitv & rxreqflitv_q),
        .rinc(/* 读取使能信号 */),
        .wdata(rxreqflit),
        // output
        .wfull(rxreq_pocq_is_full),
        .rempty(rxreq_pocq_is_empty),
        .rdata(rxreq_pocq_first_entry)
    );

    assign rxreqlcrdv = rxreq_pocq_is_full ? 1'b0 : 1'b1;

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
    wire [7:0]  StashLPID          = rxreqflit.StashLPID;
    wire        StashNIDValid      = rxreqflit.StashNIDValid;
    wire [6:0]  StashNID_ReturnNID = rxreqflit.StashNID_ReturnNID;
    wire [7:0]  TxnID              = rxreqflit.TxnID;
    wire [6:0]  SrcID              = rxreqflit.SrcID;
    wire [6:0]  TgtID              = rxreqflit.TgtID;
    wire [3:0]  QoS                = rxreqflit.QoS;

endmodule