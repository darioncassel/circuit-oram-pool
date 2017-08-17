/*
#include "block.oh"
#include "ckt_utils.oh"
#include <obliv.oh>
*/
#ifndef NONORAM_H__
#define NONORAM_H__
#include <stdio.h>
#include "../obliv/obliv.h"
#include "oram.h"
#include "block.h"

#define ORAM_BUCKET_SIZE 3
#define ORAM_STASH_SIZE 33


typedef struct NonRecursiveOram{
    Block *** tree;
	Block *** path;
	Block ** stash;
    int stash_size;
	int bucket_size;
    int index_size;
	int position_label_size;
	int N;
    OcCopy* cpy;
    int eviction_cnt;
    __obliv_c__int* deepest_index;
    __obliv_c__int* deepest_depth;
    __obliv_c__int* deepest;
    __obliv_c__int* target;
	Block * hold;
	Block * to_write;
} NonRecursiveOram;

NonRecursiveOram* nro_initialize(int _N, OcCopy* cpy);
void nro_release(NonRecursiveOram * oram);

Block*** path_initialize(__obliv_c__bool cond, NonRecursiveOram* oram);
void path_release(__obliv_c__bool cond, NonRecursiveOram * oram, Block *** path);

void bucket_read_and_remove(__obliv_c__bool cond, Block** blocks, int bucket_size, __obliv_c__int index, Block* res, OcCopy* cpy);
void bucket_add(__obliv_c__bool cond, Block** blocks, int bucket_size, Block* new_block, OcCopy* cpy);
void nro_put_back(__obliv_c__bool cond, NonRecursiveOram * oram, __obliv_c__int index, __obliv_c__int position_label, void* data);

void path_read_and_remove(__obliv_c__bool cond, NonRecursiveOram* oram, __obliv_c__int index, Block* res);


void nro_flush(__obliv_c__bool cond, NonRecursiveOram * oram);
void circuit_oram_flush(__obliv_c__bool cond, NonRecursiveOram * oram, int path_val);
void path_oram_flush(__obliv_c__bool cond, NonRecursiveOram * oram, int path_val);
void get_path(__obliv_c__bool cond, NonRecursiveOram* oram, int position_label);

void nro_read(__obliv_c__bool cond, NonRecursiveOram * oram, __obliv_c__int index, 
              __obliv_c__int position_label, __obliv_c__int new_position_label, void* data);
void nro_write(__obliv_c__bool cond, NonRecursiveOram * oram, __obliv_c__int index, 
               __obliv_c__int position_label, __obliv_c__int new_position_label, void* data);


void nro_read_and_remove(__obliv_c__bool cond, NonRecursiveOram* oram, __obliv_c__int index, __obliv_c__int position_label, void* data);
void nro_put_back(__obliv_c__bool cond, NonRecursiveOram * oram, __obliv_c__int index, __obliv_c__int position_label, void* data);

void print_block(__obliv_c__bool cond, Block * b);
void print_oram(__obliv_c__bool cond, NonRecursiveOram * oram);

#endif
