`ifndef CHI_FLIT_H
`define CHI_FLIT_H
`include "auto_chi_flit.vh"

`define OP_ReqLCrdReturn           'h00
`define OP_ReadShared              'h01
`define OP_ReadClean               'h02
`define OP_ReadOnce                'h03
`define OP_ReadNoSnp               'h04
`define OP_PCrdReturn              'h05
`define OP_ReadUnique              'h07
`define OP_CleanShared             'h08
`define OP_CleanInvalid            'h09
`define OP_MakeInvalid             'h0A
`define OP_CleanUnique             'h0B
`define OP_MakeUnique              'h0C
`define OP_Evict                   'h0D
`define OP_DVMOp                   'h14
`define OP_WriteEvictFull          'h15
`define OP_WriteCleanFull          'h17
`define OP_WriteUniquePtl          'h18
`define OP_WriteUniqueFull         'h19
`define OP_WriteBackPtl            'h1A
`define OP_WriteBackFull           'h1B
`define OP_WriteNoSnpPtl           'h1C
`define OP_WriteNoSnpFull          'h1D
`define OP_WriteUniqueFullStash    'h20
`define OP_WriteUniquePtlStash     'h21
`define OP_StashOnceShared         'h22
`define OP_StashOnceUnique         'h23
`define OP_ReadOnceCleanInvalid    'h24
`define OP_ReadOnceMakeInvalid     'h25
`define OP_ReadNotSharedDirty      'h26
`define OP_CleanSharedPersist      'h27
`define OP_AtomicStore             'h28
`define OP_AtomicLoad              'h30
`define OP_AtomicSwap              'h38
`define OP_AtomicCompare           'h39
`define OP_PrefetchTgt             'h3A


`define  OP_SnpLCrdReturn            'h00
`define  OP_SnpShared                'h01
`define  OP_SnpClean                 'h02
`define  OP_SnpOnce                  'h03
`define  OP_SnpNotSharedDirty        'h04
`define  OP_SnpUniqueStash           'h05
`define  OP_SnpMakeInvalidStash      'h06
`define  OP_SnpUnique                'h07
`define  OP_SnpCleanShared           'h08
`define  OP_SnpCleanInvalid          'h09
`define  OP_SnpMakeInvalid           'h0A
`define  OP_SnpStashUnique           'h0B
`define  OP_SnpStashShared           'h0C
`define  OP_SnpDVMOp                 'h0D
`define  OP_SnpSharedFwd             'h11
`define  OP_SnpCleanFwd              'h12
`define  OP_SnpOnceFwd               'h13
`define  OP_SnpNotSharedDirtyFwd     'h14
`define  OP_SnpUniqueFwd             'h17


`define OP_RespLCrdReturn             'h0
`define OP_SnpResp                    'h1
`define OP_CompAck                    'h2
`define OP_RetryAck                   'h3
`define OP_Comp                       'h4
`define OP_CompDBIDResp               'h5
`define OP_DBIDResp                   'h6
`define OP_PCrdGrant                  'h7
`define OP_ReadReceipt                'h8
`define OP_SnpRespFwded               'h9


`define OP_DataLCrdReturn             'h0
`define OP_SnpRespData                'h1
`define OP_CopyBackWrData             'h2
`define OP_NonCopyBackWrData          'h3
`define OP_CompData                   'h4
`define OP_SnpRespDataPtl             'h5
`define OP_SnpRespDataFwded           'h6
`define OP_WriteDataCancel            'h7

`define SnpResp_I           3'b000
`define SnpResp_SC          3'b001
`define SnpResp_UC          3'b010
`define SnpResp_UD          3'b010
`define SnpResp_SD          3'b011
`define SnpResp_I_PD        3'b100
`define SnpResp_SC_PD       3'b101
`define SnpResp_UC_PD       3'b110

`define SnpRespData_I       3'b000
`define SnpRespData_SC      3'b001
`define SnpRespData_UC      3'b010
`define SnpRespData_UD      3'b010
`define SnpRespData_SD      3'b011
`define SnpRespData_I_PD    3'b100
`define SnpRespData_SC_PD   3'b101
`define SnpRespData_UC_PD   3'b110

`define Comp_I              3'b000
`define Comp_SC             3'b001
`define Comp_UC             3'b010
`define Comp_UD_PD          3'b110
`define Comp_SD_PD          3'b111

`define CompData_I          3'b000
`define CompData_SC         3'b001
`define CompData_UC         3'b010
`define CompData_UD_PD      3'b110
`define CompData_SD_PD      3'b111

`define CBWrData_I          3'b000
`define CBWrData_SC         3'b001
`define CBWrData_UC         3'b010
`define CBWrData_UD_PD      3'b110
`define CBWrData_SD_PD      3'b111

`define NCBWrData_I         3'b000
`define NCBWrData_SC        3'b001
`define NCBWrData_UC        3'b010
`define NCBWrData_UD_PD     3'b110
`define NCBWrData_SD_PD     3'b111

`define SIZE_1              3'b000
`define SIZE_2              3'b001
`define SIZE_4              3'b010
`define SIZE_8              3'b011
`define SIZE_16             3'b100
`define SIZE_32             3'b101
`define SIZE_64             3'b110


// CHI protocol just allow 1024 transactions in flight when txnid is 10 bits
// In order to ensure the transaction ID is unique, {SrcID[3: 0], TxnID[3 :0]} is the real transaction ID
// In other words, the max value SrcID and TxnID cannot exceed 15
// `define CHI_MAX_TXNID_W                  4   
// `define CHI_MAX_TXNID                    1 << `CHI_MAX_TXNID_W
// `define CHI_MAX_TXNID_RANGE              `CHI_MAX_TXNID_W-1 :0

// `define CHI_MAX_SRCID_W                  4
// `define CHI_MAX_SRCID                    1 << `CHI_MAX_SRCID_W
// `define CHI_MAX_SRCID_RANGE              `CHI_MAX_SRCID_W-1 :0

// Tgt fields in  reqflit_t
`define CHI_REQ_TGTID_W                        reqflit_TgtID_width
`define CHI_REQ_TGTID_RANGE                    `CHI_REQ_TGTID_W-1 :0

// Src fields in  reqflit_t
`define CHI_REQ_SRCID_W                        reqflit_SrcID_width
`define CHI_REQ_SRCID_RANGE                    `CHI_REQ_SRCID_W-1 :0

// TxnID fields in  reqflit_t
`define CHI_REQ_TXNID_W                        reqflit_TxnID_width
`define CHI_REQ_TXNID_RANGE                    `CHI_REQ_TXNID_W-1 :0

function reqflit_t CreateReadNoSnpReqFlit (
    logic       [47:0]   Addr,
    logic       [2:0]    Size,
    logic       [7:0]    ReturnTxnID,
    logic       [6:0]    StashNID_ReturnNID,
    logic       [7:0]    TxnID,
    logic       [6:0]    SrcID
    // logic       [6:0]    TgtID
);
    reqflit_t reqFlit;

    reqFlit.ExpCompAck         = 1'b0;
    reqFlit.Addr               = Addr;
    reqFlit.Size               = Size;
    reqFlit.Opcode             = `OP_ReadNoSnp;
    reqFlit.ReturnTxnID        = ReturnTxnID;
    reqFlit.StashNID_ReturnNID = StashNID_ReturnNID;
    reqFlit.TxnID              = TxnID;
    reqFlit.SrcID              = SrcID;
    reqFlit.TgtID              = SNId[0];

    return reqFlit;
endfunction

`endif