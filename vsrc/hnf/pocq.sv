module pocq#(
    parameter   WIDTH = 8,
    parameter   DEPTH = 16
)(
    input                   clk     ,
    input                   rst     ,
    input                   winc    , // write increment
    input                   rinc    , // read increment
    input       [WIDTH-1:0] wdata   ,
 
    output reg              wfull   ,
    output reg              rempty  ,
    output wire [WIDTH-1:0] rdata
);
    reg[$clog2(DEPTH): 0] wp, rp;
    wire w_en, r_en;
 
    wire[$clog2(DEPTH): 0] next_wp, next_rp;
    assign next_wp = wp + 1;
    assign next_rp = rp + 1;
 
    always @(posedge clk, negedge rst_n) begin
        if (~rst_n) begin
            wp <= 0;
        end else if (w_en) begin
            wp <= next_wp;
        end
    end
 
    always @(posedge clk, negedge rst_n) begin
        if (~rst_n) begin
            rp <= 0;
        end else if (r_en) begin
            rp <= next_rp;
        end
    end
 
    assign w_en = (winc & ~wfull);
    assign r_en = (rinc & ~rempty);
 
    wire is_same_addr, is_same_msb;
    assign is_same_addr = (wp[$clog2(DEPTH)-1: 0] == rp[$clog2(DEPTH)-1: 0]);
    assign is_same_msb  = (wp[$clog2(DEPTH)] == rp[$clog2(DEPTH)]);
 
    always @(posedge clk, negedge rst_n) begin
        if (~rst_n) begin
            wfull <= 0;
        end else if (is_same_addr & ~is_same_msb) begin
            wfull <= 1;
        end else begin
            wfull <= 0;
        end
    end
 
    always @(posedge clk, negedge rst_n) begin
        if (~rst_n) begin
            rempty <= 0;
        end else if (is_same_addr & is_same_msb) begin
            rempty <= 1;
        end else begin
            rempty <= 0;
        end
    end
 
endmodule