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

    hnf_rxreq u_hnf_rxreq (
        .rxreqflit(rxreqflit),
        .rxreqflitv(rxreqflitv),
        .rxreqflitpend(rxreqflitpend),
        .rxreqlcrdv(rxreqlcrdv),
        .rxreq_pocq_first_entry(),
        .clock(clock),
        .reset(reset)
    );
    

endmodule