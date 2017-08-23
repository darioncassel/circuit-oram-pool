#include "oram.h"


void bool_to_int_obliv(__obliv_c__bool cond, __obliv_c__bool *bools, __obliv_c__int *res, int length) {
    // *res = 0;
    __obliv_c__int tmp0 = __obliv_c__newInt();
    __obliv_c__genOblivInt(tmp0, 0);
    __obliv_c__intCondAssign(cond, *res, tmp0);
    int i = 32;
    if (length < i) {
        i = length;
        --i;
        for(; i >= 0; --i) {
            // *res = (*res << 1);
            __obliv_c__int tmp1 = __obliv_c__newInt();
            __obliv_c__genOblivInt(tmp1, 0);
            __obliv_c__intLShift(tmp1, *res, 1);
            __obliv_c__intCondAssign(cond, *res, tmp1);
            // *res = (*res ^ (bools[i]));
            __obliv_c__int tmp2 = __obliv_c__newInt();
            __obliv_c__genOblivInt(tmp2, 0);
            if (bools[i].bits == NULL) {
                bools[i] = __obliv_c__newBool();
                __obliv_c__genOblivBool(bools[i], false);
            }
            __obliv_c__setBitwiseXor(tmp2.bits, res->bits, bools[i].bits, 1);
            __obliv_c__intCondAssign(cond, *res, tmp2);
        }
    }
}

__obliv_c__int compute_depth(__obliv_c__bool cond, __obliv_c__int pos1, 
                             __obliv_c__int pos2, int logN)
{  
    //return pos1^pos2;
    // obliv int x = pos1 ^ pos2;
    __obliv_c__int x = __obliv_c__newInt();
    __obliv_c__int tmp0 = __obliv_c__newInt();
    __obliv_c__intBitwiseXor(tmp0, pos1, pos2);
    __obliv_c__intCondAssign(cond, x, tmp0);
    // obliv int n = logN;
    __obliv_c__int n = __obliv_c__newInt();
    __obliv_c__int tmp_int0 = __obliv_c__newInt();
    __obliv_c__genOblivInt(tmp_int0, logN);
    __obliv_c__intCondAssign(cond, n, tmp_int0);
    // obliv int y;
    __obliv_c__int y = __obliv_c__newInt();
    // obliv int res = 0;
    __obliv_c__int res = __obliv_c__newInt();
    __obliv_c__int tmp1 = __obliv_c__newInt();
    __obliv_c__genOblivInt(tmp1, 0);
    __obliv_c__intCondAssign(cond, res, tmp1);
    // y = x >> 16;
    __obliv_c__int tmp2 = __obliv_c__newInt();
    __obliv_c__intRShift(tmp2, x, 16);
    __obliv_c__intCondAssign(cond, y, tmp2);
    // obliv if (y != 0) { n = n -16; x = y; }
        __obliv_c__bool cond_res = __obliv_c__newBool();
        // y != 0
        __obliv_c__int tmp3 = __obliv_c__newInt();
        __obliv_c__genOblivInt(tmp3, 0);
        __obliv_c__bool tmp4 = __obliv_c__newBool();
        __obliv_c__intEqual(tmp4, y, tmp3);
        __obliv_c__flipBit(tmp4.bits);
        // obliv if
        __obliv_c__boolAnd(cond_res, cond, tmp4);
        // {
            // n = n - 16;
            __obliv_c__int tmp5 = __obliv_c__newInt();
            __obliv_c__genOblivInt(tmp5, 16);
            __obliv_c__int tmp6 = __obliv_c__newInt();
            __obliv_c__intSub(tmp6, n, tmp5);
            __obliv_c__intCondAssign(cond_res, n, tmp6);
            // x = y;
            __obliv_c__intCondAssign(cond_res, x, y);
        // }
    // y = x >> 8;
    __obliv_c__int tmp7 = __obliv_c__newInt();
    __obliv_c__intRShift(tmp7, x, 8);
    __obliv_c__intCondAssign(cond, y, tmp7);
    // obliv if (y != 0) { n = n - 8; x = y; }
        __obliv_c__bool cond_res1 = __obliv_c__newBool();
        // y != 0
        __obliv_c__int tmp8 = __obliv_c__newInt();
        __obliv_c__genOblivInt(tmp8, 0);
        __obliv_c__bool tmp9 = __obliv_c__newBool();
        __obliv_c__intEqual(tmp9, y, tmp8);
        __obliv_c__flipBit(tmp9.bits);
        // obliv if
        __obliv_c__boolAnd(cond_res1, cond, tmp9);
        // {
            // n = n - 8;
            __obliv_c__int tmp10 = __obliv_c__newInt();
            __obliv_c__genOblivInt(tmp10, 8);
            __obliv_c__int tmp11 = __obliv_c__newInt();
            __obliv_c__intSub(tmp11, n, tmp10);
            __obliv_c__intCondAssign(cond_res1, n, tmp11);
            // x = y;
            __obliv_c__intCondAssign(cond_res1, x, y);
        // }
    // y = x >> 4; 
    __obliv_c__int tmp12 = __obliv_c__newInt();
    __obliv_c__intRShift(tmp12, x, 4);
    __obliv_c__intCondAssign(cond, y, tmp12);
    // obliv if (y != 0) { n = n - 4; x = y; }
        __obliv_c__bool cond_res2 = __obliv_c__newBool();
        // y != 0
        __obliv_c__int tmp13 = __obliv_c__newInt();
        __obliv_c__genOblivInt(tmp13, 0);
        __obliv_c__bool tmp14 = __obliv_c__newBool();
        __obliv_c__intEqual(tmp14, y, tmp13);
        __obliv_c__flipBit(tmp14.bits);
        // obliv if
        __obliv_c__boolAnd(cond_res2, cond, tmp14);
        // {
            // n = n - 4;
            __obliv_c__int tmp15 = __obliv_c__newInt();
            __obliv_c__genOblivInt(tmp15, 4);
            __obliv_c__int tmp16 = __obliv_c__newInt();
            __obliv_c__intSub(tmp16, n, tmp15);
            __obliv_c__intCondAssign(cond_res2, n, tmp16);
            // x = y;
            __obliv_c__intCondAssign(cond_res2, x, y);
        // }
    // y = x >> 2;
    __obliv_c__int tmp17 = __obliv_c__newInt();
    __obliv_c__intRShift(tmp17, x, 2);
    __obliv_c__intCondAssign(cond, y, tmp17);
    // obliv if (y != 0) { n = n - 2; x = y; }
        __obliv_c__bool cond_res3 = __obliv_c__newBool();
        // y != 0
        __obliv_c__int tmp18 = __obliv_c__newInt();
        __obliv_c__genOblivInt(tmp18, 0);
        __obliv_c__bool tmp19 = __obliv_c__newBool();
        __obliv_c__intEqual(tmp19, y, tmp18);
        __obliv_c__flipBit(tmp19.bits);
        // obliv if
        __obliv_c__boolAnd(cond_res3, cond, tmp19);
        // {
            // n = n - 2;
            __obliv_c__int tmp20 = __obliv_c__newInt();
            __obliv_c__genOblivInt(tmp20, 2);
            __obliv_c__int tmp21 = __obliv_c__newInt();
            __obliv_c__intSub(tmp21, n, tmp20);
            __obliv_c__intCondAssign(cond_res3, n, tmp21);
            // x = y;
            __obliv_c__intCondAssign(cond_res3, x, y);
        // }
    // y = x >> 1;
    __obliv_c__int tmp22 = __obliv_c__newInt();
    __obliv_c__intRShift(tmp22, x, 1);
    __obliv_c__intCondAssign(cond, y, tmp22);
    // obliv if (y != 0) {
        __obliv_c__bool cond_res4 = __obliv_c__newBool();
        // y != 0
        __obliv_c__int tmp23 = __obliv_c__newInt();
        __obliv_c__genOblivInt(tmp23, 0);
        __obliv_c__bool tmp24 = __obliv_c__newBool();
        __obliv_c__intEqual(tmp24, y, tmp23);
        __obliv_c__flipBit(tmp24.bits);
        // obliv if
        __obliv_c__boolAnd(cond_res4, cond, tmp24);
        // {
        // res = n - 2;
        __obliv_c__int tmp25 = __obliv_c__newInt();
        __obliv_c__genOblivInt(tmp25, 2);
        __obliv_c__int tmp26 = __obliv_c__newInt();
        __obliv_c__intSub(tmp26, n, tmp25);
        __obliv_c__intCondAssign(cond_res4, res, tmp26);
        // }
    // else {
        // res = n - x;
        __obliv_c__int tmp27 = __obliv_c__newInt();
        __obliv_c__intSub(tmp27, n, x);
        __obliv_c__intCondAssign(cond_res4, res, tmp27);
    // }
    return res;
}
