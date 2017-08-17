#ifndef LINEAR_H
#define LINEAR_H
#include <string.h>
#include <stdlib.h>
#include "../obliv/obliv.h"
#include "copy.h"
#include "oram.h"


typedef struct { 
    OcOram super;
    __obliv_c__bool* flags;
    void* data;
} OcLinearOram;

OcLinearOram* ocLinearOramNew(OcCopy* cpy,void* data,int n);

void decoderEnAux(__obliv_c__bool* dest, __obliv_c__bool en, 
                  __obliv_c__uInt ind, unsigned int indlim, 
                  unsigned int step);

void decoderEn(__obliv_c__bool* dest, __obliv_c__bool en, __obliv_c__uInt ind, unsigned int indlim);

void ocLinearOramRelease(OcOram* super);

void ocLinearOramAccess(__obliv_c__bool cond, OcOram* super, __obliv_c__int index, OcOramAccessCb f, void* arg); // obliv

OcLinearOram* ocLinearOramNew(OcCopy* cpy, void* data, int n);

#endif
