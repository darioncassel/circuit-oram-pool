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


void testOramAccess(int ram_size)
{
    // Setup Pool
    CircuitSetup(2, 1, 3);
    PoolSetup();

    #ifdef DATA_1024 
        OramMCPoolSetup(1024);
    #else
        OramMCPoolSetup(32);
    #endif

    int n = ram_size;
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
    printf("Num recur: %d\n", ((CircuitOram*)ram)->num_recursion-1);
    __obliv_c__bool cond = __obliv_c__newBool();
    __obliv_c__genOblivBool(cond, true);

    __obliv_c__int output = __obliv_c__newInt();

    // Benchmark begin
    unsigned long long t0, t1 = 0; t0 = t1;
    unsigned long long b0, b1 = 0; b0 = b1;
    b0 = inByte + outByte;
    #ifndef MAC
        t0 = wallClock();
    #endif

    // Read from ram
    for (int i = 0; i < size; ++i) { 
        ocOramRead(cond, &output, ram, indices[i]);
    }

    // Benchmark end
    #ifndef MAC
        t1 = wallClock();
    #endif
    b1 = inByte + outByte;
    printf("time = %lf\n", (t1 - t0) * 1.0 / 1000000);
    printf("B/W = %lf\n", (b1 - b0) * 1.0 / 1000000);

    ocOramRelease(ram);
    _mm_free(indices);
    _mm_free(content);
}

int main(int argc, char ** argv) 
{
    if (argc != 3) {
        printf("Error: Invalid arguments specified!\n");
        printf("Args: 1 - Connection port\n");
        printf("      2 - ORAM size\n");
        printf("Ex: ./pool_garb 1234 100\n");
        exit(EXIT_FAILURE);
    }
    int c = atoi(argv[1]);
    int n = atoi(argv[2]);
    opensocket(c);
    printf("Beginning test...\n");
    testOramAccess(n);
    printf("%lld\n", __debug__AND_count);
    printf("%lld\n", __debug__XOR_count);
    printf("Test complete.\n");
}
