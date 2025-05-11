module pipe #(
    parameter WIDTH = 16
)(
    input                                   flush,

    input                                   pin_valid,
    output                                  pin_ready,

    output                                  pout_valid,
    input                                   pout_ready,

    input  [WIDTH-1:0]                      pin_data,
    output [WIDTH-1:0]                      pout_data,

    input                                   clock,
    input                                   reset
);

/*
in_valid   valid_q   out_ready  |  out_valid(flush)  next_valid_q(flush)  in_ready(flush)
    0         0         0       |      0(0)              0(0)                 1(1)
    0         0         1       |      0(0)              0(0)                 1(1)
    0         1         0       |      1(0)              1(0)                 0(1)
    0         1         1       |      1(0)              0(0)                 1(1)
    1         0         0       |      0(0)              1(1)                 1(1)
    1         0         1       |      0(0)              1(1)                 1(1)
    1         1         0       |      1(0)              1(1)                 0(1)
    1         1         1       |      1(0)              1(1)                 1(1)
*/
    reg                 valid_q;
    wire                valid_d;
    wire                valid_en;

    assign pin_ready = ~valid_q | pout_ready | flush;
    assign valid_d   = pin_valid | (~flush & valid_q & ~pout_ready);

    assign valid_en = pin_ready;

    always @(posedge clock) begin
        if (reset)
            valid_q <= 0;
        else if (valid_en)
            valid_q <= pin_valid;
    end

    reg[WIDTH-1:0] data_q;
    assign pout_data = data_q;
    
    always @(posedge clock) begin
        if (reset)
            data_q <= 'b0;
        else if (valid_en)
            data_q <= pin_data;
    end

    assign pout_valid = valid_q & ~flush;
endmodule