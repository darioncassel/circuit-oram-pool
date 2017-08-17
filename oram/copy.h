#ifndef COPY_H
#define COPY_H
#include <string.h>
#include "../obliv/obliv.h"


typedef struct OcCopy OcCopy;

typedef void (*ocopy_cb)(__obliv_c__bool cond, const OcCopy*, const void*, const void*, __obliv_c__size_t); // obliv

typedef void (*ocopy_cond_cb)(__obliv_c__bool cond, const OcCopy*, void*,
                              const void*, const __obliv_c__bool *, __obliv_c__size_t); // obliv

struct OcCopy
{ 
    ocopy_cb go;
    ocopy_cond_cb goN;
    void (*release)(OcCopy*);
    __obliv_c__size_t eltsize; 
    void (*zeroFill)(const OcCopy*, void*, __obliv_c__size_t);
    void (*addN)(const OcCopy*, void*, const void*, __obliv_c__size_t);
    void (*subN)(const OcCopy*, void*, const void*, __obliv_c__size_t);
};

static inline void ocCopyN(__obliv_c__bool cond, const OcCopy* c, const void* dest, const void* src, __obliv_c__size_t n) // obliv
{ 
    c->go(cond, c, dest, src, n); 
}

static inline void ocCopy(__obliv_c__bool cond, const OcCopy* c, const void* dest, const void* src) // obliv
{ 
    ocCopyN(cond, c,dest,src,1); 
}

static inline void *ocCopyElt(__obliv_c__bool cond, OcCopy* cpy, void* arr, int x) // obliv
{ 
    return ((x * cpy->eltsize) + (char*) arr); 
}

static inline void ocCopyZeroFill(OcCopy* c, void* dest, __obliv_c__size_t n)
{ 
    c->zeroFill(c, dest, n); 
}

extern const OcCopy ocCopyBool, ocCopyInt;

#define DEFINE_OC_COPY_H(t,T) \
void ocCopy##T##_cb(__obliv_c__bool en, const OcCopy *c,const void *dest, const void *src, __obliv_c__size_t n); \
\
void ocCopyCond##T##_cb(__obliv_c__bool en, const OcCopy* c, const void* dest, \
                        const void* src, const __obliv_c__bool* cond, __obliv_c__size_t n); \
OcCopy ocCopy##T##N(__obliv_c__size_t lim); \

DEFINE_OC_COPY_H(int, Int)
DEFINE_OC_COPY_H(bool, Bool)

OcCopy ocCopyBoolN (__obliv_c__size_t lim);
OcCopy ocCopyIntN  (__obliv_c__size_t lim);

#endif
