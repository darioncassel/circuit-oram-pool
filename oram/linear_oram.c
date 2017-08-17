#include "linear_oram.h"


void decoderEnAux(__obliv_c__bool* dest, __obliv_c__bool en, 
                  __obliv_c__uInt ind, unsigned int indlim, 
                  unsigned int step)
{
    if (indlim == 1) { 
        *dest = en; 
        return;
    }
    __obliv_c__uInt __obliv_uInt_tmp1 = __obliv_c__newUInt();
    __obliv_c__genOblivUInt(__obliv_uInt_tmp1, (unsigned int) 1);
    // (ind & 1)
    __obliv_c__uInt __obliv_uInt_tmp2 = __obliv_c__newUInt();
    __obliv_c__uIntBitwiseAnd(__obliv_uInt_tmp2, ind, __obliv_uInt_tmp1);
    // __obliv_c__bool en1 = ((ind & 1) & en);`
    __obliv_c__bool en1 = __obliv_c__newBool();
    __obliv_c__bool __obliv_bool_tmp1 = __obliv_c__newBool();
    __obliv_bool_tmp1.bits = __obliv_uInt_tmp2.bits;
    __obliv_c__boolAnd(en1, __obliv_bool_tmp1, en);
    // __obliv_c__bool en0 = (en1 != en);
    __obliv_c__bool en0 = __obliv_c__newBool();
    __obliv_c__boolNotEqual(en0, en1, en);
    __obliv_c__uIntRShift(ind, ind, 1);
    decoderEnAux(dest, en0, ind, (indlim + 1) / 2, step * 2);
    decoderEnAux(dest + step, en1, ind, indlim / 2, step * 2);
}

void decoderEn(__obliv_c__bool* dest, __obliv_c__bool en,
               __obliv_c__uInt ind, unsigned int indlim)
{ 
    decoderEnAux(dest, en, ind, indlim, 1);
}

void ocLinearOramRelease(OcOram* super) 
{ 
    OcLinearOram* ram = (OcLinearOram*) super; // CAST(super);
    free(ram->data);
    free(ram->flags);
    free(ram);
}

void ocLinearOramAccess(__obliv_c__bool cond, OcOram* super, __obliv_c__int index, OcOramAccessCb f, void* arg) // obliv
{
    OcLinearOram* ram = (OcLinearOram*) super; // CAST(super);
    // ~obliv(en)
    __obliv_c__uInt __obliv_uInt_tmp1;
    __obliv_uInt_tmp1.bits = index.bits;
    decoderEn(ram->flags, cond, __obliv_uInt_tmp1, super->n);
    for (int i = 0; i < super->n; ++i) {
        // obliv if (ram->flags[i])
            __obliv_c__bool cond_res = __obliv_c__newBool();
            __obliv_c__boolAnd(cond_res, ram->flags[i], cond); // TODO: Check that this AND has to happen
            f(cond_res, super, ocCopyElt(cond_res, super->cpy, ram->data, i), arg);
        // }
    }
}

OcLinearOram* ocLinearOramNew(OcCopy* cpy, void* data, int n)
{
    OcLinearOram* ram = malloc(sizeof(*ram));
    ram->data = malloc(cpy->eltsize * n);
    if (data) {
        memcpy(ram->data, data, cpy->eltsize * n);
    } else {
        ocCopyZeroFill(cpy, ram->data, n);
    }
    ram->flags = calloc(n, sizeof(__obliv_c__bool));
    OcOram *s = &ram->super;
    s->n = n;
    s->cpy = cpy;
    s->access = ocLinearOramAccess;
    s->release = ocLinearOramRelease;
    return ram;
}
