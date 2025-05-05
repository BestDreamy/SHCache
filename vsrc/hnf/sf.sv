module sf(
    input reqflit_t rxreq_pocq_first_entry,
    input logic     rxreq_pocq_entry_v,

    output          sf_hit,
    output          sf_hit_state
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

    localparam int ADDR_W = 48, BYTE_W = 8, STATE_W = 3, SET_W = 7;
    localparam int OFFSET_W = RNsOFFSET, SET_NUM = 1 << SET_W;
    localparam int TAG_W = ADDR_W - OFFSET_W - $clog2(SET_NUM);
    localparam int RNF_W = $clog2(numRNs);
    reg [TAG_W-1: 0]                    tagArray [SET_NUM];
    // reg [BYTE_W-1: 0]                   dataArray[SET_NUM][OFFSET_NUM];
    reg [STATE_W-1: 0]                  stateArray[SET_NUM]; // (U|S), (D|C), I
    reg [RNF_W-1: 0]                    rnfVec[SET_NUM][numRNs];


    wire is_read_unique = (rxreq_pocq_first_entry.Opcode == `OP_ReadUnique);

    `define SF_TAG_RANGE (ADDR_W-1) : (ADDR_W - TAG_W)
    `define SF_SET_RANGE (ADDR_W - TAG_W - 1) : (ADDR_W - TAG_W - SET_W)
    // Get the tag and set address from the incoming reqflit
    wire [SET_W-1:0]    sf_set_addr    = rxreq_pocq_first_entry.Addr[`SF_SET_RANGE];
    wire [TAG_W-1:0]    sf_tag         = rxreq_pocq_first_entry.Addr[`SF_TAG_RANGE];
    wire                sf_hit         = (tagArray[sf_set_addr] == sf_tag);
    wire [STATE_W-1:0]  sf_hit_state   = stateArray[sf_set_addr];

    
endmodule