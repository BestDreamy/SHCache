#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <cstdint>
#include "../include/utils.h"
#include "auto_flit.h"
#include "flit.h"

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

    set_req_bits(0, 4);                     // QoS
    set_req_bits(req.TgtID.to_ulong(), 7);             // TgtID
    set_req_bits(req.SrcID.to_ulong(), 7);             // SrcID
    set_req_bits(req.TxnID.to_ulong(), 8);             // TxnID
    set_req_bits(0, 7);                     // StashNID_ReturnNID
    set_req_bits(0, 1);                     // StashNIDValid
    set_req_bits(0, 8);                     // StashLPID
    set_req_bits(req.Opcode.to_ulong(), 6);            // Opcode
    set_req_bits(req.Size.to_ulong(), 3);              // Size (8 bytes)
    set_req_bits(req.Addr.to_ullong(), 48);             // Addr
    set_req_bits(0, 1);                     // NS
    set_req_bits(0, 1);                     // LikelyShared
    set_req_bits(0, 1);                     // AllowRetry
    set_req_bits(0, 2);                     // Order
    set_req_bits(0, 4);                     // PCrdType
    set_req_bits(0, 4);                     // MemAttr
    set_req_bits(0, 1);                     // SnpAttr
    set_req_bits(0, 5);                     // LPID
    set_req_bits(0, 1);                     // Excl
    set_req_bits(req.ExpCompAck.to_ulong(), 1);        // ExpCompAck
    set_req_bits(0, 1);                     // TraceTag
    set_req_bits(0, 4);                     // RSVDC
    set_req_bits(0, 4);                     // SrcType
    set_req_bits(0, 6);                     // ldid

    for (int i = 0; i < 4; ++i) {
        dut->RXREQFLIT[i] = bits[i];
    }
}

inline reqflit_t chi_issue_ReadUnique_req(
    Vmodule* dut, VerilatedFstC* tfp, 
    const int &srcID, const uint32_t &Addr, const uint32_t &Size
) {
    reqflit_t req = createReadUnique(srcID, Addr, Size);
    while (true) {
        if (dut->RXREQLCRDV == 1) {
            sim_half_cycle(dut, tfp); // @posedge
            dut->RXREQFLITPEND = 1;
            dut->eval();
            sim_half_cycle(dut, tfp);

            sim_half_cycle(dut, tfp); // @posedge
            dut->RXREQFLITPEND = 0; // Special for verilator (Real should be 0)
            dut->RXREQFLITV = 1;
            encode_chi_req_flit(dut, req);
            dut->eval();
            sim_half_cycle(dut, tfp);

            sim_half_cycle(dut, tfp); // @posedge
            dut->RXREQFLITV = 0;
            dut->eval();
            sim_half_cycle(dut, tfp);
            break;
        } else {
            sim_one_cycle(dut, tfp); // @posedge
        }
    }
    return req;
}

// input: svBitVecVal*
// output: reqflit_t
inline reqflit_t decode_req_from_bitset(const std::bitset<125>& bits) {
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

    req.QoS                  = static_cast<uint8_t>(get_req_bits(offset, 4));  offset += 4;
    req.TgtID                = static_cast<uint8_t>(get_req_bits(offset, 7));  offset += 7;
    req.SrcID                = static_cast<uint8_t>(get_req_bits(offset, 7));  offset += 7;
    req.TxnID                = static_cast<uint8_t>(get_req_bits(offset, 8));  offset += 8;
    req.StashNID_ReturnNID   = static_cast<uint8_t>(get_req_bits(offset, 7));  offset += 7;
    req.StashNIDValid        = static_cast<uint8_t>(get_req_bits(offset, 1));  offset += 1;
    req.ReturnTxnID          = static_cast<uint8_t>(get_req_bits(offset, 8));  offset += 8;
    req.Opcode               = static_cast<uint8_t>(get_req_bits(offset, 6));  offset += 6;
    req.Size                 = static_cast<uint8_t>(get_req_bits(offset, 3));  offset += 3;
    req.Addr                 = static_cast<uint64_t>(get_req_bits(offset, 48)); offset += 48;
    req.NS                   = static_cast<uint8_t>(get_req_bits(offset, 1));  offset += 1;
    req.LikelyShared         = static_cast<uint8_t>(get_req_bits(offset, 1));  offset += 1;
    req.AllowRetry           = static_cast<uint8_t>(get_req_bits(offset, 1));  offset += 1;
    req.Order                = static_cast<uint8_t>(get_req_bits(offset, 2));  offset += 2;
    req.PCrdType             = static_cast<uint8_t>(get_req_bits(offset, 4));  offset += 4;
    req.MemAttr              = static_cast<uint8_t>(get_req_bits(offset, 4));  offset += 4;
    req.SnpAttr              = static_cast<uint8_t>(get_req_bits(offset, 1));  offset += 1;
    req.LPID                 = static_cast<uint8_t>(get_req_bits(offset, 5));  offset += 5;
    req.Excl                 = static_cast<uint8_t>(get_req_bits(offset, 1));  offset += 1;
    req.ExpCompAck           = static_cast<uint8_t>(get_req_bits(offset, 1));  offset += 1;
    req.TraceTag             = static_cast<uint8_t>(get_req_bits(offset, 1));  offset += 1;
    req.RSVDC                = static_cast<uint8_t>(get_req_bits(offset, 4));  offset += 4;
    return req;
}

// inline void chi_recv_ReadNoSnp_req(
//     Vmodule* dut, VerilatedFstC* tfp, 
//     const int &srcID, const uint32_t &Addr, const uint32_t &Size
// ) {
//     reqflit_t req = createReadNoSnp(srcID, Addr, Size);
//     while (true) {
//         if (dut->rxreqlcrdv == 1) {
//             dut->rxreqflitpend = 1;
//             sim_one_cycle(dut, tfp); // @posedge

//             dut->rxreqflitpend = 0;
//             dut->rxreqflitv = 1;
//             bind_chi_req_flit(dut, req);

//             sim_one_cycle(dut, tfp); // @posedge
//             dut->rxreqflitv = 0;
//             break;
//         } else {
//             sim_one_cycle(dut, tfp); // @posedge
//         }
//     }
// }

extern "C" void chi_recv_ReadNoSnp_req(reqflit_t req, datflit_t *data);

extern "C" void chi_DMT_ReadNoSnp_req(const svBitVecVal* req);

#endif // TRANSACTION_H