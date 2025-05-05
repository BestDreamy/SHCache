module pocq #(
    parameter WIDTH = 8,  // 数据位宽
    parameter DEPTH = 16  // 缓冲区深度
)(
    input                   clk,
    input                   rst_n,
    input                   write_en,        // 写入使能
    input                   release_en,      // 释放使能
    input                   sleep_en,        // 休眠使能
    input                   wake_en,         // 唤醒使能
    input       [WIDTH-1:0] wdata,           // 写入数据
    input       [7:0]       write_txnid,     // 写入操作的 TxnID
    input       [7:0]       release_txnid,   // 释放操作的 TxnID
    input       [7:0]       sleep_txnid,     // 休眠操作的 TxnID
    input       [7:0]       wake_txnid,      // 唤醒操作的 TxnID
    output reg              full,            // 缓冲区满标志
    output reg              empty,           // 缓冲区空标志
    output reg [WIDTH-1:0]  rdata            // 读取数据
);

    // 缓冲区存储器
    reg [WIDTH-1:0] buffer [DEPTH-1:0];
    reg [DEPTH-1:0] valid;   // 有效位，表示 entry 是否被占用
    reg [DEPTH-1:0] sleep;   // 休眠位，表示 entry 是否处于休眠状态

    integer i;

    // 根据 TxnID 计算 entry 索引
    wire [$clog2(DEPTH)-1:0] write_idx   = write_txnid % DEPTH;
    wire [$clog2(DEPTH)-1:0] release_idx = release_txnid % DEPTH;
    wire [$clog2(DEPTH)-1:0] sleep_idx   = sleep_txnid % DEPTH;
    wire [$clog2(DEPTH)-1:0] wake_idx    = wake_txnid % DEPTH;

    // 初始化逻辑
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            valid <= 0;
            sleep <= 0;
            full <= 0;
            empty <= 1;
        end else begin
            // 写入逻辑
            if (write_en) begin
                buffer[write_idx] <= wdata;
                valid[write_idx] <= 1;
            end

            // 释放逻辑
            if (release_en) begin
                valid[release_idx] <= 0;
            end

            // 休眠逻辑
            if (sleep_en) begin
                sleep[sleep_idx] <= 1;
            end

            // 唤醒逻辑
            if (wake_en) begin
                sleep[wake_idx] <= 0;
            end

            // 更新缓冲区满/空标志
            full <= (valid == {DEPTH{1'b1}});
            empty <= (valid == 0);
        end
    end

    // 读取逻辑：读取第一个有效且未休眠的 entry
    always @(*) begin
        rdata = 0;
        for (i = 0; i < DEPTH; i = i + 1) begin
            if (valid[i] && !sleep[i]) begin
                rdata = buffer[i];
                break;
            end
        end
    end

endmodule