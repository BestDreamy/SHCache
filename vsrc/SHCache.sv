`include "chi_flit.vh"
`include "autoconfig.vh"
`include "chi_cache.vh"

module SHCache(
    input  reqflit_t RXREQFLIT, // rxreq channel
    input  logic     RXREQFLITV,
    input  logic     RXREQFLITPEND,
    output logic     RXREQLCRDV,

    output reqflit_t TXREQFLIT, // txreq channel
    output logic     TXREQFLITV,
    output logic     TXREQFLITPEND,
    input  logic     TXREQLCRDV,

    input  rspflit_t RXRSPFLIT, // rxrsp channel
    input  logic     RXRSPFLITV,
    input  logic     RXRSPFLITPEND,
    output logic     RXRSPLCRDV,

    input  rspflit_t RXDATFLIT, // rxdat channel
    input  logic     RXDATFLITV,
    input  logic     RXDATFLITPEND,
    output logic     RXDATLCRDV,

    output logic     pocq_is_empty, // pocq is empty when all valid bits are 0

    input  clock,
    input  reset
);

    wire default_pipe_flush = 'b0;

    reqflit_t rxreq_posq_first_entry;
    logic     rxreq_posq_first_entry_valid;
    logic     rxreq_posq_first_entry_ready;

    // 1.1.1 RXREQ Channel: Receive Request Flit
    hnf_rxreq u_hnf_rxreq (
        .RXREQFLIT(RXREQFLIT),
        .RXREQFLITV(RXREQFLITV),
        .RXREQFLITPEND(RXREQFLITPEND),
        .RXREQLCRDV(RXREQLCRDV),
        .rxreq_posq_first_entry(rxreq_posq_first_entry),
        .rxreq_posq_first_entry_valid(rxreq_posq_first_entry_valid),
        .rxreq_posq_first_entry_ready(rxreq_posq_first_entry_ready),
        .clock(clock),
        .reset(reset)
    );

    wire                slc_sf_req_valid;
    reqflit_t           slc_sf_req;
    wire                slc_sf_req_ready;
    wire                slc_sf_req_lhs_hs = rxreq_posq_first_entry_valid & 
                                            rxreq_posq_first_entry_ready;
    wire                pocq_push_rxreq_en = slc_sf_req_lhs_hs & slc_sf_req.ExpCompAck;
    // 1.1.2 REQ2SLC: Pipe RXREQ to SLC SF
    rxreq_slc pipe_rxreq_slc (
        .flush(default_pipe_flush),
        .pin_valid(rxreq_posq_first_entry_valid),
        .pin_ready(rxreq_posq_first_entry_ready),
        .pout_valid(slc_sf_req_valid),
        .pout_ready(slc_sf_req_ready), // to be determined
        .rxreq_posq_first_entry_i(rxreq_posq_first_entry),
        .rxreq_posq_first_entry_o(slc_sf_req),
        .clock(clock),
        .reset(reset)
    );


    reqflit_t rxrsp_posq_first_entry;
    logic     rxrsp_posq_first_entry_valid;
    logic     rxrsp_posq_first_entry_ready;

    // 1.2.1 RXRSP Channel: Receive Response Flit
    hnf_rxrsp u_hnf_rxrsp (
        .RXRSPFLIT(RXRSPFLIT),
        .RXRSPFLITV(RXRSPFLITV),
        .RXRSPFLITPEND(RXRSPFLITPEND),
        .RXRSPLCRDV(RXRSPLCRDV),
        .rxrsp_posq_first_entry(rxrsp_posq_first_entry),
        .rxrsp_posq_first_entry_valid(rxrsp_posq_first_entry_valid),
        .rxrsp_posq_first_entry_ready(rxrsp_posq_first_entry_ready),
        .clock(clock),
        .reset(reset)
    );

    wire                slc_sf_rsp_valid;
    reqflit_t           slc_sf_rsp;
    wire                slc_sf_rsp_ready;
    wire                slc_sf_rsp_lhs_hs = rxrsp_posq_first_entry_valid & 
                                            rxrsp_posq_first_entry_ready;
    wire                pocq_push_rxrsp_en = slc_sf_rsp_lhs_hs;
    // 1.2.2 REQ2SLC: Pipe RXRSP to SLC SF
    rxrsp_slc pipe_rxrsp_slc (
        .flush(default_pipe_flush),
        .pin_valid(rxrsp_posq_first_entry_valid),
        .pin_ready(rxrsp_posq_first_entry_ready),
        .pout_valid(slc_sf_rsp_valid),
        .pout_ready(slc_sf_req_ready), // to be determined
        .rxrsp_posq_first_entry_i(rxrsp_posq_first_entry),
        .rxrsp_posq_first_entry_o(slc_sf_rsp),
        .clock(clock),
        .reset(reset)
    );


    // 2.1 SLC SF: Main Snoop Filter Logic
    wire                            sf_hit;
    wire [`CHI_CACHE_STATE_RANGE]   sf_hit_state;
    reqflit_t                       read_no_snp;
    wire                            read_no_snp_v;
    slc u_slc (
        .slc_sf_req(slc_sf_req),
        .slc_sf_req_valid(slc_sf_req_valid),
        .slc_sf_rsp(slc_sf_rsp),
        .slc_sf_rsp_valid(slc_sf_rsp_valid),
        .sf_hit(sf_hit),
        .sf_hit_state(sf_hit_state),
        .read_no_snp(read_no_snp),
        .read_no_snp_v(read_no_snp_v),
        .clock(clock),
        .reset(reset)
    );

    // 2.2 Snoop Filter: Handle Snoop Requests
    sf u_sf (
        .slc_sf_req(slc_sf_req),
        .slc_sf_req_valid(slc_sf_req_valid),
        .slc_sf_rsp(slc_sf_rsp),
        .slc_sf_rsp_valid(slc_sf_rsp_valid),
        .sf_hit(sf_hit),
        .sf_hit_state(sf_hit_state),
        .clock(clock),
        .reset(reset)
    );

    // 2.3 POCQ: Push Read No Snoop Requests and Responses
    pocq #(
        .DEPTH(16)
    ) u_hnf_pocq (
        .req_entry_en(pocq_push_rxreq_en),
        .req_entry(rxreq_posq_first_entry),
        .rsp_entry_en(pocq_push_rxrsp_en),
        .rsp_entry(rxrsp_posq_first_entry),
        .read_no_snp(read_no_snp),
        .read_no_snp_v(read_no_snp_v),
        .pocq_is_empty(pocq_is_empty),
        .clock(clock),
        .reset(reset)
    );

    wire                    txreq_valid;
    reqflit_t               txreqflit;
    wire                    txreq_ready;
    wire                    txreq_rhs_hs = txreq_valid & txreq_ready;
    // 3.1.1 TXREQ Channel: Create Request Flit
    slc_txreq pipe_slc_txreq (
        .flush(default_pipe_flush),
        .pin_valid(read_no_snp_v),
        .pin_ready(slc_sf_req_ready),
        .pout_valid(txreq_valid),
        .pout_ready(txreq_ready), // to be determined
        .txreq_in(read_no_snp),
        .txreq_out(txreqflit),
        .clock(clock),
        .reset(reset)
    );

    // 3.1.2 TXREQ Channel: Transmit Request Flit
    hnf_txreq u_hnf_txreq (
        .TXREQFLIT(TXREQFLIT),
        .TXREQFLITV(TXREQFLITV),
        .TXREQFLITPEND(TXREQFLITPEND),
        .TXREQLCRDV(TXREQLCRDV),
        .txreqflit(txreqflit),
        .txreq_valid(txreq_rhs_hs),
        .txreq_ready(txreq_ready),
        .clock(clock),
        .reset(reset)
    );
    

endmodule
