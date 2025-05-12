module pocq #(
    parameter DEPTH = 16
)(
    input                   req_entry_en,
    input reqflit_t         req_entry,

    input                   clock,
    input                   reset
);
    reg [$bits(reqflit_t)-1:0]          buffer [DEPTH-1:0];
    reg                                 valid  [DEPTH-1:0];
    reg                                 sleep  [DEPTH-1:0];

    reg [$clog2(DEPTH)-1:0] pocq_first_valid_entry;

    always @(*) begin : find_pocq_first_valid_entry
        pocq_first_valid_entry = '0;
        for (int i = 0; i < DEPTH; i = i + 1) begin
            if (~valid[i] && pocq_first_valid_entry == '0) begin
                pocq_first_valid_entry = i[$clog2(DEPTH)-1:0];
            end
        end
    end

    always @(posedge clock) begin: req_entry_into_buffer
        if (reset) begin
            for (int i = 0; i < DEPTH; i = i + 1) begin
                buffer[i] <= 'b0;
                valid[i]  <= 'b0;
                sleep[i]  <= 'b0;
            end
        end else if (req_entry_en) begin
            buffer[pocq_first_valid_entry] <= req_entry;
            valid[pocq_first_valid_entry] <= 1'b1;
            sleep[pocq_first_valid_entry] <= 1'b1; // fill buffer && sleep
        end
    end
endmodule