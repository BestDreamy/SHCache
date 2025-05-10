`include "chi_intf.vh"
`include "autoconfig.v"
`include "chi_cache.vh"

module shhl(
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
    wire default_flush = 'b0;

    reqflit_t rxreq_posq_first_entry;
    logic     rxreq_posq_first_entry_v;
    logic     rxreq_posq_first_entry_dis;

    hnf_rxreq u_hnf_rxreq (
        .RXREQFLIT(RXREQFLIT),
        .RXREQFLITV(RXREQFLITV),
        .RXREQFLITPEND(RXREQFLITPEND),
        .RXREQLCRDV(RXREQLCRDV),
        .rxreq_posq_first_entry(rxreq_posq_first_entry),
        .rxreq_posq_first_entry_v(rxreq_posq_first_entry_v),
        .clock(clock),
        .reset(reset)
    );

    wire slc_sf_req_v;
    reqflit_t slc_sf_req;
    wire rxreq_slc_dis;
    rxreq_slc u_rxreq_slc (
        .flush(default_flush),
        .pin_valid(rxreq_posq_first_entry_v),
        .pin_ready(rxreq_posq_first_entry_dis),
        .pout_valid(slc_sf_req_v),
        .pout_ready(rxreq_slc_dis), // to be determined
        .rxreq_posq_first_entry_i(rxreq_posq_first_entry_i),
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
        .slc_sf_req_v(slc_sf_req_v),
        .sf_hit(sf_hit),
        .sf_hit_state(sf_hit_state),
        .read_no_snp(read_no_snp),
        .read_no_snp_v(read_no_snp_v),
        .clock(clock),
        .reset(reset)
    );

    sf u_sf (
        .slc_sf_req(slc_sf_req),
        .slc_sf_req_v(slc_sf_req_v),
        .sf_hit(sf_hit),
        .sf_hit_state(sf_hit_state),
        .clock(clock),
        .reset(reset)
    );

    wire txreqflitv;
    reqflit_t txreqflit;
    wire txreqflitp;
    slc_txreq u_slc_txreq (
        .flush(default_flush),
        .pin_valid(read_no_snp_v),
        .pin_ready(rxreq_slc_dis),
        .pout_valid(txreqflitv),
        .pout_ready(txreqflitp), // to be determined
        .TXREQFLIT_i(read_no_snp),
        .TXREQFLIT_o(txreqflit),
        .clock(clock),
        .reset(reset)
    );

    hnf_txreq u_hnf_txreq (
        .TXREQFLIT(txreqflit),
        .TXREQFLITV(txreqflitp),
        .TXREQFLITPEND(txreqflitv),
        .TXREQLCRDV(TXREQLCRDV),
        .clock(clock),
        .reset(reset)
    );

    // pocq #(
    //     .DEPTH(16)
    // ) u_pocq_inst (
    //     .rxreq_pocq_first_entry_dis(rxreq_pocq_first_entry_dis),
    //     .rxreq_pocq_first_entry(rxreq_pocq_first_entry),
    //     .clk(clock),
    //     .rst(reset),
    // );
    

endmodule
