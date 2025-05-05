`include "chi_intf.sv"
`include "autoconfig.v"

module shhl(
    input  reqflit_t rxreqflit, // rxreq channel
    input  logic     rxreqflitv,
    input  logic     rxreqflitpend,
    output logic     rxreqlcrdv,

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
    

endmodule