#include "block.h"


Block* block_initialize(__obliv_c__bool cond, OcCopy* cpy) 
{
	Block* res = calloc_obliv(cond, sizeof(Block), 1);
	res->data = calloc_obliv(cond, 1, cpy->eltsize);
    // res->is_dummy = true;
    __obliv_c__bool tmp0 = __obliv_c__newBool();
    __obliv_c__genOblivBool(tmp0, true);
    __obliv_c__boolCondAssign(cond, res->is_dummy, tmp0);
	return res;	
}

void block_release(__obliv_c__bool cond, Block* b) 
{
	free_obliv(cond, b->data);
	free_obliv(cond, b);
}

void block_copy(__obliv_c__bool cond, Block* block, Block * des, OcCopy* cpy) 
{
	// des->index = block->index;
    __obliv_c__intCondAssign(cond, des->index, block->index);
	// des->is_dummy = block->is_dummy;
    __obliv_c__boolCondAssign(cond, des->is_dummy, block->is_dummy);
	// des->position_label = block->position_label;
    __obliv_c__intCondAssign(cond, des->position_label, block->position_label);
    ocCopy(cond, cpy, des->data, block->data);
}
