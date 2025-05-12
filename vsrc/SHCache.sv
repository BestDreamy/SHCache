`include "chi_intf.vh"
`include "autoconfig.v"
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

    input  clock,
    input  reset
);

    wire default_pipe_flush = 'b0;

    reqflit_t rxreq_posq_first_entry;
    logic     rxreq_posq_first_entry_valid;
    logic     rxreq_posq_first_entry_ready;

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
    rxreq_slc u_rxreq_slc (
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

    wire                            sf_hit;
    wire [`CHI_CACHE_STATE_RANGE]   sf_hit_state;
    reqflit_t                       read_no_snp;
    wire                            read_no_snp_v;
    slc u_slc (
        .slc_sf_req(slc_sf_req),
        .slc_sf_req_valid(slc_sf_req_valid),
        .sf_hit(sf_hit),
        .sf_hit_state(sf_hit_state),
        .read_no_snp(read_no_snp),
        .read_no_snp_v(read_no_snp_v),
        .clock(clock),
        .reset(reset)
    );

    sf u_sf (
        .slc_sf_req(slc_sf_req),
        .slc_sf_req_valid(slc_sf_req_valid),
        .sf_hit(sf_hit),
        .sf_hit_state(sf_hit_state),
        .clock(clock),
        .reset(reset)
    );

    wire                    txreq_valid;
    reqflit_t               txreqflit;
    wire                    txreq_ready;
    slc_txreq u_slc_txreq (
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

    hnf_txreq u_hnf_txreq (
        .TXREQFLIT(txreqflit),
        .TXREQFLITV(txreq_ready),
        .TXREQFLITPEND(txreq_valid),
        .TXREQLCRDV(TXREQLCRDV),
        .clock(clock),
        .reset(reset)
    );

    pocq #(
        .DEPTH(16)
    ) u_pocq_inst (
        .req_entry_en(rxreq_posq_first_entry_ready),
        .req_entry(rxreq_posq_first_entry),
        .clock(clock),
        .reset(reset)
    );
    

endmodule
