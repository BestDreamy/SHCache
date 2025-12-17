#ifndef MEM_H
#define MEM_H

#include <map>
#include "chi/flit/auto_flit.h"
#include "chi/flit/req_flit.h"
#include "chi/flit/dat_flit.h"
#include "chi/flit/rsp_flit.h"

typedef uint32_t paddr_t;

struct Memory {
    std::map<paddr_t, uint32_t> mem;

    Memory();

    // Read from memory
    bool read_memory(const paddr_t &addr, uint32_t& data);

    // Write to memory
    void write_memory(const paddr_t &addr, const uint32_t &data);

    // AMBA5 CHI
    void chi_read_memory_with_DMT(const reqflit_t &req);
};

extern Memory mem;

inline datflit_t createCompData_UC(const reqflit_t &req) {
    Assert(req.Opcode == ReadNoSnp, "Opcode must be ReadNoSnp");

    datflit_t flit;

    uint8_t size = req.Size; // Suppose Size=4
    Assert(req.Addr % 4 == 0, "Address must be aligned to 4 bytes");
    Assert(size < 6, "Size must be less than 6");
    
    int num_bytes = 1 << size; // 16 bytes
    int num_words = num_bytes / 4;
    uint32_t word_data = 0;
    for (int i = 0; i < num_words; i++) {
        uint64_t word_addr = static_cast<uint64_t>((req.Addr & ~0x3UL) + i * 4);
        mem.read_memory(word_addr, word_data);
        
        // DataFlit include 256 bits (32 bytes)
        // Each size < 256 bits req just use 1 DataFlit
        for (int bit = 0; bit < 32; bit++) {
            bool bit_val = ((word_data >> bit) & 1) != 0;
            flit.Data.set(i * 32 + bit, bit_val);
        }
        
        flit.BE |= (0xF << (i * 4));
    }
    // flit.CCID              = 0;
    // flit.DBID              = req.TxnID;
    flit.Resp              = CompData_UC;
    flit.Opcode            = CompData;
    flit.HomeNID           = req.SrcID;
    // flit.TxnID             = req.ReturnTxnID;
    flit.SrcID             = req.TgtID;
    flit.TgtID             = req.StashNID_ReturnNID;

    return flit;
}
#endif