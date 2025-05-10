module rxreq_slc (

    input                                   flush,

    input                                   pin_valid,
    output                                  pin_ready,

    output                                  pout_valid,
    input                                   pout_ready,

    input reqflit_t                         rxreq_posq_first_entry_i,

    output reqflit_t                        rxreq_posq_first_entry_o,

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
        .pin_data(rxreq_posq_first_entry_i),
        .pout_data(rxreq_posq_first_entry_o),
        .clock(clock),
        .reset(reset)
    );
endmodule