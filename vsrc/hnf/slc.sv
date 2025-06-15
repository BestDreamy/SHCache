module slc(
    input reqflit_t                     slc_sf_req,
    input logic                         slc_sf_req_valid,
    input rspflit_t                     slc_sf_rsp, // TODO
    input logic                         slc_sf_rsp_valid, // TODO

    input                               sf_hit,
    output [`CHI_CACHE_STATE_RANGE]     sf_hit_state,

    output reqflit_t                    read_no_snp,
    output                              read_no_snp_v,

    input                               clock,
    input                               reset
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

    // Same as RN
    localparam int ADDR_W = 48, BYTE_W = 8, STATE_W = `CHI_CACHE_STATE_W, SET_W = 7, OFFSET_W = 4;
    localparam int OFFSET_NUM = 1 << OFFSET_W, SET_NUM = 1 << SET_W;
    localparam int TAG_W = ADDR_W - $clog2(OFFSET_NUM) - $clog2(SET_NUM);
    reg [TAG_W-1: 0]                    tagArray [SET_NUM];
    reg [BYTE_W-1: 0]                   dataArray[SET_NUM][OFFSET_NUM];
    reg [STATE_W-1: 0]                  stateArray[SET_NUM] ; // (U|S), (D|C), I

    // wire [47:0] Addr               = read_no_snp.Addr;
    // wire [2:0]  Size               = read_no_snp.Size;
    // wire [5:0]  Opcode             = read_no_snp.Opcode;
    // wire [7:0]  TxnID              = read_no_snp.TxnID;
    // wire [6:0]  SrcID              = read_no_snp.SrcID;
    // wire [6:0]  TgtID              = read_no_snp.TgtID;

    `define SLC_TAG_RANGE (ADDR_W-1) : (ADDR_W - TAG_W)
    `define SLC_SET_RANGE (ADDR_W - TAG_W - 1) : (ADDR_W - TAG_W - SET_W)
    wire is_read_unique = (slc_sf_req.Opcode == `OP_ReadUnique);
    // Get the tag and set address from the incoming reqflit
    wire [SET_W-1:0]    slc_set_addr    = slc_sf_req.Addr[`SLC_SET_RANGE];
    wire [TAG_W-1:0]    slc_tag         = slc_sf_req.Addr[`SLC_TAG_RANGE];
    wire [STATE_W-1:0]  slc_hit_state   = stateArray[slc_set_addr];


    `define SLC_UC 3'b100
    `define SLC_UD 3'b110
    `define SLC_SC 3'b000
    `define SLC_SD 3'b010
    `define SLC_I  3'b001
    `define SF_UC  `SLC_UC
    `define SF_UD  `SLC_UC
    `define SF_SC  `SLC_SC
    `define SF_SD  `SLC_SD
    `define SF_I   `SLC_I

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

    // chi protocal
    // slc miss and sf miss
    wire [`CHI_REQ_SRCID_RANGE] ReturnNID     = slc_sf_req.SrcID;
    wire [`CHI_REQ_TXNID_RANGE] ReturnTxnID   = slc_sf_req.TxnID;
    wire [`CHI_REQ_SRCID_RANGE] SrcID         = slc_sf_req.TgtID;
    wire [`CHI_REQ_TXNID_RANGE] TxnID         = slc_sf_req.TxnID;

    assign read_no_snp = CreateReadNoSnpReqFlit(
        .Addr(slc_sf_req.Addr),
        .Size(slc_sf_req.Size),
        .ReturnTxnID(ReturnTxnID),
        .StashNID_ReturnNID(ReturnNID),
        .TxnID(TxnID),
        .SrcID(SrcID)
    );

    assign read_no_snp_v = slc_miss_sf_miss & slc_sf_req_valid;


    // parameter int rxreq_posq_size = numCreditsForHNReq[0];
    // reg HN_Tracker [`CHI_REQ_SRCID_RANGE][rxreq_posq_size];

    // always @(posedge clock) begin: HN_Tracker_ff
    //     if (reset) begin
    //         for (int i = 0; i < `CHI_SRCID_W; i++) begin
    //             for (int j = 0; j < rxreq_posq_size; j++) begin
    //                 HN_Tracker[i][j] <= '0;
    //             end
    //         end
    //     end else begin
    //         if (read_no_snp_v) begin
    //             int free_txnid = -1;
    //             for (int i = 0; i < rxreq_posq_size; i++) begin
    //                 if (HN_Tracker[i][slc_sf_req.SrcID] == '0 && free_txnid == -1) begin
    //                     free_txnid = i;
    //                 end
    //             end

    //             if (free_txnid != -1) begin
    //                 HN_Tracker[free_txnid][slc_sf_req.SrcID] <= slc_sf_req;
    //             end else begin
    //                 $error("No free HN Tracker entry available for SrcID %0d", SrcID);
    //             end
    //         end
    //     end
    // end

    /*************************************************************/

    /*
    wire [3:0]  RSVDC              = read_no_snp.RSVDC;
    wire        TraceTag           = read_no_snp.TraceTag;
    wire        ExpCompAck         = read_no_snp.ExpCompAck;
    wire        Excl               = read_no_snp.Excl;
    wire [4:0]  LPID               = read_no_snp.LPID;
    wire        SnpAttr            = read_no_snp.SnpAttr;
    wire [3:0]  MemAttr            = read_no_snp.MemAttr;
    wire [3:0]  PCrdType           = read_no_snp.PCrdType;
    wire [1:0]  Order              = read_no_snp.Order;
    wire        AllowRetry         = read_no_snp.AllowRetry;
    wire        LikelyShared       = read_no_snp.LikelyShared;
    wire        NS                 = read_no_snp.NS;
    wire [47:0] Addr               = read_no_snp.Addr;
    wire [2:0]  Size               = read_no_snp.Size;
    wire [5:0]  Opcode             = read_no_snp.Opcode;
    // wire [7:0]  StashLPID          = read_no_snp.StashLPID;
    // wire        StashNIDValid      = read_no_snp.StashNIDValid;
    wire [6:0]  StashNID_ReturnNID = read_no_snp.StashNID_ReturnNID;
    wire [7:0]  TxnID              = read_no_snp.TxnID;
    wire [6:0]  SrcID              = read_no_snp.SrcID;
    wire [6:0]  TgtID              = read_no_snp.TgtID;
    wire [3:0]  QoS                = read_no_snp.QoS;
    */

endmodule