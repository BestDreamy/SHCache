#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <cstdint>
#include "../include/utils.h"
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

inline void bind_chi_req_flit(Vmodule* dut, const reqFlit &req) {
    uint32_t bits[7] = {0};

    uint64_t bit_offset = 0;

    auto set_req_bits = [&](uint64_t value, int width) {
        set_bits(value, width, bit_offset, bits);
    };

    set_req_bits(0, 4);                     // QoS
    set_req_bits(req.TgtID, 7);             // TgtID
    set_req_bits(req.SrcID, 7);             // SrcID
    set_req_bits(req.TxnID, 8);             // TxnID
    set_req_bits(0, 7);                     // StashNID_ReturnNID
    set_req_bits(0, 1);                     // StashNIDValid
    set_req_bits(0, 8);                     // StashLPID
    set_req_bits(req.Opcode, 6);            // Opcode
    set_req_bits(req.Size, 3);              // Size (8 bytes)
    set_req_bits(req.Addr, 48);             // Addr
    set_req_bits(0, 1);                     // NS
    set_req_bits(0, 1);                     // LikelyShared
    set_req_bits(0, 1);                     // AllowRetry
    set_req_bits(0, 2);                     // Order
    set_req_bits(0, 4);                     // PCrdType
    set_req_bits(0, 4);                     // MemAttr
    set_req_bits(0, 1);                     // SnpAttr
    set_req_bits(0, 5);                     // LPID
    set_req_bits(0, 1);                     // Excl
    set_req_bits(req.ExpCompAck, 1);        // ExpCompAck
    set_req_bits(0, 1);                     // TraceTag
    set_req_bits(0, 4);                     // RSVDC
    set_req_bits(0, 4);                     // SrcType
    set_req_bits(0, 6);                     // ldid

    for (int i = 0; i < 7; ++i) {
        dut->rxreqflit[i] = bits[i];
    }
}

inline void chi_issue_ReadUnique_req(
    Vmodule* dut, VerilatedFstC* tfp, 
    const int &srcID, const uint32_t &Addr, const uint32_t &Size
) {
    reqFlit req = createReadUnique(srcID, Addr, Size);
    while (true) {
        if (dut->rxreqlcrdv == 1) {
            dut->rxreqflitpend = 1;
            sim_one_cycle(dut, tfp); // @posedge

            dut->rxreqflitpend = 0;
            dut->rxreqflitv = 1;
            bind_chi_req_flit(dut, req);

            sim_one_cycle(dut, tfp); // @posedge
            dut->rxreqflitv = 0;
            break;
        } else {
            sim_one_cycle(dut, tfp); // @posedge
        }
    }
}

inline void chi_recv_ReadNoSnp_req(
    Vmodule* dut, VerilatedFstC* tfp, 
    const int &srcID, const uint32_t &Addr, const uint32_t &Size
) {
    reqFlit req = createReadNoSnp(srcID, Addr, Size);
    while (true) {
        if (dut->rxreqlcrdv == 1) {
            dut->rxreqflitpend = 1;
            sim_one_cycle(dut, tfp); // @posedge

            dut->rxreqflitpend = 0;
            dut->rxreqflitv = 1;
            bind_chi_req_flit(dut, req);

            sim_one_cycle(dut, tfp); // @posedge
            dut->rxreqflitv = 0;
            break;
        } else {
            sim_one_cycle(dut, tfp); // @posedge
        }
    }
}

#endif // TRANSACTION_H