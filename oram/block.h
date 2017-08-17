#ifndef BLOCK_H
#define BLOCK_H
#include "../obliv/obliv.h"
#include "copy.h"


typedef struct Block {
   __obliv_c__int index;
   __obliv_c__bool is_dummy;
   __obliv_c__int position_label;
   void* data;
} Block;

Block* block_initialize(__obliv_c__bool cond, OcCopy* cpy);
void block_release(__obliv_c__bool cond, Block* b);
void block_copy(__obliv_c__bool cond, Block* a, Block* des, OcCopy* cpy);

#endif
