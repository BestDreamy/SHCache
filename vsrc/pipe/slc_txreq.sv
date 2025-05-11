module slc_txreq (

    input                                   flush,

    input                                   pin_valid,
    output                                  pin_ready,

    output                                  pout_valid,
    input                                   pout_ready,

    input reqflit_t                         txreq_in,

    output reqflit_t                        txreq_out,

    input                                   clock,
    input                                   reset
);

    pipe #(
        .WIDTH($bits(reqflit_t))
    ) u_rxreq_pocq_first_entry_pipe (
        .flush(flush),
        .pin_valid(pin_valid),
        .pin_ready(pin_ready),
        .pout_valid(pout_valid),
        .pout_ready(pout_ready),
        .pin_data(txreq_in),
        .pout_data(txreq_out),
        .clock(clock),
        .reset(reset)
    );
endmodule