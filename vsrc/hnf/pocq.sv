module pocq #(
    parameter DEPTH = 16
)(
    input                   rxreq_pocq_first_entry_dis,
    input reqflit_t         rxreq_pocq_first_entry,

    input                   clk,
    input                   rst
);
    reg [$bits(reqflit_t)-1:0]          buffer [DEPTH-1:0];
    reg                                 valid  [DEPTH-1:0];
    reg                                 sleep  [DEPTH-1:0];

    reg [$clog2(DEPTH)-1:0] pocq_first_valid_entry;

    always @(*) begin : find_pocq_first_valid_entry
        for (int i = 0; i < DEPTH; i = i + 1) begin
            if (~valid[i]) begin
                pocq_first_valid_entry = i[$clog2(DEPTH)-1:0];
                disable find_pocq_first_valid_entry;
            end
        end
    end

    always @(posedge clk) begin: rxreq_pocq_first_entry_fill_buffer
        if (rst) begin
            for (int i = 0; i < DEPTH; i = i + 1) begin
                buffer[i] <= 'b0;
                valid[i]  <= 'b0;
                sleep[i]  <= 'b0;
            end
        end else if (rxreq_pocq_first_entry_dis) begin
            buffer[pocq_first_valid_entry] <= rxreq_pocq_first_entry;
            valid[pocq_first_valid_entry] <= 1'b1;
            sleep[pocq_first_valid_entry] <= 1'b1; // fill buffer && sleep
        end
    end
endmodule