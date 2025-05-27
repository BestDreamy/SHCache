#include <cstdint>
#include "../../include/utils.h"
#include "../flit/auto_flit.h"
#include "../flit/flit.h"

inline void sim_one_cycle(
    Vmodule* dut, VerilatedFstC* tfp
) {
    dut->clock = 1 - dut->clock; // clock = 1
    dut->eval();
    tfp->dump(time_counter ++);

    dut->clock = 1 - dut->clock; // clock = 0
    dut->eval();
    tfp->dump(time_counter ++);
}

inline void sim_half_cycle(
    Vmodule* dut, VerilatedFstC* tfp
) {
    dut->clock = 1 - dut->clock; // clock = 1
    dut->eval();
    tfp->dump(time_counter ++);
}

inline void set_bits(uint64_t value, int width, uint64_t &bit_offset, uint32_t *bits) {
    uint64_t mask = (1ULL << width) - 1;
    value &= mask;
    int word_idx = bit_offset / 32;
    int bit_in_word = bit_offset % 32;

    bits[word_idx] |= (value << bit_in_word);
    if (bit_in_word + width > 32) {
        bits[word_idx + 1] |= (value >> (32 - bit_in_word));
    }

    bit_offset += width;
}

// input: reqflit_t
// output: svBitVecVal*
inline void encode_chi_req_flit(Vmodule* dut, const reqflit_t &req) {
    // Total 125 bits
    uint32_t bits[4] = {0};

    uint64_t bit_offset = 0;

    auto set_req_bits = [&](uint64_t value, int width) {
        set_bits(value, width, bit_offset, bits);
    };

    set_req_bits(0, reqflit_QoS_width);                     // QoS
    set_req_bits(req.TgtID, reqflit_TgtID_width);             // TgtID
    set_req_bits(req.SrcID, reqflit_SrcID_width);             // SrcID
    set_req_bits(req.TxnID, reqflit_TxnID_width);             // TxnID
    set_req_bits(0, reqflit_StashNID_ReturnNID_width);                     // StashNID_ReturnNID
    set_req_bits(0, reqflit_StashNIDValid_width);                     // StashNIDValid
    set_req_bits(0, reqflit_ReturnTxnID_width);                     // StashLPID
    set_req_bits(req.Opcode, reqflit_Opcode_width);            // Opcode
    set_req_bits(req.Size, reqflit_Size_width);              // Size (8 bytes)
    set_req_bits(req.Addr, reqflit_Addr_width);             // Addr
    set_req_bits(0, reqflit_NS_width);                     // NS
    set_req_bits(0, reqflit_LikelyShared_width);                     // LikelyShared
    set_req_bits(0, reqflit_AllowRetry_width);                     // AllowRetry
    set_req_bits(0, reqflit_Order_width);                     // Order
    set_req_bits(0, reqflit_PCrdType_width);                     // PCrdType
    set_req_bits(0, reqflit_MemAttr_width);                     // MemAttr
    set_req_bits(0, reqflit_SnpAttr_width);                     // SnpAttr
    set_req_bits(0, reqflit_LPID_width);                     // LPID
    set_req_bits(0, reqflit_Excl_width);                     // Excl
    set_req_bits(req.ExpCompAck, reqflit_ExpCompAck_width);        // ExpCompAck
    set_req_bits(0, reqflit_TraceTag_width);                     // TraceTag
    set_req_bits(0, reqflit_RSVDC_width);                     // RSVDC

    for (int i = 0; i < 4; ++i) {
        dut->RXREQFLIT[i] = bits[i];
    }
}

// input: svBitVecVal*
// output: reqflit_t
inline reqflit_t decode_req_from_bitset(const std::bitset<reqflit_width>& bits) {
    reqflit_t req;
    int offset = 0;

    auto get_req_bits = [=](uint64_t offset, int width) {
        uint64_t value = 0;
        for (int i = 0; i < width; i++) {
            if (bits.test(offset + i))
                value |= (1ULL << i);
        }
        return value;
    };

    req.QoS                = static_cast<uint8_t>(get_req_bits(offset, reqflit_QoS_width));                offset += reqflit_QoS_width;
    req.TgtID              = static_cast<uint8_t>(get_req_bits(offset, reqflit_TgtID_width));              offset += reqflit_TgtID_width;
    req.SrcID              = static_cast<uint8_t>(get_req_bits(offset, reqflit_SrcID_width));              offset += reqflit_SrcID_width;
    req.TxnID              = static_cast<uint8_t>(get_req_bits(offset, reqflit_TxnID_width));              offset += reqflit_TxnID_width;
    req.StashNID_ReturnNID = static_cast<uint8_t>(get_req_bits(offset, reqflit_StashNID_ReturnNID_width)); offset += reqflit_StashNID_ReturnNID_width;
    req.StashNIDValid      = static_cast<uint8_t>(get_req_bits(offset, reqflit_StashNIDValid_width));      offset += reqflit_StashNIDValid_width;
    req.ReturnTxnID        = static_cast<uint8_t>(get_req_bits(offset, reqflit_ReturnTxnID_width));        offset += reqflit_ReturnTxnID_width;
    req.Opcode             = static_cast<uint8_t>(get_req_bits(offset, reqflit_Opcode_width));             offset += reqflit_Opcode_width;
    req.Size               = static_cast<uint8_t>(get_req_bits(offset, reqflit_Size_width));               offset += reqflit_Size_width;
    req.Addr               = static_cast<uint64_t>(get_req_bits(offset, reqflit_Addr_width));              offset += reqflit_Addr_width;
    req.NS                 = static_cast<uint8_t>(get_req_bits(offset, reqflit_NS_width));                 offset += reqflit_NS_width;
    req.LikelyShared       = static_cast<uint8_t>(get_req_bits(offset, reqflit_LikelyShared_width));       offset += reqflit_LikelyShared_width;
    req.AllowRetry         = static_cast<uint8_t>(get_req_bits(offset, reqflit_AllowRetry_width));         offset += reqflit_AllowRetry_width;
    req.Order              = static_cast<uint8_t>(get_req_bits(offset, reqflit_Order_width));              offset += reqflit_Order_width;
    req.PCrdType           = static_cast<uint8_t>(get_req_bits(offset, reqflit_PCrdType_width));           offset += reqflit_PCrdType_width;
    req.MemAttr            = static_cast<uint8_t>(get_req_bits(offset, reqflit_MemAttr_width));            offset += reqflit_MemAttr_width;
    req.SnpAttr            = static_cast<uint8_t>(get_req_bits(offset, reqflit_SnpAttr_width));            offset += reqflit_SnpAttr_width;
    req.LPID               = static_cast<uint8_t>(get_req_bits(offset, reqflit_LPID_width));               offset += reqflit_LPID_width;
    req.Excl               = static_cast<uint8_t>(get_req_bits(offset, reqflit_Excl_width));               offset += reqflit_Excl_width;
    req.ExpCompAck         = static_cast<uint8_t>(get_req_bits(offset, reqflit_ExpCompAck_width));         offset += reqflit_ExpCompAck_width;
    req.TraceTag           = static_cast<uint8_t>(get_req_bits(offset, reqflit_TraceTag_width));           offset += reqflit_TraceTag_width;
    req.RSVDC              = static_cast<uint8_t>(get_req_bits(offset, reqflit_RSVDC_width));              offset += reqflit_RSVDC_width;
    return req;
}