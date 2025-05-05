`include "chi_intf.vh"
`include "autoconfig.v"
`include "chi_cache.vh"

module shhl(
    input  reqflit_t rxreqflit, // rxreq channel
    input  logic     rxreqflitv,
    input  logic     rxreqflitpend,
    output logic     rxreqlcrdv,

    output reqflit_t txreqflit, // txreq channel
    output logic     txreqflitv,
    output logic     txreqflitpend,
    input  logic     txreqlcrdv,

    input  clock,
    input  reset
);
    reqflit_t rxreq_pocq_first_entry;
    logic     rxreq_pocq_first_entry_v;

    hnf_rxreq u_hnf_rxreq (
        .rxreqflit(rxreqflit),
        .rxreqflitv(rxreqflitv),
        .rxreqflitpend(rxreqflitpend),
        .rxreqlcrdv(rxreqlcrdv),
        .rxreq_pocq_first_entry(rxreq_pocq_first_entry),
        .rxreq_pocq_first_entry_v(rxreq_pocq_first_entry_v),
        .clock(clock),
        .reset(reset)
    );

    wire                      sf_hit;
    wire [`CHI_CACHE_STATE_RANGE] sf_hit_state;
    slc u_slc (
        .rxreq_pocq_first_entry(rxreq_pocq_first_entry),
        .rxreq_pocq_first_entry_v(rxreq_pocq_first_entry_v),
        .sf_hit(sf_hit),
        .sf_hit_state(sf_hit_state)
    );

    sf u_sf (
        .rxreq_pocq_first_entry(rxreq_pocq_first_entry),
        .rxreq_pocq_first_entry_v(rxreq_pocq_first_entry_v),
        .sf_hit(sf_hit),
        .sf_hit_state(sf_hit_state)
    );
    

endmodule