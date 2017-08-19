#include "obliv_gates.h"


#ifdef DEBUG_METRICS_ON
    unsigned long long __debug__AND_count = 0;
    unsigned long long __debug__XOR_count = 0;
#endif

void __obliv_c__gateAND(OblivBit* dest, const OblivBit* a, const OblivBit* b)
{
    #ifdef DEBUG_METRICS_ON
        __debug__AND_count++;
    #endif
    #ifdef POOL_EVAL
        dest->pool.wE = PoolAnd(a->pool.wE, a->pool.wE);
    #else
        #ifdef POOL_GARB
            dest->pool.w = PoolAnd(a->pool.w, a->pool.w);
        #endif
        dest->knownValue = (a->knownValue && b->knownValue);
    #endif
    dest->unknown = true;
}

void __obliv_c__gateOR(OblivBit* dest, const OblivBit* a, const OblivBit* b)
{
    dest->knownValue = (a->knownValue || b->knownValue);
    dest->unknown = true; 
}

void __obliv_c__gateXOR(OblivBit* dest, const OblivBit* a, const OblivBit* b)
{
    #ifdef DEBUG_METRICS_ON
        __debug__XOR_count++;
    #endif
    #ifdef POOL_EVAL
        dest->pool.wE = XorGate(a->pool.wE, a->pool.wE);
    #else
        #ifdef POOL_GARB
            dest->pool.w = XorGate(a->pool.w, a->pool.w);
        #endif
        dest->knownValue = (a->knownValue && b->knownValue);
    #endif
    dest->knownValue = (!!a->knownValue != !!b->knownValue);
    dest->unknown = true;
}

void __obliv_c__gateNOT(OblivBit* dest, const OblivBit* a)
{
    dest->knownValue = !a->knownValue;
    dest->unknown = a->unknown;
}
