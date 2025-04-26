#ifndef CHANNEL_H
#define CHANNEL_H
#include "flit.h"
#include <sys/types.h>

struct Req {
    uint32_t reqflitpend;
    uint32_t reqflitv;
    reqFlit reqflit;
    uint32_t reqlcrdv;
};

extern Req rxreq, txreq;

#endif // CHANNEL_H