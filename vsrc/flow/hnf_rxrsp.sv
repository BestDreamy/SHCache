// Receive response flit from the RNF
module hnf_rxrsp(
    input  rspflit_t RXRSPFLIT, // rxrsp channel
    input  logic     RXRSPFLITV,
    input  logic     RXRSPFLITPEND,
    output logic     RXRSPLCRDV,

    output rspflit_t rxrsp_posq_first_entry,
    output logic     rxrsp_posq_first_entry_valid,
    input  logic     rxrsp_posq_first_entry_ready,
    input            clock,
    input            reset
);

    reg rxrspflitv_q; // ensure L-credit for chi
    
    always @(posedge clock) begin: rxrspflitv_ff
        if(reset)
            rxrspflitv_q <= 1'b0;
        else if (RXRSPFLITPEND == 1'b1)
            rxrspflitv_q <= 1'b1;
        else
            rxrspflitv_q <= 1'b0;
    end

    wire rxrspflit_recv_en = RXRSPFLITV & rxrspflitv_q;

    parameter int rxrsp_posq_size = numCreditsForHNRsp[0];

    wire rxrsp_posq_is_full;
    wire rxrsp_posq_is_empty;
    
    sfifo #(
        .WIDTH($bits(rspflit_t)),
        .DEPTH(rxrsp_posq_size)
    ) rxrsp_posq (
        .clk(clock),
        .rst(reset),
        .winc(rxrspflit_recv_en),
        .rinc(rxrsp_posq_first_entry_ready),
        .wdata(RXRSPFLIT),
        // output
        .wfull(rxrsp_posq_is_full),
        .rempty(rxrsp_posq_is_empty),
        .rdata(rxrsp_posq_first_entry)
    );

    assign rxrsp_posq_first_entry_valid = ~rxrsp_posq_is_empty;

    assign RXRSPLCRDV = rxrsp_posq_is_full ? 1'b0 : 1'b1;

    /*************************************************************/

    wire        TraceTag           = RXRSPFLIT.TraceTag;
    wire [3:0]  PCrdType           = RXRSPFLIT.PCrdType;
    wire [7: 0] DBID               = RXRSPFLIT.DBID;
    wire [2:0]  FwdState_DataPull  = RXRSPFLIT.FwdState_DataPull;
    wire [2:0]  Resp               = RXRSPFLIT.Resp;
    wire [1:0]  RespErr            = RXRSPFLIT.RespErr;
    wire [5:0]  Opcode             = RXRSPFLIT.Opcode;
    wire [7:0]  TxnID              = RXRSPFLIT.TxnID;
    wire [6:0]  SrcID              = RXRSPFLIT.SrcID;
    wire [6:0]  TgtID              = RXRSPFLIT.TgtID;
    wire [3:0]  QoS                = RXRSPFLIT.QoS;

    // always_comb begin
    //     assert(rxrspflit_recv_en & (TgtID[`CHI_REQ_SRCID_RANGE] == HNId[0][`CHI_REQ_SRCID_RANGE])) else
    //         $error("HNF RXRSP: Received a rspuest with TgtID != HNF");

    //     assert(rxrspflit_recv_en & (TxnID[`CHI_REQ_TXNID_RANGE] < rxrsp_posq_size)) else
    //         $error("Each RNF can only send %0d rspuests to HNF at a time", rxrsp_posq_size);
    // end

endmodule
