module hnf_rxreq(
    input  reqflit_t rxreqflit, // rxreq channel
    input  logic     rxreqflitv,
    input  logic     rxreqflitpend,
    output logic     rxreqlcrdv,

    input  clock,
    input  reset
);

    reg rxreqflitv_q;
    
    always @(posedge clock) begin: rxreqflitv_ff
        if(reset == 1'b1)
            rxreqflitv_q <= 1'b0;
        else if (rxreqflitpend == 1'b1)
            rxreqflitv_q <= 1'b1;
        else
            rxreqflitv_q <= 1'b0;
    end


endmodule