#include "obliv_gates.h"


#ifdef DEBUG_METRICS_ON
    unsigned long long __debug__AND_count = 0;
    unsigned long long __debug__XOR_count = 0;
#endif

__obliv_c__debugMem(OblivBit *a) {
    #ifdef POOL_EVAL
        if(_mm256_extract_epi8(a->pool.wE.W.P.B, 31) != 0) {
            printf("Test Eval!\n");
            PBprint(a->pool.wE.W.P.B);
        }
    #endif
    #ifdef POOL_GARB
        if(_mm256_extract_epi8(a->pool.w.P, 31) != 0) {
            printf("Test Garb!\n"); 
            PBprint(a->pool.w.P);
        }
    #endif
    #ifdef OBLIV_KNOWN
        if (a.knownValue == NULL) {
            printf("Malloc fail!\n");
            exit(EXIT_FAILURE);
        }
    #endif
}

__pool__debugWireLabel(OblivBit *x, char *name) {
    #ifdef POOL_GARB
        printf("%s:\n", name);
        PBprint(x->pool.w.P);
        WLprint(x->pool.w.L);
        WLprint(WLabelXor(x->pool.w.L,Delta));
        printf("\n");
    #endif
    #ifdef POOL_EVAL
        printf("%s:\n", name);
        PBprint(x->pool.wE.W.P.B);
        //   WLprint(c.pool.wE.W.L);
        WLprint(x->pool.wE.L);
        printf("\n");
    #endif
}

void __obliv_c__gateAND(OblivBit* dest, const OblivBit* a, const OblivBit* b)
{
    #ifdef DEBUG_METRICS_ON
        __debug__AND_count++;
    #endif
    #ifdef POOL_EVAL
        dest->pool.wE = PoolAnd(a->pool.wE, b->pool.wE);
    #endif
    #ifdef POOL_GARB
        dest->pool.w = PoolAnd(a->pool.w, b->pool.w);
    #endif
    #ifdef OBLIV_KNOWN
        dest->knownValue = (a->knownValue && b->knownValue);
    #endif
    dest->unknown = true;
}

void __obliv_c__gateOR(OblivBit* dest, const OblivBit* a, const OblivBit* b)
{
    #ifdef OBLIV_KNOWN
        dest->knownValue = (a->knownValue || b->knownValue);
        dest->unknown = true;
    #endif
}

void __obliv_c__gateXOR(OblivBit* dest, const OblivBit* a, const OblivBit* b)
{
    #ifdef DEBUG_METRICS_ON
        __debug__XOR_count++;
    #endif
    #ifdef POOL_EVAL
        dest->pool.wE = XorGate(a->pool.wE, b->pool.wE);
    #endif
    #ifdef POOL_GARB
        dest->pool.w = XorGate(a->pool.w, b->pool.w);
    #endif
    #ifdef OBLIV_KNOWN
        dest->knownValue = (!!a->knownValue != !!b->knownValue);
    #endif
    dest->unknown = true;
}

void __obliv_c__gateNOT(OblivBit* dest, const OblivBit* a)
{
    #ifdef OBLIV_KNOWN
        dest->knownValue = !a->knownValue;
        dest->unknown = a->unknown;
    #endif
}
