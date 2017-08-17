#ifndef ORAM_H
#define ORAM_H
#include "../obliv/obliv.h"
#include "copy.h"

typedef struct OcOram OcOram;
typedef void* OcOramAccessCb(__obliv_c__bool cond, OcOram*, void*, void*); // obliv

typedef struct OcOram 
{ 
    __obliv_c__size_t n;
    OcCopy* cpy;
    void (*access)(__obliv_c__bool cond, OcOram*, __obliv_c__int, OcOramAccessCb, void*);
    void (*release)(OcOram*);
} OcOram;

// Virtual function wrappers
static inline void ocOramAccess(__obliv_c__bool cond, OcOram* ram, 
                                __obliv_c__int index, OcOramAccessCb f, void* arg) // obliv
{ 
    ram->access(cond, ram, index, f, arg);
}

static inline void ocOramRelease(OcOram* ram) 
{ 
    ram->release(ram); 
}

// Read/write convenience wrappers
static void ocOramReadAux(__obliv_c__bool cond, OcOram* ram, void* oramBlock, void* extBlock) // obliv 
{ 
    ocCopy(cond, ram->cpy, extBlock, oramBlock); 
}

static void inline ocOramRead(__obliv_c__bool cond, void* dest, OcOram* ram, __obliv_c__int index) // obliv
{ 
    ocOramAccess(cond, ram, index, ocOramReadAux, dest); 
}

static void ocOramWriteAux(__obliv_c__bool cond, OcOram* ram, void* oramBlock, void* extBlock) // obliv 
{ 
    ocCopy(cond, ram->cpy, oramBlock, extBlock); 
}

static void inline ocOramWrite(__obliv_c__bool cond, OcOram* ram, __obliv_c__int index, const void* src) // obliv
{ 
    ocOramAccess(cond, ram, index, ocOramWriteAux, src); 
}

void bool_to_int_obliv(__obliv_c__bool cond, __obliv_c__bool* bools, __obliv_c__int* res, int length);

__obliv_c__int compute_depth(__obliv_c__bool cond, __obliv_c__int pos1, 
                             __obliv_c__int pos2, int logN);

#endif
