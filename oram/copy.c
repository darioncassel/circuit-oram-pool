#include "../obliv/obliv.h"
#include "copy.h"

static void ocZeroBitsFill(const OcCopy* cpy,void* dest,__obliv_c__size_t n);
static void xorBits(const OcCopy* cpy, void* dest, const void* src, __obliv_c__size_t n);

#define DEFINE_OC_COPY(t,T) \
    void ocCopy##T##_cb(__obliv_c__bool en, const OcCopy *c, \
                        const void *dest, const void *src, __obliv_c__size_t n) \
    { \
        bool b; \
        if (__obliv_c__bitIsKnown(&b, &en)) { \
            if (b && dest != src) { \
                memcpy(dest,src,n*c->eltsize); \
            } \
        } else { \
            __obliv_c__size_t lim = c->eltsize / sizeof(__obliv_c__##t); \
            for (int i = 0; i < n * lim; ++i) { \
                __obliv_c__##t##CondAssign(en, ((__obliv_c__##t*)dest)[i], ((const __obliv_c__##t*)src)[i]); \
            } \
        } \
    } \
    void ocCopyCond##T##_cb(__obliv_c__bool en, const OcCopy* c, const void* dest, \
                            const void* src, const __obliv_c__bool* cond, __obliv_c__size_t n) \
    { \
        __obliv_c__size_t lim = c->eltsize / sizeof(__obliv_c__##t); \
        for (int i = 0; i < n; ++i) { \
            __obliv_c__bool cond_res = __obliv_c__newBool(); \
            __obliv_c__genOblivBool(cond_res, false); \
            __obliv_c__boolAnd(cond_res, en, cond[i]); \
            for (int j = 0; j < lim; ++j) { \
                __obliv_c__##t##CondAssign(cond_res, ((__obliv_c__##t*)dest)[i*lim+j], ((const __obliv_c__##t*)src)[i*lim+j]); \
            } \
        } \
    } \
    const OcCopy ocCopy##T = { \
        .go = ocCopy##T##_cb,  \
        .goN = ocCopyCond##T##_cb, \
        .release = NULL, \
        .eltsize = sizeof(__obliv_c__##t), \
        .zeroFill = ocZeroBitsFill, \
        .addN = xorBits, \
        .subN = xorBits \
    }; \
    OcCopy ocCopy##T##N(__obliv_c__size_t lim) \
    { \
        OcCopy rv = ocCopy##T; \
        rv.eltsize *= lim; \
        return rv; \
    }

DEFINE_OC_COPY(int, Int)
DEFINE_OC_COPY(bool, Bool)

void ocZeroBitsFill(const OcCopy* cpy,void* dest,__obliv_c__size_t n)
{
    memset(dest, 0, n*cpy->eltsize);
}

void xorBits(const OcCopy* cpy, void* dest, const void* src, __obliv_c__size_t n)
{
    for(int i = 0; i < n; ++i) {
        __obliv_c__setBitwiseXor((char*)dest+i*cpy->eltsize,
                                 (char*)dest+i*cpy->eltsize,
                                 (char*)src +i*cpy->eltsize,
                                 cpy->eltsize / sizeof(__obliv_c__bool));
    }
}

