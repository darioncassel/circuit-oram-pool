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

    int n = 258;
    int size = 1;
    
    #ifdef DATA_1024
        typedef struct {
            __obliv_c__int data[32];
        } data_1024;
        data_1024 *content = calloc_mset(n, sizeof(data_1024));
        OramMCPoolSetup(1024);
    #else
        __obliv_c__int *content = calloc_mset(n, sizeof(__obliv_c__int));
        OramMCPoolSetup(32);
    #endif

    __obliv_c__int *indices = calloc_mset(size, sizeof(__obliv_c__int));

    // Seed content
    for (int i = 0; i < n; i++) {
        #ifdef DATA_1024
            data_1024 tmp0;
            for (int j = 0; j < 32; j++) {
                __obliv_c__int tmp1 = __obliv_c__newInt();
                __obliv_c__genOblivInt(tmp1, j);
                tmp0.data[j] = tmp1;
            }
        #else
            __obliv_c__int tmp0 = __obliv_c__newInt();
            __obliv_c__genOblivInt(tmp0, i);
        #endif
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

    #ifdef DATA_1024
        data_1024 output;
        for (int j = 0; j < 32; j++) {
            __obliv_c__int tmp1 = __obliv_c__newInt();
            __obliv_c__genOblivInt(tmp1, j);
            output.data[j] = tmp1;
        }
    #else
        __obliv_c__int output = __obliv_c__newInt();
    #endif

    // Commented out for metrics gathering:
    // int *outputs = _mm_malloc(size * sizeof(int), 32);

    unsigned long long t0, t1 = 0; t0 = t1;
    unsigned long long b0, b1 = 0; b0 = b1;
    b0 = inByte + outByte;
    #ifndef MAC
        t0 = WallClock();
    #endif

    // Read from ram
    for (int i = 0; i < size; ++i) { 
        ocOramRead(cond, &output, ram, indices[i]);
        // Commented out for metrics gathering:
        // __obliv_c__revOblivInt(outputs + i, output);
    }

    #ifndef MAC
        t1 = WallClock();
    #endif
    b1 = inByte + outByte;
    printf("time = %lf\n", (t1 - t0) * 1.0 / 1000000);
    printf("B/W = %lf\n", (b1 - b0) * 1.0 / 1000000);

    #ifdef POOL_GARB
        printf("PoolAnd count: %d\n", AndGateCount);
    #endif

    ocOramRelease(ram);
    _mm_free(indices);
    _mm_free(content);
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
