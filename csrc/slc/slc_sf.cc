#include "sf.h"
#include "slc.h"

void sf_evict(
    bool* rnfVec,
    size_t numCores,
    const paddr_t& addr)
{
    for (size_t c = 0; c < numCores; ++c) {
        rnfVec[c] = false;
    }
}
