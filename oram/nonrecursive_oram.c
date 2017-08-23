#include "nonrecursive_oram.h"


void print_block(__obliv_c__bool cond, Block * b) // obliv
{
	// ~obliv(){
		int tmp, tmp2;
		// revealOblivInt(&tmp, b->index, 0);
		__obliv_c__revOblivInt(&tmp, b->index);
		// revealOblivBool(&tmp2, b->is_dummy, 0);
		__obliv_c__revOblivBool(&tmp2, b->is_dummy);
		// if (ocCurrentParty() == 1)
		printf("Idx:%d;isD:%d  ", tmp, tmp2); 
	// }
}

void print_oram(__obliv_c__bool cond, NonRecursiveOram * oram) // obliv 
{
	// ~obliv() {
		// if (ocCurrentParty() == 1)
		// 	printf("%s", "stash:"); 
		for(int i = 0; i < oram->stash_size && i < 10; ++i)
			print_block(cond, oram->stash[i]);
		printf("%s", "\n"); 
	// }
}

NonRecursiveOram* nro_initialize(int _N, OcCopy* cpy) 
{
	__obliv_c__bool cond = __obliv_c__newBool();
	__obliv_c__genOblivBool(cond, true);
	NonRecursiveOram* res = calloc_obliv(cond, 1, sizeof(NonRecursiveOram));
	res->stash_size = ORAM_STASH_SIZE;
	res->bucket_size = ORAM_BUCKET_SIZE;
	res->cpy = cpy;

	int a = 1;
	int logN = 1;

	while (a < _N) {
		a *= 2;
		++logN;
	}

	--logN;
	res->index_size = logN;
	res->position_label_size = logN;
	res->N = 1 << logN;

	res->tree = calloc_obliv(cond, res->N, sizeof(Block**));
	res->stash = calloc_obliv(cond, res->stash_size, sizeof(Block*));

	for(int i = 0; i < res->N; ++i) {
		res->tree[i] = calloc_obliv(cond, res->bucket_size, sizeof(Block*));
		for(int j = 0; j < res->bucket_size; ++j) {
			res->tree[i][j] = block_initialize(cond, res->cpy);
		}
	}
	
	for(int i = 0; i < res->stash_size; ++i) {
		res->stash[i] = block_initialize(cond, res->cpy);
	}

	res->eviction_cnt = 0;
	res->deepest_index = calloc_obliv(cond, res->index_size + 1, sizeof(__obliv_c__int));
	res->deepest_depth = calloc_obliv(cond, res->index_size + 1, sizeof(__obliv_c__int));
	res->deepest = calloc_obliv(cond, res->index_size + 1, sizeof(__obliv_c__int));
	res->target = calloc_obliv(cond, res->index_size + 1, sizeof(__obliv_c__int));

	res->hold = block_initialize(cond, res->cpy); 
	res->to_write = block_initialize(cond, res->cpy); 
	res->path = path_initialize(cond, res);
	return res;
}

void nro_release(NonRecursiveOram* oram) {
	__obliv_c__bool cond = __obliv_c__newBool();
	__obliv_c__genOblivBool(cond, true);
	for(int i = 0; i < oram->N; ++i) {
		for(int j = 0; j < oram->bucket_size; ++j)
			block_release(cond, oram->tree[i][j]);
		free_obliv(cond, oram->tree[i]);
	}
	free_obliv(cond, oram->tree);
	for(int i = 0; i < oram->stash_size; ++i)
		block_release(cond, oram->stash[i]);
	free_obliv(cond, oram->stash);
	free_obliv(cond, oram->deepest_index);
	free_obliv(cond, oram->deepest_depth);
	free_obliv(cond, oram->deepest);
	free_obliv(cond, oram->target);
	block_release(cond, oram->hold);
	block_release(cond, oram->to_write);
	path_release(cond, oram, oram->path);
	free_obliv(cond, oram);
}


Block*** path_initialize(__obliv_c__bool cond, NonRecursiveOram* oram) // obliv
{
	Block*** path = calloc_obliv(cond, sizeof(Block**), oram->index_size + 1);
	path[0] = calloc_obliv(cond, sizeof(Block*), oram->stash_size);
	for(int i = 0; i < oram->index_size; ++i) {
		path[i + 1] = calloc_obliv(cond, sizeof(Block*), oram->bucket_size);
	}
	return path;
}

void path_release(__obliv_c__bool cond, NonRecursiveOram * oram, Block *** path) // obliv
{
	for(int i = 0; i < oram->index_size + 1; ++i) {
		free_obliv(cond, path[i]);
	}
	free_obliv(cond, path);
}

void bucket_read_and_remove(__obliv_c__bool cond, Block** blocks, int bucket_size, __obliv_c__int index, Block* res, OcCopy* cpy) // obliv
{
	// TODO: why stash size instead of bucket_size?
	for(int i = 0; i < ORAM_STASH_SIZE; ++i) {
		// obliv if (blocks[i]->index == index & ( !blocks[i]->is_dummy)) {
			__obliv_c__bool cond_res = __obliv_c__newBool();
			__obliv_c__genOblivBool(cond_res, false);
			__obliv_c__bool tmp0 = __obliv_c__newBool();
			__obliv_c__genOblivBool(tmp0, false);
			// !blocks[i]->is_dummy
			if (blocks[i]->is_dummy.bits == NULL) {
				blocks[i]->is_dummy = __obliv_c__newBool();
				__obliv_c__genOblivBool(blocks[i]->is_dummy, false);
			}
			tmp0.bits = blocks[i]->is_dummy.bits;
			__obliv_c__flipBit(tmp0.bits);
			__obliv_c__bool tmp1 = __obliv_c__newBool();
			__obliv_c__genOblivBool(tmp1, false);

			// blocks[i]->index == index
			__obliv_c__bool tmp2 = __obliv_c__newBool();
			__obliv_c__genOblivBool(tmp2, false);
			if (blocks[i]->index.bits == NULL) {
				blocks[i]->index = __obliv_c__newInt();
				__obliv_c__genOblivInt(blocks[i]->index, 0);
			}
			__obliv_c__intEqual(tmp2, blocks[i]->index, index);
			// blocks[i]->index == index & ( !blocks[i]->is_dummy)
			__obliv_c__bool tmp3 = __obliv_c__newBool();
			__obliv_c__genOblivBool(tmp3, false);
			__obliv_c__boolAnd(tmp3, tmp2, tmp1);
			// obliv if
			__obliv_c__boolAnd(cond_res, cond, tmp3);
			// {
			block_copy(cond_res, blocks[i], res, cpy);
			// blocks[i]->is_dummy = true;
			__obliv_c__bool tmp4 = __obliv_c__newBool();
			__obliv_c__genOblivBool(tmp4, true);
			blocks[i]->is_dummy = __obliv_c__newBool();
			__obliv_c__genOblivBool(blocks[i]->is_dummy, false);
			__obliv_c__boolCondAssign(cond_res, blocks[i]->is_dummy, tmp4);
		// }
	}
}

void bucket_add(__obliv_c__bool cond, Block** blocks, int bucket_size, Block* new_block, OcCopy* cpy) // obliv
{
	__obliv_c__bool added = __obliv_c__newBool();
	__obliv_c__genOblivBool(added, false);
	for(int i = 0; i < bucket_size; ++i) {
		// obliv if( blocks[i]->is_dummy & (!added) ) {
			__obliv_c__bool cond_res = __obliv_c__newBool();
			__obliv_c__genOblivBool(cond_res, false);
			// (!added)
			__obliv_c__bool tmp0 = __obliv_c__newBool();
			__obliv_c__genOblivBool(tmp0, false);
			__obliv_c__copyBit(tmp0.bits, added.bits);
			__obliv_c__flipBit(tmp0.bits);
			// blocks[i]->is_dummy & (!added)
			__obliv_c__bool tmp1 = __obliv_c__newBool();
			__obliv_c__genOblivBool(tmp1, false);
			__obliv_c__boolAnd(tmp1, blocks[i]->is_dummy, tmp0);

			__obliv_c__boolAnd(cond_res, cond, tmp1);
			block_copy(cond_res, new_block, blocks[i], cpy);

			// added = true;
			__obliv_c__bool tmp2 = __obliv_c__newBool();
			__obliv_c__genOblivBool(tmp2, true);
			added = __obliv_c__newBool();
			__obliv_c__genOblivBool(added, false);
			__obliv_c__boolCondAssign(cond_res, added, tmp2);
		// }
	}
}

void path_read_and_remove(__obliv_c__bool cond, NonRecursiveOram* oram, __obliv_c__int index, Block* res) // obliv
{
	int bucket_size = oram->index_size * oram->bucket_size + oram->stash_size;
	Block **arr = calloc_obliv(cond, bucket_size, sizeof(Block*));
	int cnt = 0;

	for(int i = 0; i < oram->stash_size; ++i) {
		arr[cnt++] = oram->path[0][i];
	}

	for(int i = 0; i < oram->index_size; ++i) {
		for(int j = 0; j < oram->bucket_size; ++j) {
			arr[cnt++] = oram->path[i+1][j];
		}
	}

	bucket_read_and_remove(cond, arr, bucket_size, index, res, oram->cpy);
	free_obliv(cond, arr);
}

void nro_read_and_remove(__obliv_c__bool cond, NonRecursiveOram* oram, __obliv_c__int index, __obliv_c__int position_label, void* data) // obliv
{
	int position_label_clear = 0;
	// ~obliv() {
		//     struct timeval tv;
		//    gettimeofday(&tv, 0);
		//   printf("%d, %d s, %d milli\n", ocCurrentParty(), tv.tv_sec, tv.tv_usec/1000);
	__obliv_c__revOblivInt(&position_label_clear, position_label);
	// }
	get_path(cond, oram, position_label_clear);
	Block* res_block = block_initialize(cond, oram->cpy);   
	path_read_and_remove(cond, oram, index, res_block);

    ocCopy(cond, oram->cpy,data,res_block->data);
	block_release(cond, res_block);
}

void nro_put_back(__obliv_c__bool cond, NonRecursiveOram * oram, 
			      __obliv_c__int index, __obliv_c__int position_label, 
				  void* data) // obliv
{
	Block* des = block_initialize(cond, oram->cpy);

	// des->index = index;
	__obliv_c__intCondAssign(cond, des->index, index);

	// des->is_dummy = false;
	__obliv_c__bool tmp1 = __obliv_c__newBool();
	__obliv_c__genOblivBool(tmp1, false);
	__obliv_c__boolCondAssign(cond, des->is_dummy, tmp1);

	// des->position_label = position_label;
	__obliv_c__intCondAssign(cond, des->position_label, position_label);

    ocCopy(cond,oram->cpy,des->data,data);
	bucket_add(cond, oram->stash, oram->stash_size, des, oram->cpy);
	block_release(cond, des);  
	nro_flush(cond, oram);
	nro_flush(cond, oram);
}

void get_path(__obliv_c__bool cond, NonRecursiveOram* oram, int position_label) // obliv 
{
	int index = 1;

	for(int i = 0; i < oram->stash_size; ++i) {
		// oram->path[0][i] = oram->stash[i];
		oram->path[0][i] = oram->stash[i];
	}

	for(int i = 0; i < 1; ++i) {
		// oram->path[1][i] = oram->tree[index][i];
		oram->path[1][i] = oram->tree[index][i];
	}

	for(int i = 1; i < oram->index_size; ++i) {
		index *= 2;
		if((position_label >> (oram->position_label_size - i ) & 1) == 1)
			index++;
		for(int j = 0; j < oram->bucket_size; ++j) {
			oram->path[i + 1][j] = oram->tree[index][j];
		}
	}
}


void nro_read(__obliv_c__bool cond, NonRecursiveOram * oram, __obliv_c__int index, 
	          __obliv_c__int position_label, __obliv_c__int new_position_label, 
			  void* data) // obliv
{ 
	nro_read_and_remove(cond, oram, index, position_label, data);
	nro_put_back(cond, oram, index, new_position_label, data);
}

void nro_write(__obliv_c__bool cond, NonRecursiveOram * oram, __obliv_c__int index, __obliv_c__int position_label, 
	          __obliv_c__int new_position_label, void* data) 
{
	void* tmp = calloc_obliv(cond, 1, oram->cpy->eltsize);
	nro_read_and_remove(cond, oram, index, position_label, tmp);
	nro_put_back(cond, oram, index, new_position_label, data);
	free_obliv(cond, tmp);
}

void nro_flush(__obliv_c__bool cond, NonRecursiveOram * oram) {
	int res = 0;
	int temp = oram->eviction_cnt;
	for (int i = oram->index_size - 1; i >= 0; --i) {
		res = res | ((temp & 1) << i);
		temp >>= 1;
	}
	oram->eviction_cnt = (oram->eviction_cnt + 1) % oram->N;
	get_path(cond, oram, res);
	// circuit_oram_flush(oram, res);
	circuit_oram_flush(cond, oram, res);
	// print_oram(oram);
	/*print_oram(cond, oram);*/
}


void path_oram_flush(__obliv_c__bool cond, NonRecursiveOram * oram, int path_val) // obliv
{
	for(int i = oram->index_size; i > 0; --i) {
		for(int j = 0; j < oram->bucket_size; ++j) {
			// obliv bool added = !oram->path[i][j]->is_dummy;
			__obliv_c__bool added = __obliv_c__newBool();
			__obliv_c__bool tmp0 = __obliv_c__newBool();
			__obliv_c__copyBit(tmp0.bits, oram->path[i][j]->is_dummy.bits);
			__obliv_c__flipBit(tmp0.bits);
			__obliv_c__copyBit(added.bits, tmp0.bits);
			for(int k = 0; k < i; ++k)  {
				int cap = oram->bucket_size;
				if (k == 0) cap = oram->stash_size;
				for(int l = 0; l < cap; ++l) {
					// obliv int depth = compute_depth(oram->path[k][l]->position_label, path_val, oram->index_size);
					__obliv_c__int depth = __obliv_c__newInt();
					__obliv_c__int tmp1 = __obliv_c__newInt();
					__obliv_c__int tmp_int5 = __obliv_c__newInt();
					__obliv_c__genOblivInt(tmp_int5, path_val);
					tmp1 = compute_depth(cond, oram->path[k][l]->position_label, tmp_int5, oram->index_size);
					__obliv_c__intCondAssign(cond, depth, tmp1);
					// obliv if(depth >= i & !oram->path[k][l]->is_dummy & !added) {
						__obliv_c__bool cond_res = __obliv_c__newBool();
						// !added
						__obliv_c__bool tmp2 = __obliv_c__newBool();
						__obliv_c__copyBit(tmp2.bits, added.bits);
						__obliv_c__flipBit(tmp2.bits);
						// !oram->path[k][l]->is_dummy
						__obliv_c__bool tmp3 = __obliv_c__newBool();
						__obliv_c__copyBit(tmp3.bits, oram->path[k][l]->is_dummy.bits);
						__obliv_c__flipBit(tmp3.bits);
						// !oram->path[k][l]->is_dummy & !added
						__obliv_c__bool tmp4 = __obliv_c__newBool();
						__obliv_c__boolAnd(tmp4, tmp3, tmp2);
						// depth >= i
						__obliv_c__int tmp5 = __obliv_c__newInt();
						__obliv_c__genOblivInt(tmp5, i);
						__obliv_c__bool tmp6 = __obliv_c__newBool();
						__obliv_c__intGrEq(tmp6, depth, tmp5);
						// depth >= i & !oram->path[k][l]->is_dummy & !added
						__obliv_c__bool tmp7 = __obliv_c__newBool();
						__obliv_c__boolAnd(tmp7, tmp6, tmp4);
						
						__obliv_c__boolAnd(cond_res, cond, tmp7);

						block_copy(cond_res, oram->path[k][l], oram->path[i][j], oram->cpy);

						// oram->path[k][l]->is_dummy = true;
						__obliv_c__bool tmp8 = __obliv_c__newBool();
						__obliv_c__genOblivBool(tmp8, true);
						__obliv_c__boolCondAssign(cond_res, oram->path[k][l]->is_dummy, tmp8);;
						
						// added = true;
						__obliv_c__bool tmp9 = __obliv_c__newBool();
						__obliv_c__genOblivBool(tmp9, true);
						__obliv_c__boolCondAssign(cond_res, added, tmp9);
					
					// }
				}
			}
		}
	}
}

void circuit_oram_flush(__obliv_c__bool cond, NonRecursiveOram * oram, int path_val) // obliv
{
	for (int i = 0; i <= oram->index_size; ++i) {	
		int cap = oram->bucket_size;
		if (i == 0)
			cap = oram->stash_size;
		// oram->deepest_index[i] = -1;
		__obliv_c__int tmp0 = __obliv_c__newInt();
		__obliv_c__genOblivInt(tmp0, -1);
		__obliv_c__intCondAssign(cond, oram->deepest_index[i], tmp0);
		// oram->deepest_depth[i] = -1;
		__obliv_c__int tmp1 = __obliv_c__newInt();
		__obliv_c__genOblivInt(tmp1, -1);
		__obliv_c__intCondAssign(cond, oram->deepest_depth[i], tmp1);
		for(int j = 0; j < cap; ++j) {
			// obliv block_depth = compute_depth(oram->path[i][j]->position_label, path_val, oram->index_size);
			__obliv_c__int block_depth = __obliv_c__newInt();
			__obliv_c__int tmp2 = __obliv_c__newInt();
			__obliv_c__int tmp_int4 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp_int4, path_val);
			/*tmp2 = compute_depth(cond, oram->path[i][j]->position_label, tmp_int4, oram->index_size);
			__obliv_c__intCondAssign(cond, block_depth, tmp2);
			// obliv if (block_depth > oram->deepest_depth[i] & !oram->path[i][j]->is_dummy) {
				__obliv_c__bool cond_res = __obliv_c__newBool();
				// !oram->path[i][j]->is_dummy
				__obliv_c__bool tmp3 = __obliv_c__newBool();
				__obliv_c__copyBit(tmp3.bits, oram->path[i][j]->is_dummy.bits);
				__obliv_c__flipBit(tmp3.bits);
				// block_depth > oram->deepest_depth[i]
				__obliv_c__bool tmp4 = __obliv_c__newBool();
				__obliv_c__intGr(tmp4, block_depth, oram->deepest_depth[i]);
				// block_depth > oram->deepest_depth[i] & !oram->path[i][j]->is_dummy
				__obliv_c__bool tmp5 = __obliv_c__newBool();
				__obliv_c__boolAnd(tmp5, tmp4, tmp3);

				__obliv_c__boolAnd(cond_res, cond, tmp5);

				// oram->deepest_depth[i] = block_depth;
				__obliv_c__intCondAssign(cond_res, oram->deepest_depth[i], block_depth);
				// oram->deepest_index[i] = j;
				__obliv_c__int tmp6 = __obliv_c__newInt();
				__obliv_c__genOblivInt(tmp6, j);
				__obliv_c__intCondAssign(cond_res, oram->deepest_index[i], tmp6);
			// }*/
		}
	}
	/*~obliv(){
	  for (int i = 0; i < oram->index_size; ++i) {
	  int tmp;
	  revealOblivInt(&tmp, deepest_depth[i], 0);
	  printf("%d: dpeth,%d  ",i,  tmp);
	  revealOblivInt(&tmp, deepest_index[i], 0);
	  printf("index, %d\n", tmp);
	  }
	  }*/

	//prepare deepest
	/*for (int i = 0; i < oram->index_size+1; ++i) {
		// oram->deepest[i] = oram->target[i] = -1;
		__obliv_c__int tmp7 = __obliv_c__newInt();
		__obliv_c__genOblivInt(tmp7, -1);
		__obliv_c__intCondAssign(cond, oram->deepest[i], tmp7);
		__obliv_c__intCondAssign(cond, oram->target[i], tmp7);
	}

	// obliv int src = -1;
	__obliv_c__int src = __obliv_c__newInt();
	__obliv_c__int tmp8 = __obliv_c__newInt();
	__obliv_c__genOblivInt(tmp8, -1);
	__obliv_c__intCondAssign(cond, src, tmp8);
	// obliv int goal = -1;
	__obliv_c__int goal = __obliv_c__newInt();
	__obliv_c__int tmp9 = __obliv_c__newInt();
	__obliv_c__genOblivInt(tmp9, -1);
	__obliv_c__intCondAssign(cond, goal, tmp9);
	// obliv bool is_stash_empty = true;
	__obliv_c__bool is_stash_empty = __obliv_c__newBool();
	__obliv_c__bool tmp10 = __obliv_c__newBool();
	__obliv_c__genOblivBool(tmp10, true);
	__obliv_c__boolCondAssign(cond, is_stash_empty, tmp10);

	for(int i = 0; i < oram->stash_size; ++i) {
		// is_stash_empty = is_stash_empty & oram->stash[i]->is_dummy; 
		__obliv_c__bool tmp11 = __obliv_c__newBool();
		__obliv_c__boolAnd(tmp11, is_stash_empty, oram->stash[i]->is_dummy);
		__obliv_c__boolCondAssign(cond, is_stash_empty, tmp11);
	}
	// obliv if (!is_stash_empty) {
		__obliv_c__bool cond_res = __obliv_c__newBool();
		__obliv_c__bool tmp12 = __obliv_c__newBool();
		__obliv_c__copyBit(tmp12.bits, is_stash_empty.bits);
		__obliv_c__flipBit(tmp12.bits);
		__obliv_c__boolAnd(cond_res, cond, tmp12);
		
		// src = 0;
		__obliv_c__int tmp13 = __obliv_c__newInt();
		__obliv_c__genOblivInt(tmp13, 0);
		__obliv_c__intCondAssign(cond_res, src, tmp13);
		// goal = oram->deepest_depth[0];
		__obliv_c__intCondAssign(cond_res, goal, oram->deepest_depth[0]);
	// }

	for(int i = 1; i <= oram->index_size; ++i) {
		// obliv if (goal >= i)
			__obliv_c__bool cond_res2 = __obliv_c__newBool();
			__obliv_c__int tmp14 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp14, i);
			__obliv_c__bool tmp15 = __obliv_c__newBool();
			__obliv_c__intGrEq(tmp15, goal, tmp14);
			__obliv_c__boolAnd(cond_res2, cond, tmp15);
			// oram->deepest[i] = src;
			__obliv_c__intCondAssign(cond_res2, oram->deepest[i], src);
			// obliv int l = oram->deepest_depth[i];
			__obliv_c__int l = __obliv_c__newInt();
			__obliv_c__intCondAssign(cond, l, oram->deepest_depth[i]);
			// obliv if (l > goal) {
			__obliv_c__bool cond_res3 = __obliv_c__newBool();
			__obliv_c__bool tmp16 = __obliv_c__newBool();
			__obliv_c__intGr(tmp16, l, goal);
			__obliv_c__boolAnd(cond_res3, cond, tmp16);
			// goal = l;
			__obliv_c__intCondAssign(cond_res3, goal, l);
			// src = i;
			__obliv_c__int tmp_int3 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp_int3, i);
			__obliv_c__intCondAssign(cond_res3, src, tmp_int3);
		// }
	}*/

	/*~obliv(){
	  for (int i = 0; i < oram->index_size; ++i) {
	  int tmp;
	  revealOblivInt(&tmp, deepest[i], 0);
	  printf("deepest, %d\n", tmp);
	  }
	  }*/

	//prepare target
	// obliv int dest = -1;
	/*__obliv_c__int dest = __obliv_c__newInt();
	__obliv_c__int tmp17 = __obliv_c__newInt();
	__obliv_c__genOblivInt(tmp17, -1);
	__obliv_c__intCondAssign(cond, dest, tmp17);
	// src = -1;
	__obliv_c__int tmp18 = __obliv_c__newInt();
	__obliv_c__genOblivInt(tmp18, -1);
	__obliv_c__intCondAssign(cond, src, tmp18);

	for(int i = oram->index_size; i >= 0; --i) {
		// obliv if (i == src) {
			__obliv_c__bool cond_res4 = __obliv_c__newBool();
			__obliv_c__int tmp19 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp19, i);
			__obliv_c__bool tmp20 = __obliv_c__newBool();
			__obliv_c__intEqual(tmp20, tmp19, src);
			// oram->target[i] = dest;
			__obliv_c__intCondAssign(cond_res4, oram->target[i], dest);
			// dest = -1;
			__obliv_c__int tmp21 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp21, -1);
			__obliv_c__intCondAssign(cond_res4, dest, tmp21);
			// src = -1;
			__obliv_c__int tmp22 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp22, -1);
			__obliv_c__intCondAssign(cond_res4, src, tmp22);
		// }

		// obliv bool not_full = false;
		__obliv_c__bool not_full = __obliv_c__newBool();
		__obliv_c__bool tmp23 = __obliv_c__newBool();
		__obliv_c__genOblivBool(tmp23, false);
		__obliv_c__boolCondAssign(cond, not_full, tmp23);
	
		int cap = oram->bucket_size;
		if (i == 0)
			cap = oram->stash_size;
		for (int j = 0; j < cap; ++j) {
			// not_full = not_full | oram->path[i][j]->is_dummy;
			__obliv_c__bool tmp24 = __obliv_c__newBool();
			__obliv_c__boolOr(tmp24, not_full, oram->path[i][j]->is_dummy);
			__obliv_c__boolCondAssign(cond, not_full, tmp24);
		}
		// obliv if ( ((dest == -1 & not_full ) | oram->target[i] != -1 ) & oram->deepest[i] != -1) {
			__obliv_c__bool cond_res5 = __obliv_c__newBool();
			// oram->deepest[i] != -1
			__obliv_c__int tmp25 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp25, -1);
			__obliv_c__bool tmp26 = __obliv_c__newBool();
			__obliv_c__intEqual(tmp26, oram->deepest[i], tmp25);
			__obliv_c__flipBit(tmp26.bits);
			// oram->target[i] != -1
			__obliv_c__int tmp27 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp27, -1);
			__obliv_c__bool tmp28 = __obliv_c__newBool();
			__obliv_c__intEqual(tmp28, oram->target[i], tmp27);
			__obliv_c__flipBit(tmp28.bits);
			// dest == -1
			__obliv_c__int tmp29 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp29, -1);
			__obliv_c__bool tmp30 = __obliv_c__newBool();
			__obliv_c__intEqual(tmp30, dest, tmp29);
			// dest == -1 & not_full
			__obliv_c__bool tmp31 = __obliv_c__newBool();
			__obliv_c__boolAnd(tmp31, tmp30, not_full);
			// (dest == -1 & not_full ) | oram->target[i] != -1)
			__obliv_c__bool tmp32 = __obliv_c__newBool();
			__obliv_c__boolOr(tmp32, tmp31, tmp28);
			// (dest == -1 & not_full ) | oram->target[i] != -1) & oram->deepest[i] != -1)
			__obliv_c__bool tmp33 = __obliv_c__newBool();
			__obliv_c__boolAnd(tmp33, tmp32, tmp26);
			// obliv if
			__obliv_c__boolAnd(cond_res5, cond, tmp33);
			// src = oram->deepest[i];
			__obliv_c__intCondAssign(cond_res5, src, oram->deepest[i]);
			// dest = i;
			__obliv_c__int tmp34 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp34, i);
			__obliv_c__intCondAssign(cond_res5, dest, tmp34);
		// }
	}*/
	/*~obliv(){
	  for (int i = 0; i < oram->index_size; ++i) {
	  int tmp;
	  revealOblivInt(&tmp, target[i], 0);
	  printf("target, %d\n", tmp);
	  }
	  printf("===============\n");
	  }*/

	/*__obliv_c__int tmp_int1 = __obliv_c__newInt();
	__obliv_c__genOblivInt(tmp_int1, -1);
	dest = tmp_int1;
	// obliv int to_write_depth = 0;
	__obliv_c__int to_write_depth = __obliv_c__newInt();
	__obliv_c__int tmp35 = __obliv_c__newInt();
	__obliv_c__genOblivInt(tmp35, 0);
	__obliv_c__intCondAssign(cond, to_write_depth, tmp35);
	// obliv int hold_depth = 0;
	__obliv_c__int hold_depth = __obliv_c__newInt();
	__obliv_c__int tmp36 = __obliv_c__newInt();
	__obliv_c__genOblivInt(tmp36, 0);
	__obliv_c__intCondAssign(cond, hold_depth, tmp36);

	for(int i = 0; i <= oram->index_size; ++i) {
		// oram->to_write->is_dummy = true;
		__obliv_c__bool tmp37 = __obliv_c__newBool();
		__obliv_c__genOblivBool(tmp37, true);
		__obliv_c__boolCondAssign(cond, oram->to_write->is_dummy, tmp37);
		
		// obliv if (!oram->hold->is_dummy & i == dest) {
			__obliv_c__bool cond_res6 = __obliv_c__newBool();
			// i == dest
			__obliv_c__bool tmp38 = __obliv_c__newBool();
			__obliv_c__int tmp_int0 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp_int0, i);
			__obliv_c__intEqual(tmp38, tmp_int0, dest);
			// !oram->hold->is_dummy
			__obliv_c__bool tmp39 = __obliv_c__newBool();
			__obliv_c__copyBit(tmp39.bits, oram->hold->is_dummy.bits);
			__obliv_c__flipBit(tmp39.bits);
			// !oram->hold->is_dummy & i == dest
			__obliv_c__bool tmp40 = __obliv_c__newBool();
			__obliv_c__boolAnd(tmp40, tmp39, tmp38);
			// obliv if
			__obliv_c__boolAnd(cond_res6, cond, tmp40);
			block_copy(cond_res6, oram->hold, oram->to_write, oram->cpy);
			// oram->hold->is_dummy = true;
			__obliv_c__bool tmp41 = __obliv_c__newBool();
			__obliv_c__genOblivBool(tmp41, true);
			__obliv_c__boolCondAssign(cond_res6, oram->hold->is_dummy, tmp41);
			// to_write_depth = hold_depth;
			__obliv_c__intCondAssign(cond_res6, to_write_depth, hold_depth);
			// dest = -1;
			__obliv_c__int tmp42 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp42, -1);
			__obliv_c__intCondAssign(cond_res6, dest, tmp42);
		// }

		int cap = oram->bucket_size;
		if (i == 0) cap = oram->stash_size;
		// obliv if(oram->target[i] != -1) {
			__obliv_c__bool cond_res7 = __obliv_c__newBool();
			// oram->target[i] != -1
			__obliv_c__int tmp43 = __obliv_c__newInt();
			__obliv_c__genOblivInt(tmp43, -1);
			__obliv_c__bool tmp44 = __obliv_c__newBool();
			__obliv_c__intEqual(tmp44, oram->target[i], tmp43);
			__obliv_c__flipBit(tmp44.bits);
			// obliv if
			__obliv_c__boolAnd(cond_res7, cond, tmp44);
			for(int j = 0; j < cap; ++j) {
				// obliv if (oram->deepest_index[i] == j) {
					__obliv_c__bool cond_res8 = __obliv_c__newBool();
					// oram->deepest_index[i] == j
					__obliv_c__int tmp45 = __obliv_c__newInt();
					__obliv_c__genOblivInt(tmp45, j);
					__obliv_c__bool tmp46 = __obliv_c__newBool();
					__obliv_c__intEqual(tmp46, oram->deepest_index[i], tmp45);
					// obliv if
					__obliv_c__boolAnd(cond_res8, cond_res7, tmp46);
					block_copy(cond_res8, oram->path[i][j], oram->hold, oram->cpy);
					// oram->path[i][j]->is_dummy = true;
					__obliv_c__bool tmp47 = __obliv_c__newBool();
					__obliv_c__genOblivBool(tmp47, true);
					__obliv_c__boolCondAssign(cond_res8, oram->path[i][j]->is_dummy, tmp47);
					// dest = oram->target[i];
					__obliv_c__intCondAssign(cond_res8, dest, oram->target[i]);
					// hold_depth = oram->deepest_depth[i];
					__obliv_c__intCondAssign(cond_res8, hold_depth, oram->deepest_depth[i]);
				// }
			}
		// }

		// obliv bool added = false;
		__obliv_c__bool added = __obliv_c__newBool();
		__obliv_c__bool tmp48 = __obliv_c__newBool();
		__obliv_c__genOblivBool(tmp48, false);
		__obliv_c__boolCondAssign(cond, added, tmp48);
		// obliv if (!oram->to_write->is_dummy) {
			__obliv_c__bool cond_res9 = __obliv_c__newBool();
			// !oram->to_write->is_dummy
			__obliv_c__bool tmp49 = __obliv_c__newBool();
			__obliv_c__copyBit(tmp49.bits, oram->to_write->is_dummy.bits);
			__obliv_c__flipBit(tmp49.bits);
			// obliv if
			__obliv_c__boolAnd(cond_res9, cond, tmp49);
			for(int j = 0; j < cap; ++j) {
				// obliv if (oram->path[i][j]->is_dummy & !added) {
					__obliv_c__bool cond_res10 = __obliv_c__newBool();
					// !added
					__obliv_c__bool tmp50 = __obliv_c__newBool();
					__obliv_c__copyBit(tmp50.bits, added.bits);
					__obliv_c__flipBit(tmp50.bits);
					// oram->path[i][j]->is_dummy & !added
					__obliv_c__bool tmp51 = __obliv_c__newBool();
					__obliv_c__boolAnd(tmp51, oram->path[i][j]->is_dummy, tmp50);
					// obliv if
					__obliv_c__boolAnd(cond_res10, cond_res9, tmp51);
					block_copy(cond_res10, oram->to_write, oram->path[i][j], oram->cpy);
					// added = true;
					__obliv_c__bool tmp52 = __obliv_c__newBool();
					__obliv_c__genOblivBool(tmp52, true);
					__obliv_c__boolCondAssign(cond_res10, added, tmp52);
					// obliv if (to_write_depth > oram->deepest_depth[i]) {
						__obliv_c__bool cond_res11 = __obliv_c__newBool();
						// to_write_depth > oram->deepest_depth[i]
						__obliv_c__bool tmp53 = __obliv_c__newBool();
						__obliv_c__intGr(tmp53, to_write_depth, oram->deepest_depth[i]);
						// obliv if
						__obliv_c__boolAnd(cond_res11, cond_res10, tmp53);
						// oram->deepest_depth[i] = to_write_depth;
						__obliv_c__intCondAssign(cond_res11, oram->deepest_depth[i], to_write_depth);
						// oram->deepest_index[i] = j;
						__obliv_c__int tmp54 = __obliv_c__newInt();
						__obliv_c__genOblivInt(tmp54, j);
						__obliv_c__intCondAssign(cond_res11, oram->deepest_index[i], tmp54);
					// }
				// }
			}
		// }
	}*/
}
