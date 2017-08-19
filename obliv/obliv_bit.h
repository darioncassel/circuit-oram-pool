#ifndef OBLIV_BIT_H
#define OBLIV_BIT_H

#ifdef POOL_EVAL
    #include "../Pool/ZH128/Eval/InputProcess.h"
    #include "../Pool/ZH128/Eval/CircuitBuild.h"
#endif
#ifdef POOL_GARB
    #include "../Pool/ZH128/Garb/InputProcess.h"
    #include "../Pool/ZH128/Garb/CircuitBuild.h"
#endif

#define bool int
#define true 1
#define false 0

#ifndef NULL
    #define NULL 0
#endif

#define __obliv_c__size_t long unsigned int

#define EXIT_FAILURE -1


typedef struct {
    bool unknown;
    union {
        bool knownValue;
        struct {
            union {
                bool value;
                bool inverted;
            };
        } yao;
        struct {
            #ifdef POOL_GARB
                Wire w;
            #endif
            #ifdef POOL_EVAL
                WireE wE;
            #endif
        } pool;
    };
} OblivBit;

#endif
