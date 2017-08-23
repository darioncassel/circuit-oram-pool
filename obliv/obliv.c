#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obliv.h"


__obliv_c__int __obliv_c__newInt(void)
{
	__obliv_c__int newInt;
	newInt.bits = _mm_malloc(sizeof(OblivBit) * 32, 32);
    if (newInt.bits == NULL) {
        exit(EXIT_FAILURE);
    }
	return newInt;
}

__obliv_c__uInt __obliv_c__newUInt(void)
{
	__obliv_c__uInt newUInt;
	newUInt.bits = _mm_malloc(sizeof(OblivBit) * 32, 32);
    if (newUInt.bits == NULL) {
        exit(EXIT_FAILURE);
    }
	return newUInt;
}

__obliv_c__bool __obliv_c__newBool(void)
{
	__obliv_c__bool newBool;
	newBool.bits = _mm_malloc(sizeof(OblivBit) * 1, 32);
    if (newBool.bits == NULL) {
        exit(EXIT_FAILURE);
    }
	return newBool;
}

__obliv_c__float __obliv_c__newFloat(void)
{
	__obliv_c__float newFloat;
	newFloat.bits = _mm_malloc(sizeof(OblivBit) * 32, 32);
    if (newFloat.bits == NULL) {
        exit(EXIT_FAILURE);
    }
	return newFloat;
}

void __obliv_c__copyBit(OblivBit* dest,const OblivBit* src)
{ 
	if (dest != src) {
		*dest = *src;
	}
}

void __obliv_c__copyBits(OblivBit* dest, const OblivBit* src, __obliv_c__size_t size)
{ 
    if(dest != src) {
        while(size-- > 0) {
            __obliv_c__copyBit(dest++, src++); 
        }
    }
}

void __obliv_c__assignBitKnown(OblivBit* dest, bool value)
{ 
    bool valbox[1];
    valbox[0] = value;
    #ifdef POOL_GARB
        AliceInput(&dest->pool.w, valbox, 1);
    #endif
    #ifdef POOL_EVAL
        AliceInput(&dest->pool.wE, 1);
    #endif
    #ifdef OBLIV_KNOWN
        dest->knownValue = value; 
    #endif
    dest->unknown = true;
}

static inline bool __obliv_c__known(const OblivBit* o) { 
    return !o->unknown; 
}

bool __obliv_c__bitIsKnown(bool* v, const OblivBit* bit)
{ 
    if(__obliv_c__known(bit)) {
        *v = bit->knownValue;
    }
    return __obliv_c__known(bit);
}

void __obliv_c__flipBit(OblivBit* dest) 
{
    #ifdef OBLIV_KNOWN
        dest->knownValue = !dest->knownValue;
    #endif
}

void __obliv_c__genOblivBool(__obliv_c__bool dest, bool x) 
{
    __obliv_c__assignBitKnown(dest.bits, x);
}

void __obliv_c__revOblivBool(bool* dest, __obliv_c__bool src)
{
    #ifdef POOL_GARB
        *dest = AliceOutput(src.bits->pool.w);
    #endif
    #ifdef POOL_EVAL
        AliceOutput(src.bits->pool.wE);
    #endif
    #ifdef OBLIV_KNOWN
        *dest = src.bits[0].knownValue;
    #endif
}

void __obliv_c__genOblivInt(__obliv_c__int dest, int x) 
{
    int int_byte_size = sizeof(int);
    int byte_size = sizeof(char) * 8;
    int this_bit;
    for ( int i = 0; i < int_byte_size * byte_size; i++ ) {
	this_bit = (x >> i) & 1;
        __obliv_c__assignBitKnown(dest.bits+(i), this_bit);
    }
}

void __obliv_c__genOblivUInt(__obliv_c__uInt dest, unsigned int x) 
{
    int int_byte_size = sizeof(int);
    int byte_size = sizeof(char) * 8;
    int this_bit;
    for ( int i = 0; i < int_byte_size * byte_size; i++ ) {
	this_bit = (x >> i) & 1;
        __obliv_c__assignBitKnown(dest.bits+(i), this_bit);
    }
}

void __obliv_c__revOblivInt(int* dest, __obliv_c__int src) 
{
    int int_byte_size = sizeof(int);
    int byte_size = sizeof(char) * 8;
    int* ret = _mm_malloc(int_byte_size * byte_size, 32);
    *ret = 0;
    for ( int i = 0; i < int_byte_size * byte_size; i++ ) {
        #ifdef POOL_GARB
            *ret |= AliceOutput(src.bits[i].pool.w);
        #endif
        #ifdef POOL_EVAL
            AliceOutput(src.bits[i].pool.wE);
        #endif
        #ifdef OBLIV_KNOWN
            *ret |= (src.bits[i].knownValue << i);
        #endif
    }
    memcpy(dest, ret, int_byte_size);
}

void __obliv_c__genOblivFloat(OblivBit* dest, float x) 
{
    int float_byte_size = sizeof(float);
    int byte_size = sizeof(char) * 8;
    unsigned char* float_bytes = (unsigned char*) &x;
    int bit_number;
    int this_bit;
    for ( int i = 0; i < float_byte_size * byte_size; i++ ) {
        bit_number = i % byte_size;
        this_bit = (*float_bytes >> bit_number) & 1;
        __obliv_c__assignBitKnown(dest+(i), this_bit);
        if ( bit_number == byte_size - 1 ) {
            float_bytes++;
        }
    }
}

void __obliv_c__revOblivFloat(float* dest, OblivBit* bits) 
{
    int float_byte_size = sizeof(float);
    int byte_size = sizeof(char) * 8;
    int tmp = 0;
    unsigned char floatBytes[float_byte_size];
    memcpy(floatBytes, (unsigned char*) &tmp, float_byte_size);
    int j = 0;
    unsigned char currentByte = floatBytes[j];
    int bit_number;
    for ( int i = 0; i < float_byte_size * byte_size; i++ ) {
        bit_number = i % byte_size;
        currentByte |= (bits[i].knownValue << bit_number);
        if (bit_number == byte_size - 1) {
            floatBytes[j] = currentByte;
            j++;
            currentByte = floatBytes[j];
        }
    }
    memcpy(dest, floatBytes, float_byte_size);
}

void __obliv_c__bitwiseOp(OblivBit* dest, const OblivBit* a, 
                          const OblivBit* b, __obliv_c__size_t size, 
                          void (*f)(OblivBit*, const OblivBit*, const OblivBit*))
{ 
    while(size-- > 0) {
        f(dest++, a++, b++);
    }
}

void __obliv_c__setBitAnd(OblivBit* dest, const OblivBit* a, const OblivBit* b)
{
    if (__obliv_c__known(a) || __obliv_c__known(b)) {
        if (!__obliv_c__known(a)) { 
            const OblivBit* t = a; a = b; b = t; 
        }
        if (a->knownValue) {
            __obliv_c__copyBit(dest, b);
        } else {
            __obliv_c__assignBitKnown(dest,false);
        }
    } else {
        __obliv_c__gateAND(dest, a, b);
    }
}

void __obliv_c__setBitOr(OblivBit* dest, const OblivBit* a, const OblivBit* b)
{
    if (__obliv_c__known(a) || __obliv_c__known(b)) { 
        if (!__obliv_c__known(a)) { 
            const OblivBit* t = a; a = b; b = t; 
        }
        if (!a->knownValue) {
            __obliv_c__copyBit(dest, b);
        } else {
            __obliv_c__assignBitKnown(dest, true);
        }
    } else {
        __obliv_c__gateOR(dest, a, b);
    }
}

void __obliv_c__setBitXor(OblivBit* dest, const OblivBit* a, const OblivBit* b)
{
    bool v;
    if (__obliv_c__known(a) || __obliv_c__known(b)) { 
        if (!__obliv_c__known(a)) { 
            const OblivBit* t = a; a = b; b = t;
        }
        v = a->knownValue;
        __obliv_c__copyBit(dest,b);
        if (v) {
            __obliv_c__flipBit(dest);
        }
    } else {
        __obliv_c__gateXOR(dest, a, b);
    }
}

void __obliv_c__setBitNot(OblivBit* dest ,const OblivBit* a)
{ 
    if (__obliv_c__known(a)) { 
        *dest = *a;
        #ifdef OBLIV_KNOWN
            dest->knownValue = !dest->knownValue;
        #endif
    } else {
        __obliv_c__gateNOT(dest, a); 
    }
}

void __obliv_c__setBitwiseAnd(void* vdest, const void* vop1,
                              const void* vop2, __obliv_c__size_t size)
{ 
    __obliv_c__bitwiseOp(vdest, vop1, vop2, size, __obliv_c__setBitAnd); 
}

void __obliv_c__uIntBitwiseAnd(__obliv_c__uInt dest, __obliv_c__uInt op1, 
                               __obliv_c__uInt op2) 
{
    __obliv_c__setBitwiseAnd(dest.bits, op1.bits, op2.bits, 32);
}

void __obliv_c__intBitwiseAnd(__obliv_c__int dest, __obliv_c__int op1, 
                              __obliv_c__int op2) 
{
    __obliv_c__setBitwiseAnd(dest.bits, op1.bits, op2.bits, 32);
}

void __obliv_c__boolAnd(__obliv_c__bool dest, __obliv_c__bool op1,
                        __obliv_c__bool op2)
{
    __obliv_c__setBitwiseAnd(dest.bits, op1.bits, op2.bits, 1);
}

void __obliv_c__setBitwiseOr(void* vdest, const void* vop1,
                             const void* vop2, __obliv_c__size_t size)
{ 
    __obliv_c__bitwiseOp(vdest, vop1, vop2, size, __obliv_c__setBitOr); 
}

void __obliv_c__boolOr(__obliv_c__bool dest, __obliv_c__bool op1,
                       __obliv_c__bool op2)
{
    __obliv_c__setBitwiseOr(dest.bits, op1.bits, op2.bits, 1);
}

void __obliv_c__setBitwiseXor(void* vdest, const void* vop1,
                              const void* vop2, __obliv_c__size_t size)
{ 
    __obliv_c__bitwiseOp(vdest, vop1, vop2, size, __obliv_c__setBitXor); 
}

void __obliv_c__intBitwiseXor(__obliv_c__int dest, __obliv_c__int op1,
                              __obliv_c__int op2)
{
    __obliv_c__setBitwiseXor(dest.bits, op1.bits, op2.bits, 32);
}

void __obliv_c__boolXor(__obliv_c__bool dest, __obliv_c__bool op1,
                        __obliv_c__bool op2)
{
    __obliv_c__setBitwiseXor(dest.bits, op1.bits, op2.bits, 1);
}

void __obliv_c__setNotEqual(void* vdest, const void* vop1, 
                            const void* vop2, __obliv_c__size_t size)
{
    OblivBit t;
    OblivBit *dest = vdest;
    const OblivBit *op1 = vop1, *op2 = vop2;
    __obliv_c__assignBitKnown(dest, 0);
    while(size-- > 0) {
        __obliv_c__setBitXor(&t, op1++, op2++);
        __obliv_c__setBitOr(dest, dest, &t);
    }
}

void __obliv_c__boolNotEqual(__obliv_c__bool dest, __obliv_c__bool op1,
                             __obliv_c__bool op2)
{
    __obliv_c__setNotEqual(dest.bits, op1.bits, op2.bits, 1);
}

void __obliv_c__intEqual(__obliv_c__bool dest, __obliv_c__int op1,
                         __obliv_c__int op2)
{
    __obliv_c__setNotEqual(dest.bits, op1.bits, op2.bits, 32);
    __obliv_c__flipBit(dest.bits);
}

void __obliv_c__setLessThanUnit(OblivBit* ltOut, const OblivBit* op1, 
                                const OblivBit* op2, __obliv_c__size_t size, 
                                const OblivBit* ltIn)
{
  // (a+b)(a+1)b + (a+b+1)c = b(a+1)+(a+b+1)c = ab+b+c+ac+bc = (a+b)(b+c)+c
    OblivBit t,x;
    __obliv_c__copyBit(ltOut, ltIn);
    while(size-- > 0) {
        __obliv_c__setBitXor(&x, op1, op2);
        __obliv_c__setBitXor(&t, op2, ltOut);
        __obliv_c__setBitAnd(&t, &t, &x);
        __obliv_c__setBitXor(ltOut, &t, ltOut);
        op1++; 
        op2++;
    }
}

void __obliv_c__setLessThanSigned(void* vdest, const void* vop1, 
                                  const void* vop2, __obliv_c__size_t size)
{
    OblivBit *dest = vdest;
    const OblivBit *op1 = vop1, *op2 = vop2;
    __obliv_c__assignBitKnown(dest, 0);
    __obliv_c__setLessThanUnit(dest, op1, op2, size - 1, dest);
    __obliv_c__setLessThanUnit(dest, op2 + size - 1, op1 + size - 1, 1, dest);
}

void __obliv_c__setLessOrEqualSigned (void* vdest, const void* vop1, 
                                      const void* vop2, __obliv_c__size_t size)
{
    __obliv_c__setLessThanSigned(vdest,vop2,vop1,size);
    __obliv_c__flipBit(vdest);
}

void __obliv_c__intGr(__obliv_c__bool dest, __obliv_c__int op1, __obliv_c__int op2) {
    __obliv_c__setLessOrEqualSigned(&dest, &op1, &op2, 32);
    __obliv_c__flipBit(dest.bits);
}

void __obliv_c__intGrEq(__obliv_c__bool dest, __obliv_c__int op1, __obliv_c__int op2) {
    __obliv_c__setLessThanSigned(&dest, &op1, &op2, 32);
    __obliv_c__flipBit(dest.bits);
}

void __obliv_c__setBitsAdd(void* vdest, void* carryOut,
                           const void* vop1, const void* vop2, 
                           const void* carryIn, int size)
{
  	OblivBit carry, bxc, axc, t;
  	OblivBit *dest = vdest;
  	const OblivBit *op1 = vop1, *op2 = vop2;
  	int skipLast;
  	if (size == 0) { 
		if (carryIn && carryOut) {
			__obliv_c__copyBit(carryOut, carryIn);
		}
    	return;
 	}
  	if (carryIn) {
		__obliv_c__copyBit(&carry, carryIn);
	} else {
		__obliv_c__assignBitKnown(&carry, 0);
	}
  	// skip AND on last bit if carryOut==NULL
  	skipLast = (carryOut == NULL);
  	while (size-->skipLast) {
        __obliv_c__setBitXor(&axc,op1,&carry);
        __obliv_c__setBitXor(&bxc,op2,&carry);
        __obliv_c__setBitXor(dest,op1,&bxc);
        __obliv_c__setBitAnd(&t,&axc,&bxc);
        __obliv_c__setBitXor(&carry,&carry,&t);
   	 	++dest; 
		++op1; 
		++op2;
  	}
  	if (carryOut) {
		__obliv_c__copyBit(carryOut,&carry);
	} else { 
		__obliv_c__setBitXor(&axc,op1,&carry);
    	__obliv_c__setBitXor(dest,&axc,op2);
  	}
}

void __obliv_c__intAdd(__obliv_c__int dest, __obliv_c__int op1, __obliv_c__int op2) 
{
	__obliv_c__setBitsAdd(dest.bits, NULL, op1.bits, op2.bits, NULL, 32);
}

void __obliv_c__setBitsSub(void* vdest, void* borrowOut, const void* vop1,
                           const void* vop2, const void* borrowIn, __obliv_c__size_t size)
{
    OblivBit borrow, bxc, bxa, t;
    OblivBit *dest = vdest;
    const OblivBit *op1 = vop1, *op2 = vop2;
    __obliv_c__size_t skipLast;
    if (size == 0) { 
        if (borrowIn && borrowOut) {
            __obliv_c__copyBit(borrowOut,borrowIn);
        }
        return;
    }
    if (borrowIn) {
        __obliv_c__copyBit(&borrow, borrowIn);
    } else {
        __obliv_c__assignBitKnown(&borrow, 0);
    }
    // skip AND on last bit if borrowOut==NULL
    skipLast = (borrowOut == NULL);
    while(size-->skipLast) { 
        // c = borrow; a = op1; b=op2; borrow = (b+c)(b+a)+c
        __obliv_c__setBitXor(&bxa, op1, op2);
        __obliv_c__setBitXor(&bxc, &borrow, op2);
        __obliv_c__setBitXor(dest, &bxa, &borrow);
        __obliv_c__setBitAnd(&t, &bxa, &bxc);
        __obliv_c__setBitXor(&borrow, &borrow, &t);
        ++dest; ++op1; ++op2;
    }
    if(borrowOut) {
        __obliv_c__copyBit(borrowOut, &borrow);
    } else { 
        __obliv_c__setBitXor(&bxa, op1, op2);
        __obliv_c__setBitXor(dest, &bxa, &borrow);
    }
}

void __obliv_c__intSub(__obliv_c__int dest, __obliv_c__int op1, __obliv_c__int op2) 
{
	__obliv_c__setBitsAdd(dest.bits, NULL, op1.bits, op2.bits, NULL, 32);
}

void __obliv_c__setSignExtend(void* vdest, __obliv_c__size_t dsize,
                            const void* vsrc, __obliv_c__size_t ssize)
{
    if (ssize>dsize) {
        ssize = dsize;
    }
    OblivBit *dest = vdest;
    __obliv_c__copyBits(vdest, vsrc, ssize);
    const OblivBit* s = ((const OblivBit*) vsrc) + ssize - 1;
    dsize -= ssize;
    dest += ssize;
    while (dsize-- > 0) {
        __obliv_c__copyBit(dest++, s);
    } 
}

void __obliv_c__setZeroExtend(void* vdest, __obliv_c__size_t dsize, 
                              const void* vsrc, __obliv_c__size_t ssize)
{
    if (ssize > dsize) {
        ssize = dsize;
    }
    OblivBit *dest = vdest;
    __obliv_c__copyBits(vdest, vsrc, ssize);
    dsize -= ssize;
    dest += ssize;
    while (dsize-- > 0) {
        __obliv_c__assignBitKnown(dest++, 0);
    }
}

void __obliv_c__setRShift(void* vdest, const void* vsrc, __obliv_c__size_t n,
                          unsigned shiftAmt, bool isSigned)
{
    int i;
    OblivBit *dest = vdest;
    const OblivBit *src = vsrc;
    for (i = shiftAmt; i < n; ++i) {
        __obliv_c__copyBit(dest + i - shiftAmt, src + i);
    }
    (isSigned ? __obliv_c__setSignExtend :__obliv_c__setZeroExtend)(dest, n, dest, n - shiftAmt);
}

void __obliv_c__uIntRShift(__obliv_c__uInt dest, __obliv_c__uInt src, unsigned int shift) 
{
    __obliv_c__setRShift(dest.bits, src.bits, 32, shift, false);
}

void __obliv_c__intRShift(__obliv_c__int dest, __obliv_c__int src, int shift) 
{
    __obliv_c__setRShift(dest.bits, src.bits, 32, shift, false);
}

void __obliv_c__setLShift(void* vdest, const void* vsrc, __obliv_c__size_t n,
                          unsigned shiftAmt)
{
    int i;
    OblivBit *dest = vdest;
    const OblivBit *src = vsrc;
    for (i = n - 1; i >= (signed)shiftAmt; --i) {
        __obliv_c__copyBit(dest + i, src + i - shiftAmt);
    }
    for (; i >= 0; --i) {
        __obliv_c__assignBitKnown(dest + i, false);
    }
}

void __obliv_c__intLShift(__obliv_c__int dest, __obliv_c__int src, int shift) 
{
    __obliv_c__setLShift(dest.bits, src.bits, 32, shift);
}

void __obliv_c__setBitMux(OblivBit *dest, OblivBit *a, OblivBit *b) {
    #ifdef POOL_GARB
    #endif
    #ifdef POOL_EVAL
    #endif
}

void __obliv_c__ifThenElse(void* vdest, const void* vtsrc
                           , const void* vfsrc, __obliv_c__size_t size
                           , const void* vcond)
{
    OblivBit x,a,c = *(const OblivBit*)vcond;
    OblivBit *dest=vdest;
    const OblivBit *tsrc = vtsrc, *fsrc = vfsrc;
    if (size == 32) {
        while(size-- > 0) {
            __obliv_c__setBitXor(&x, tsrc, fsrc);
            __obliv_c__setBitMux(&a, &c, &x);
            __obliv_c__setBitXor(dest, &a, fsrc);
            ++dest; 
            ++fsrc; 
            ++tsrc;
        }
    } else {
        while(size-- > 0) {
            __obliv_c__setBitXor(&x, tsrc, fsrc);
            __obliv_c__setBitAnd(&a, &c, &x);
            __obliv_c__setBitXor(dest, &a, fsrc);
            ++dest; 
            ++fsrc; 
            ++tsrc;
        }
    }
}

void __obliv_c__condAssign(const void* cond, void* dest, const void* src, __obliv_c__size_t size)
{
    __obliv_c__ifThenElse(dest, src, dest, size, cond);
}

void __obliv_c__boolCondAssign(__obliv_c__bool cond, __obliv_c__bool dest, __obliv_c__bool src)
{
    if (cond.bits == NULL) {
        cond = __obliv_c__newBool();
        __obliv_c__genOblivBool(cond, true);
    }
    if (dest.bits == NULL) {
        dest = __obliv_c__newBool();
        __obliv_c__genOblivBool(dest, false);
    }
    if (src.bits == NULL) {
        src = __obliv_c__newBool();
        __obliv_c__genOblivBool(src, false);
    }
    __obliv_c__condAssign(cond.bits, dest.bits, src.bits, 1);
}

void __obliv_c__intCondAssign(__obliv_c__bool cond, __obliv_c__int dest, __obliv_c__int src)
{
    if (cond.bits == NULL) {
        cond = __obliv_c__newBool();
        __obliv_c__genOblivBool(cond, true);
    }
    if (dest.bits == NULL) {
        dest = __obliv_c__newInt();
        __obliv_c__genOblivInt(dest, 0);
    }
    if (src.bits == NULL) {
        src = __obliv_c__newInt();
        __obliv_c__genOblivInt(src, 0);
    }
    __obliv_c__condAssign(cond.bits, dest.bits, src.bits, 32);
}

void *calloc_mset(size_t __count, size_t __size) {
    void *ptr = _mm_malloc(__size * __count, 32);
    if (ptr == NULL) {
        printf("Calloc failed!\n");
        exit(EXIT_FAILURE);
    }
    memset(ptr, 0, __size * __count);
    return ptr;
}

void *calloc_obliv(__obliv_c__bool cond, __obliv_c__size_t nitems, __obliv_c__size_t size) {
    return calloc_mset(nitems, size);
}

void free_obliv(__obliv_c__bool cond, void* ptr) {
    _mm_free(ptr);
}

void __obliv_c__dbgPrintOblivBits(OblivBit* bits, int size) 
{
	for (int i = 0; i < size; i++) {
		printf("%d", bits[i].knownValue);
	}
	printf("\n");
}

