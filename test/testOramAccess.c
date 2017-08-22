#include <stdio.h>
#include "../obliv/obliv.h"
#include "../oram/copy.h"
#include "../oram/circuit_oram.h"

#ifdef POOL_GARB
    #include "../Pool/ZH128/Garb/server.h"
    #include "../Pool/PoolCode/Garb/PoolAnd.h"
#endif
#ifdef POOL_EVAL
    #include "../Pool/ZH128/Eval/client.h"
#endif


void testOramAccess()
{
    CircuitSetup(2, 1, 3);
    PoolSetup();

    int n = 1000000;
    int size = 1;
    __obliv_c__int *content = calloc_mset(n, sizeof(__obliv_c__int));
    __obliv_c__int *indices = calloc_mset(size, sizeof(__obliv_c__int));
  
    // Seed content
    for (int i = 0; i < n; i++) {
        __obliv_c__int tmp0 = __obliv_c__newInt();
        __obliv_c__genOblivInt(tmp0, i);
        content[i] = tmp0;
    }

    // Seed Indicies
    for (int i = 0; i < size; i++) {
        __obliv_c__int tmp1 = __obliv_c__newInt();
        __obliv_c__genOblivInt(tmp1, i);
        indices[i] = tmp1;
    }

    // Initialize Circuit Oram
    OcOram *ram = (OcOram*) ckt_initialize(n, &ocCopyInt);
    __obliv_c__bool cond = __obliv_c__newBool();
    __obliv_c__genOblivBool(cond, true);

    // Commented out for metrics gathering:
    /* Write to ram
    for (int i = 0; i < n; ++i) {
        __obliv_c__int tmp2 = __obliv_c__newInt();
        __obliv_c__genOblivInt(tmp2, i);
        ocOramWrite(cond, ram, tmp2, content + i);
    }*/

    // Read from ram
    __obliv_c__int output = __obliv_c__newInt();
    int *outputs = _mm_malloc(size * sizeof(int), 32);
    for (int i = 0; i < size; ++i) { 
        ocOramRead(cond, &output, ram, indices[i]);
        // Commented out for metrics gathering:
        // __obliv_c__revOblivInt(outputs + i, output);
    }

    #ifdef POOL_GARB
        printf("PoolAnd count: %d\n", AndGateCount);
    #endif

    ocOramRelease(ram);
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
