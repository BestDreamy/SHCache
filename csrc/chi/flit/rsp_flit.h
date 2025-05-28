enum CHI_SnpFlit_Opcode {
    RespLCrdReturn = 0x0,
    SnpResp = 0x1,
    CompAck = 0x2,
    RetryAck = 0x3,
    Comp = 0x4,
    CompDBIDResp = 0x5,
    DBIDResp = 0x6,
    PCrdGrant = 0x7,
    ReadReceipt = 0x8,
    SnpRespFwded = 0x9,
    TagMatch = 0xA,
    RespSepData = 0xB,
    Persist = 0xC,
    CompPersist = 0xD,
    DBIDRespOrd = 0xE
};