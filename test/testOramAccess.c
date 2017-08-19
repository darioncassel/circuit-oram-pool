#include <stdio.h>
#include "../obliv/obliv.h"
#include "../oram/copy.h"
#include "../oram/circuit_oram.h"

#ifdef POOL_GARB
    #include "../Pool/ZH128/Garb/server.h"
#endif
#ifdef POOL_EVAL
    #include "../Pool/ZH128/Eval/client.h"
#endif


void testOramAccess()
{
    CircuitSetup(2, 1, 3);
    PoolSetup();

    int n = 1000;
    int size = 1;
    __obliv_c__bool *content = calloc(n, sizeof(__obliv_c__bool));
    __obliv_c__bool *indices = calloc(size, sizeof(__obliv_c__bool));
  
    // Seed content
    for (int i = 0; i < n; i++) {
        __obliv_c__bool tmp0 = __obliv_c__newBool();
        __obliv_c__genOblivBool(tmp0, (bool) i);
        content[i] = tmp0;
    }

    // Seed Indicies
    for (int i = 0; i < size; i++) {
        __obliv_c__bool tmp1 = __obliv_c__newBool();
        __obliv_c__genOblivBool(tmp1, (bool) i);
        indices[i] = tmp1;
    }

    __obliv_c__bool *output = calloc(n, sizeof(__obliv_c__bool));
    for (int i = 0; i < n; i++) {
        __obliv_c__bool tmp0 = content[i];
        for (int i = 0; i < size; i++) {
            __obliv_c__bool tmp1 = indices[i];
            __obliv_c__bool tmp2 = __obliv_c__newBool();
            __obliv_c__boolAnd(tmp2, tmp1, tmp0);
            __obliv_c__boolXor(tmp2, tmp1, tmp0);
            output[i] = tmp2;
        }
    }

    // Initialize Circuit Oram
    // OcOram *ram = (OcOram*) ckt_initialize(n, &ocCopyInt);
    // __obliv_c__bool cond = __obliv_c__newBool();
    // __obliv_c__genOblivBool(cond, true);

    // Commented out for metrics gathering:
    /* Write to ram
    for (int i = 0; i < n; ++i) {
        __obliv_c__int tmp2 = __obliv_c__newInt();
        __obliv_c__genOblivInt(tmp2, i);
        ocOramWrite(cond, ram, tmp2, content + i);
    }*/

    // Read from ram
    // __obliv_c__int output = __obliv_c__newInt();
    // int *outputs = _mm_malloc(size * sizeof(int), 32);
    // for (int i = 0; i < size; ++i) { 
    //     ocOramRead(cond, &output, ram, indices[i]);
        // Commented out for metrics gathering:
        // __obliv_c__revOblivInt(outputs + i, output);
    // }

    // ocOramRelease(ram);
    // _mm_free(indices);
    // _mm_free(content);
}

int main(int argc, char ** argv) 
{
    int c;
    c = atoi(argv[1]);
    opensocket(c);
    printf("Beginning test...\n");
    testOramAccess();
    printf("%lld\n", __debug__AND_count);
    printf("%lld\n", __debug__XOR_count);
    printf("Test complete.\n");
}
