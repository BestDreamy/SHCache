module sf(
    input reqflit_t                     slc_sf_req, // ReadUnique
    input logic                         slc_sf_req_valid,
    input rspflit_t                     slc_sf_rsp,
    input logic                         slc_sf_rsp_valid,
    input reqflit_t                     pocq_req, // ReadNoSnp
    input logic                         pocq_req_valid,

    output                              sf_hit,
    output [`CHI_CACHE_STATE_RANGE]     sf_hit_state,

    input                               clock,
    input                               reset
);

    //           |<------16 bytes---->| 
    //   ⌈‾‾‾‾‾⌉ ⌈‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾⌉ --
    //   |     | |                    | |
    //   |     | |                    | |
    //   |     | |                    | |
    //   |     | |                    | |
    //   | Tag | |       RNF Vec      | 128 sets
    //   |     | |                    | |
    //   |     | |                    | |
    //   |     | |                    | |
    //   |     | |                    | |
    //   ⌊_____⌋ ⌊____________________⌋ --

    localparam int ADDR_W = 48, STATE_W = `CHI_CACHE_STATE_W, SET_W = 7;
    localparam int SET_NUM = 1 << SET_W;
    localparam int TAG_W = ADDR_W - $clog2(SET_NUM);
    localparam int RNF_W = $clog2(numRNs) + 1;
    reg [TAG_W-1: 0]                    tagArray [SET_NUM];
    // reg                                 rnfVec   [SET_NUM][numRNs]; // TODO: Use for share state
    reg [STATE_W-1: 0]                  rnfState [SET_NUM]; // (U|S), (D|C), I
    reg [RNF_W-1: 0]                    rnfId    [SET_NUM];


    wire is_read_unique = (slc_sf_req.Opcode == `OP_ReadUnique);

    `define SF_TAG_RANGE (ADDR_W-1) : (ADDR_W - TAG_W)
    `define SF_SET_RANGE (ADDR_W - TAG_W - 1) : (ADDR_W - TAG_W - SET_W)
    // Get the tag and set address from the incoming reqflit
    wire [SET_W-1:0]      sf_set_addr    = slc_sf_req.Addr[`SF_SET_RANGE] & {SET_W{slc_sf_req_valid}} | 
                                           pocq_req.Addr[`SF_SET_RANGE] & {SET_W{pocq_req_valid}};
    wire [TAG_W-1:0]      sf_tag         = slc_sf_req.Addr[`SF_TAG_RANGE] & {TAG_W{slc_sf_req_valid}} | 
                                           pocq_req.Addr[`SF_TAG_RANGE] & {TAG_W{pocq_req_valid}};
    assign                sf_hit_state   = rnfState[sf_set_addr];


    assign                sf_hit         = (tagArray[sf_set_addr] == sf_tag) & (sf_hit_state != `SF_I);

    always @(posedge clock) begin
        if (reset) begin
            for (int i = 0; i < SET_NUM; i ++) begin
                tagArray[i] = 'b0;
            end
        end else if (pocq_req_valid) begin
            tagArray[sf_set_addr] <= sf_tag;
        end
    end

    // always @(posedge clock) begin
    //     if (reset) begin
    //         for (int i = 0; i < SET_NUM; i ++) begin
    //             for (int j = 0; j < numRNs; j ++) begin
    //                 rnfVec[i][j] = 'b0;
    //             end
    //         end
    //     end else if (pocq_req_valid) begin
    //         rnfVec[sf_set_addr][slc_sf_rsp.SrcID] <= 'b1;
    //     end
    // end

    always @(posedge clock) begin
        if (reset) begin
            for (int i = 0; i < SET_NUM; i ++) begin
                rnfState[i] = 'b0;
            end
        end else if (pocq_req_valid) begin
            rnfState[sf_set_addr] <= slc_sf_rsp.Resp;
        end
    end

    always @(posedge clock) begin
        if (reset) begin
            for (int i = 0; i < SET_NUM; i ++) begin
                rnfId[i] = 'b0;
            end
        end else if (pocq_req_valid) begin
            rnfId[sf_set_addr] <= slc_sf_rsp.SrcID;
        end
    end


endmodule