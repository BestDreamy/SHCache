module sfifo#(
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
 
    always @(posedge clk) begin
        if (rst) begin
            wp <= 0;
        end else if (w_en) begin
            wp <= next_wp;
        end
    end
 
    always @(posedge clk) begin
        if (rst) begin
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
 
    // always @(posedge clk) begin
    //     if (rst) begin
    //         wfull <= 0;
    //     end else if (is_same_addr & ~is_same_msb) begin
    //         wfull <= 1;
    //     end else begin
    //         wfull <= 0;
    //     end
    // end
 
    // always @(posedge clk) begin
    //     if (rst) begin
    //         rempty <= 1;
    //     end else if (is_same_addr & is_same_msb) begin
    //         rempty <= 1;
    //     end else begin
    //         rempty <= 0;
    //     end
    // end

    assign wfull  = (wp[$clog2(DEPTH)-1:0] == rp[$clog2(DEPTH)-1:0]) &&
                    (wp[$clog2(DEPTH)] != rp[$clog2(DEPTH)]);

    assign rempty = (wp[$clog2(DEPTH)-1:0] == rp[$clog2(DEPTH)-1:0]) &&
                    (wp[$clog2(DEPTH)] == rp[$clog2(DEPTH)]);


    dual_port_RAM #(
        .DEPTH(DEPTH),
        .WIDTH(WIDTH)
    ) inst_dual_port_RAM (
        .wclk  (clk),
        .wenc  (w_en),
        .waddr (wp[$clog2(DEPTH)-1: 0]),
        .wdata (wdata),
        .rclk  (clk),
        .renc  (r_en),
        .raddr (rp[$clog2(DEPTH)-1: 0]),
        .rdata (rdata)
    );
 
endmodule