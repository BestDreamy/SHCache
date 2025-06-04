module rxrsp_slc (

    input                                   flush,

    input                                   pin_valid,
    output                                  pin_ready,

    output                                  pout_valid,
    input                                   pout_ready,

    input rspflit_t                         rxrsp_posq_first_entry_i,

    output rspflit_t                        rxrsp_posq_first_entry_o,

    input                                   clock,
    input                                   reset
);

    pipe #(
        .WIDTH($bits(rspflit_t))
    ) u_rxrsp_pocq_first_entry_pipe (
        .flush(flush),
        .pin_valid(pin_valid),
        .pin_ready(pin_ready),
        .pout_valid(pout_valid),
        .pout_ready(pout_ready),
        .pin_data(rxrsp_posq_first_entry_i),
        .pout_data(rxrsp_posq_first_entry_o),
        .clock(clock),
        .reset(reset)
    );
endmodule
