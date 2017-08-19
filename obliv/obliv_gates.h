#ifndef OBLIV_GATES_H
#define OBLIV_GATES_H
#include <stdio.h>
#include "obliv_bit.h"

#ifdef DEBUG_METRICS_ON
    extern unsigned long long __debug__AND_count;
    extern unsigned long long __debug__XOR_count;
#endif

#ifdef POOL_EVAL
    WireE PoolAnd(WireE a, WireE b);
#endif
#ifdef POOL_GARB
    Wire PoolAnd(Wire a, Wire b);
#endif

void __obliv_c__gateAND(OblivBit* dest, const OblivBit* a, const OblivBit* b);

void __obliv_c__gateOR(OblivBit* dest, const OblivBit* a, const OblivBit* b);

void __obliv_c__gateXOR(OblivBit* dest, const OblivBit* a, const OblivBit* b);

void __obliv_c__gateNOT(OblivBit* dest, const OblivBit* a);

#endif
