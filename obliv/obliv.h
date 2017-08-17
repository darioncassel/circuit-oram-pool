#ifndef OBLIV_H
#define OBLIV_H
#include "obliv_bit.h"
#include "obliv_gates.h"


typedef struct {
	OblivBit* bits;
} __obliv_c__int;

typedef struct {
	OblivBit* bits;
} __obliv_c__bool;

typedef struct {
	OblivBit* bits;
} __obliv_c__float;

typedef struct {
    OblivBit* bits;
} __obliv_c__uInt;

__obliv_c__int __obliv_c__newInt(void);
__obliv_c__uInt __obliv_c__newUInt(void);
__obliv_c__bool __obliv_c__newBool(void);
__obliv_c__float __obliv_c__newFloat(void);
bool __obliv_c__bitIsKnown(bool* v, const OblivBit* bit);
void __obliv_c__copyBit(OblivBit* dest,const OblivBit* src);
void __obliv_c__copyBits(OblivBit* dest, const OblivBit* src, __obliv_c__size_t size);
void __obliv_c__flipBit(OblivBit* dest);
void __obliv_c__genOblivBool(__obliv_c__bool dest, bool x);
void __obliv_c__revOblivBool(bool* dest, __obliv_c__bool src);
void __obliv_c__genOblivInt(__obliv_c__int dest, int x);
void __obliv_c__genOblivUInt(__obliv_c__uInt dest, unsigned int x);
void __obliv_c__revOblivInt(int* dest, __obliv_c__int src);
void __obliv_c__setBitwiseXor(void* vdest, const void* vop1,
                              const void* vop2, __obliv_c__size_t size);
void __obliv_c__intBitwiseXor(__obliv_c__int dest, __obliv_c__int op1,
                              __obliv_c__int op2);
void __obliv_c__boolXor(__obliv_c__bool dest, __obliv_c__bool op1,
                        __obliv_c__bool op2);
void __obliv_c__uIntBitwiseAnd(__obliv_c__uInt dest, __obliv_c__uInt op1, 
                               __obliv_c__uInt op2);
void __obliv_c__intBitwiseAnd(__obliv_c__int dest, __obliv_c__int op1, 
                              __obliv_c__int op2);
void __obliv_c__boolAnd(__obliv_c__bool dest, __obliv_c__bool op1,
                        __obliv_c__bool op2);
void __obliv_c__boolOr(__obliv_c__bool dest, __obliv_c__bool op1,
                       __obliv_c__bool op2);
void __obliv_c__setNotEqual (void* vdest, const void* vop1, 
                             const void* vop2, __obliv_c__size_t size);              
void __obliv_c__boolNotEqual(__obliv_c__bool dest, __obliv_c__bool op1,
                             __obliv_c__bool op2);
void __obliv_c__intEqual(__obliv_c__bool dest, __obliv_c__int op1,
                        __obliv_c__int op2);
void __obliv_c__intGr(__obliv_c__bool dest, __obliv_c__int op1, __obliv_c__int op2);
void __obliv_c__intGrEq(__obliv_c__bool dest, __obliv_c__int op1, __obliv_c__int op2);
void __obliv_c__intAdd(__obliv_c__int dest, __obliv_c__int op1, __obliv_c__int op2);
void __obliv_c__intSub(__obliv_c__int dest, __obliv_c__int op1, __obliv_c__int op2);
void __obliv_c__uIntRShift(__obliv_c__uInt dest, __obliv_c__uInt src, unsigned int shift);
void __obliv_c__intRShift(__obliv_c__int dest, __obliv_c__int src, int shift);
void __obliv_c__intLShift(__obliv_c__int dest, __obliv_c__int src, int shift);
void __obliv_c__intCondAssign(__obliv_c__bool cond, __obliv_c__int dest, __obliv_c__int src);
void __obliv_c__boolCondAssign(__obliv_c__bool cond, __obliv_c__bool dest, __obliv_c__bool src);
void *calloc_obliv(__obliv_c__bool cond, __obliv_c__size_t nitems, __obliv_c__size_t size);
void free_obliv(__obliv_c__bool cond, void* ptr);


#endif
