#ifndef CIRCUIT_H
#define CIRCUIT_H
#include "../obliv/obliv.h"
#include "block.h"
#include "copy.h"
#include "oram.h"
#include "linear_oram.h"
#include "nonrecursive_oram.h"


typedef struct CircuitOram{
   OcOram super;
   OcCopy* poscpy;
   NonRecursiveOram ** orams;
   OcOram* base;
   int recursion_factor;
   int cutoff;
   int num_recursion;
   // BCipherRandomGen* gen;
   void* gen;
   __obliv_c__bool* rand_pool;
   int rand_pool_cap;
   int rand_pool_size;
} CircuitOram;

void get_random(__obliv_c__bool cond, CircuitOram * oram, __obliv_c__bool* res, int length);
CircuitOram* ckt_initialize(int N, OcCopy* cpy);
CircuitOram* ckt_initialize_more_param(int N, OcCopy* cpy, int recursion_factor, int cutoff);
void ckt_release(OcOram* oram);

void ckt_read(__obliv_c__bool cond, CircuitOram* oram, __obliv_c__int index, void* data);
void ckt_write(__obliv_c__bool cond, CircuitOram* oram, __obliv_c__int index, void* data);
void ckt_apply(__obliv_c__bool cond, OcOram* super, __obliv_c__int index, OcOramAccessCb func, void * ext_data);

void update_trees(__obliv_c__bool cond, CircuitOram* oram, __obliv_c__int index, int level, __obliv_c__int* pos, __obliv_c__int* new_pos);

void extract_bits(__obliv_c__bool cond, __obliv_c__bool* data_chunk, __obliv_c__int ith_pos, int entry_size, __obliv_c__bool* res, CircuitOram * oram);
void put_bits(__obliv_c__bool cond, __obliv_c__bool* data_chunk, __obliv_c__int ith_pos, int entry_size, __obliv_c__bool* res, CircuitOram * oram);

void get_rand_obliv(__obliv_c__bool cond, /*BCipherRandomGen*/void* gen, __obliv_c__bool * data, int len); // obliv

#endif
