module slc(
    input reqflit_t rxreq_pocq_first_entry,
    input logic     rxreq_pocq_entry_v,

    input           sf_hit,
    input           sf_hit_state
);

  //           |<------16 bytes---->| 
  //   ⌈‾‾‾‾‾⌉ ⌈‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾⌉ --
  //   |     | |                    | |
  //   |     | |                    | |
  //   |     | |                    | |
  //   |     | |                    | |
  //   | Tag | |     Cacheline      | 128 sets
  //   |     | |                    | |
  //   |     | |                    | |
  //   |     | |                    | |
  //   |     | |                    | |
  //   ⌊_____⌋ ⌊____________________⌋ --

    localparam int ADDR_W = 48, BYTE_W = 8, STATE_W = 3, SET_W = 7, OFFSET_W = 4;
    localparam int OFFSET_NUM = 1 << OFFSET_W, SET_NUM = 1 << SET_W;
    localparam int TAG_W = ADDR_W - $clog2(OFFSET_NUM) - $clog2(SET_NUM);
    reg [TAG_W-1: 0]                    tagArray [SET_NUM];
    reg [BYTE_W-1: 0]                   dataArray[SET_NUM][OFFSET_NUM];
    reg [STATE_W-1: 0]                  stateArray[SET_NUM] ; // (U|S), (D|C), I

    // wire [47:0] Addr               = rxreqflit.Addr;
    // wire [2:0]  Size               = rxreqflit.Size;
    // wire [5:0]  Opcode             = rxreqflit.Opcode;
    // wire [7:0]  TxnID              = rxreqflit.TxnID;
    // wire [6:0]  SrcID              = rxreqflit.SrcID;
    // wire [6:0]  TgtID              = rxreqflit.TgtID;

    `define SLC_TAG_RANGE (ADDR_W-1) : (ADDR_W - TAG_W)
    `define SLC_SET_RANGE (ADDR_W - TAG_W - 1) : (ADDR_W - TAG_W - SET_W)
    wire is_read_unique = (rxreq_pocq_first_entry.Opcode == `OP_ReadUnique);
    // Get the tag and set address from the incoming reqflit
    wire [SET_W-1:0]    slc_set_addr    = rxreq_pocq_first_entry.Addr[`SLC_SET_RANGE];
    wire [TAG_W-1:0]    slc_tag         = rxreq_pocq_first_entry.Addr[`SLC_TAG_RANGE];
    wire [STATE_W-1:0]  slc_hit_state   = stateArray[slc_set_addr];


    `define SLC_UC 3'b100
    `define SLC_UD 3'b110
    `define SLC_SC 3'b000
    `define SLC_SD 3'b010
    `define SLC_I  3'b001

    wire                slc_hit            = (tagArray[slc_set_addr] == slc_tag) & (slc_hit_state != `SLC_I);

    wire                slc_UC_sf_miss     = (slc_hit & (slc_hit_state == `SLC_UC)) & (~sf_hit);
    wire                slc_UD_sf_miss     = (slc_hit & (slc_hit_state == `SLC_UD)) & (~sf_hit);
    wire                slc_SC_sf_miss     = (slc_hit & (slc_hit_state == `SLC_SC)) & (~sf_hit);
    wire                slc_SD_sf_miss     = (slc_hit & (slc_hit_state == `SLC_SD)) & (~sf_hit);

    wire                slc_SC_sf_SC       = (slc_hit & (slc_hit_state == `SLC_SC)) & (sf_hit & (sf_hit_state == `SLC_SC));
    wire                slc_SC_sf_SD       = (slc_hit & (slc_hit_state == `SLC_SC)) & (sf_hit & (sf_hit_state == `SLC_SD));
    wire                slc_SD_sf_SC       = (slc_hit & (slc_hit_state == `SLC_SD)) & (sf_hit & (sf_hit_state == `SLC_SC));
    wire                slc_miss_sf_miss   = (~slc_hit) & (~sf_hit);

    wire                slc_miss_sf_UC     = (~slc_hit) & (sf_hit & (sf_hit_state == `SF_UC));
    wire                slc_miss_sf_UD     = (~slc_hit) & (sf_hit & (sf_hit_state == `SF_UD));
    wire                slc_miss_sf_SC     = (~slc_hit) & (sf_hit & (sf_hit_state == `SF_SC));
    wire                slc_miss_sf_SD     = (~slc_hit) & (sf_hit & (sf_hit_state == `SF_SD));

    // chi proto
    // slc miss and sf miss
    reqflit_t read_no_snp = CreateReadNoSnpReqFlit(
        .Addr(rxreq_pocq_first_entry.Addr),
        .Size(rxreq_pocq_first_entry.Size),
        .Opcode(rxreq_pocq_first_entry.Opcode),
        .ReturnTxnID(rxreq_pocq_first_entry.ReturnTxnID),
        .StashNID_ReturnNID(rxreq_pocq_first_entry.StashNID_ReturnNID),
        .TxnID(rxreq_pocq_first_entry.TxnID),
        .SrcID(rxreq_pocq_first_entry.SrcID),
        .TgtID(rxreq_pocq_first_entry.TgtID),
    );
endmodule