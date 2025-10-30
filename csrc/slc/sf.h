#pragma once
#include <cstdint>
#include <cassert>

template<size_t SETS, size_t numRNs, size_t ADDR_W = 48, size_t STATE_W = 3>
struct SF {

    // Derived parameters
    static constexpr size_t SET_W = []{
        size_t w = 0;
        while ((1ULL << w) < SETS) w++;
        return w;
    }();
    static_assert((1ULL << SET_W) == SETS, "SETS must be power of 2");

    static constexpr size_t TAG_W = ADDR_W - SET_W;
    static constexpr size_t RNF_W = []{
        size_t w = 0;
        while ((1ULL << w) < numRNs) w++;
        return w + 1; // +1 same as Verilog
    }();

    // Storage
    uint64_t tagArray[SETS];
    uint8_t  rnfState[SETS]; // STATE_W bits
    uint16_t rnfId[SETS];    // RNF_W bits

    // Must be called before using
    void init() {
        for (size_t i = 0; i < SETS; i++) {
            tagArray[i] = 0;
            rnfState[i] = 0; // I
            rnfId[i] = 0;
        }
    }

    // Extract SET and TAG
    inline uint32_t set_of(uint64_t addr) const {
        return (addr >> (ADDR_W - SET_W)) & (SETS - 1);
    }

    inline uint64_t tag_of(uint64_t addr) const {
        return addr >> (ADDR_W - SET_W - TAG_W);
    }

    // Query hit
    inline bool lookup(uint64_t addr, uint8_t& state_out) const {
        uint32_t set = set_of(addr);
        uint64_t tag = tag_of(addr);

        state_out = rnfState[set];
        return (tagArray[set] == tag) && (state_out != 0); // state != I
    }

    // Called when ReadNoSnp arrives (pocq_req_valid)
    inline void install(uint64_t addr) {
        uint32_t set = set_of(addr);
        tagArray[set] = tag_of(addr);
    }

    // Called when slc_sf_rsp_valid arrives (store state + RN ID)
    inline void update_state(uint64_t addr, uint8_t newState, uint16_t srcId) {
        uint32_t set = set_of(addr);
        rnfState[set] = newState;
        rnfId[set] = srcId;
    }
};
