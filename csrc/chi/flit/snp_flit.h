enum CHI_SnpFlit_Opcode {
    SnpLCrdReturn = 0x0,
    SnpShared = 0x1,
    SnpClean = 0x2,
    SnpOnce = 0x3,
    SnpNotSharedDirty = 0x4,
    SnpUniqueStash = 0x5,
    SnpMakeInvalidStash = 0x6,
    SnpUnique = 0x7,
    SnpCleanShared = 0x8,
    SnpCleanInvalid = 0x9,
    SnpMakeInvalid = 0xA,
    SnpStashUnique = 0xB,
    SnpStashShared = 0xC,
    SnpDVMOp = 0xD
};