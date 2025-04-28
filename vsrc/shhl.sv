`include "chi_intf.sv"
`include "autoconfig"

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
        .clock(clock),
        .reset(reset)
    );
    

endmodule