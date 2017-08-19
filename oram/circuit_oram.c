#include "circuit_oram.h"


void get_random(__obliv_c__bool cond, CircuitOram * oram, __obliv_c__bool* res, int length) // obliv
{
	if (length + oram->rand_pool_size >= oram->rand_pool_cap) {
		get_rand_obliv(cond, oram->gen, oram->rand_pool, oram->rand_pool_cap);
		oram->rand_pool_size = 0;
	}
	for(int i = 0; i < length; ++i)
		// res[i] = oram->rand_pool[oram->rand_pool_size+i];
		__obliv_c__boolCondAssign(cond, res[i], oram->rand_pool[oram->rand_pool_size + i]);
	oram->rand_pool_size += length;
}

CircuitOram* ckt_initialize_more_param(int N, OcCopy* cpy, int recursion_factor, int cutoff)
{
	CircuitOram* res = calloc(sizeof(CircuitOram), 1);
	res->super.n = N;
	res->super.cpy = cpy;
	res->super.release = ckt_release;
	res->super.access = ckt_apply;
	res->cutoff = cutoff;
	res->recursion_factor = recursion_factor;
	res->orams = calloc(sizeof(NonRecursiveOram*), 10);
    res->poscpy = _mm_malloc(sizeof(OcCopy) * 10, 32);

	res->orams[0] = nro_initialize(N, cpy);

	int new_data_size = res->orams[0]->position_label_size * recursion_factor;
	int newN = (1 << res->orams[0]->index_size) / recursion_factor;
	int cur = 0;

	while(newN * recursion_factor > cutoff)
	{
		cur++;
        res->poscpy[cur] = ocCopyBoolN(new_data_size);
		res->orams[cur] = nro_initialize(newN, res->poscpy+cur);
		new_data_size = res->orams[cur]->position_label_size * recursion_factor;
		newN = (1 << res->orams[cur]->index_size) / recursion_factor;
	}
	res->num_recursion = cur + 1;

	res->poscpy[cur+1] = ocCopyBoolN(res->orams[cur]->position_label_size);
	res->base = ocLinearOramNew(res->poscpy + cur + 1, NULL, 1 << res->orams[cur]->index_size);

	// res->gen = newBCipherRandomGen();

	res->rand_pool_cap = 200000;
	res->rand_pool_size = 0;
	res->rand_pool = calloc(sizeof(__obliv_c__bool), res->rand_pool_cap);
	__obliv_c__bool cond = __obliv_c__newBool();
	__obliv_c__genOblivBool(cond, true);
	get_rand_obliv(cond, res->gen, res->rand_pool, res->rand_pool_cap);
	return res;
}

CircuitOram* ckt_initialize(int N, OcCopy* cpy) 
{
	return ckt_initialize_more_param(N, cpy, 8, 1<<8);
}

void ckt_release(OcOram* super) {
	CircuitOram* oram = (CircuitOram*) super; // CAST(super);
	for(int i = 0; i < oram->num_recursion; ++i) {	
		nro_release(oram->orams[i]);
	}
	ocOramRelease(oram->base);
	// _mm_free(oram->poscpy);
	// _mm_free(oram->orams);
	// releaseBCipherRandomGen(oram->gen);
	// _mm_free(oram->rand_pool);
	// _mm_free(oram);
}

void ckt_read(__obliv_c__bool cond, CircuitOram* oram, __obliv_c__int index, void* data) // obliv 
{
	__obliv_c__int pos = __obliv_c__newInt();
	__obliv_c__genOblivInt(pos, 0);
	__obliv_c__int new_pos = __obliv_c__newInt();
	__obliv_c__genOblivInt(new_pos, 0);

	update_trees(cond, oram, index, 1, &pos, &new_pos);

	/*	~obliv(){int p1; revealOblivInt(&p1,index, 0);
		int p2; revealOblivInt(&p2,pos, 0);
		int p3; revealOblivInt(&p3,new_pos, 0);
		printf("idx,pos,n_pos, %d,%d,%d\n", p1,p2,p3);}
	 */
	nro_read(cond, oram->orams[0], index, pos, new_pos, data);   
}

void ckt_apply(__obliv_c__bool cond, OcOram* super, __obliv_c__int index, OcOramAccessCb func, void* ext_data) // obliv 
{
	CircuitOram* oram = (CircuitOram*) super; // CAST(super);
	__obliv_c__int pos = __obliv_c__newInt();
	__obliv_c__genOblivInt(pos, 0);
	__obliv_c__int new_pos = __obliv_c__newInt();
	__obliv_c__genOblivInt(new_pos, 0);

	update_trees(cond, oram, index, 1, &pos, &new_pos);

    /*void *data = calloc_obliv(cond, 1, super->cpy->eltsize);
	nro_read_and_remove(cond, oram->orams[0], index, pos, data);
	func(cond, super, data, ext_data);
	nro_put_back(cond, oram->orams[0], index, new_pos, data);
	free_obliv(cond, data);*/
}

void ckt_write(__obliv_c__bool cond, CircuitOram* oram, __obliv_c__int index, void* data) // obliv
{
	__obliv_c__int pos = __obliv_c__newInt();
	__obliv_c__genOblivInt(pos, 0);
	__obliv_c__int new_pos = __obliv_c__newInt();
	__obliv_c__genOblivInt(new_pos, 0);

	update_trees(cond, oram, index, 1, &pos, &new_pos);

	/*	~obliv(){int p1; revealOblivInt(&p1,index, 0);
		int p2; revealOblivInt(&p2,pos, 0);
		int p3; revealOblivInt(&p3,new_pos, 0);
		printf("idx,pos,n_pos, %d,%d,%d\n", p1,p2,p3);}
	 */
	nro_write(cond, oram->orams[0], index, pos, new_pos, data);
}

void update_trees(__obliv_c__bool cond, CircuitOram* oram, __obliv_c__int index, int level, __obliv_c__int* pos, __obliv_c__int* new_pos) // obliv
{
	int entry_size = oram->orams[level - 1]->position_label_size;
	int block_size = oram->recursion_factor*entry_size;
	if (level == oram->num_recursion) {
		__obliv_c__bool* bools = calloc_obliv(cond, sizeof(__obliv_c__bool), entry_size);
		ocOramRead(cond, bools, oram->base, index);
		__obliv_c__bool* new_pos_bools = calloc_obliv(cond, sizeof(__obliv_c__bool), entry_size);
		get_random(cond, oram, new_pos_bools, entry_size);
		ocOramWrite(cond, oram->base, index, new_pos_bools);

		bool_to_int_obliv(cond, bools, pos, entry_size);
		bool_to_int_obliv(cond, new_pos_bools, new_pos, entry_size);
		free_obliv(cond, bools);
		free_obliv(cond, new_pos_bools);
	} else {
		NonRecursiveOram* current_oram = oram->orams[level];
		__obliv_c__bool* pos_map = calloc_obliv(cond, sizeof(__obliv_c__bool), block_size);
		__obliv_c__bool* bools = calloc_obliv(cond, sizeof(__obliv_c__bool), entry_size);
		__obliv_c__bool* new_pos_bools = calloc_obliv(cond, sizeof(__obliv_c__bool), entry_size);

		__obliv_c__int new_index = __obliv_c__newInt();
		__obliv_c__int tmp_int1 = __obliv_c__newInt();
		__obliv_c__intRShift(tmp_int1, index, oram->orams[level - 1]->index_size - current_oram->index_size);
		__obliv_c__intCondAssign(cond, new_index, tmp_int1);
		__obliv_c__int f_pos = __obliv_c__newInt();
		__obliv_c__genOblivInt(f_pos, 0);
		__obliv_c__int f_new_pos = __obliv_c__newInt();
		__obliv_c__genOblivInt(f_new_pos, 0);

		update_trees(cond, oram, new_index, level + 1, &f_pos, &f_new_pos);

		nro_read_and_remove(cond, current_oram, new_index, f_pos, pos_map);

		__obliv_c__int ith_pos = __obliv_c__newInt();
		__obliv_c__int tmp_int2 = __obliv_c__newInt();
		__obliv_c__genOblivInt(tmp_int2, (1 << (oram->orams[level - 1]->index_size-current_oram->index_size)) - 1);
		__obliv_c__int tmp_int3 = __obliv_c__newInt();
		__obliv_c__intBitwiseAnd(tmp_int3, tmp_int2, index);
		__obliv_c__intCondAssign(cond, ith_pos, tmp_int3);

		extract_bits(cond, pos_map, ith_pos, entry_size, bools, oram);
		get_random(cond, oram, new_pos_bools, entry_size);
		put_bits(cond, pos_map, ith_pos, entry_size, new_pos_bools, oram);

		nro_put_back(cond, current_oram, new_index, f_new_pos, pos_map);
		bool_to_int_obliv(cond, bools, pos, entry_size);
		bool_to_int_obliv(cond, new_pos_bools, new_pos, entry_size);
		free_obliv(cond, bools);
		free_obliv(cond, new_pos_bools);
		free_obliv(cond, pos_map);
	}
}

void extract_bits(__obliv_c__bool cond, __obliv_c__bool* data_chunk, __obliv_c__int ith_pos, 
				  int entry_size, __obliv_c__bool* res, CircuitOram* oram) //obliv
{
	for(int i = 0; i < oram->recursion_factor; ++i) {
		// obliv if(ith_pos == i) {
		__obliv_c__bool cond_res = __obliv_c__newBool();
		__obliv_c__bool tmp0 = __obliv_c__newBool();
		__obliv_c__int tmp1 = __obliv_c__newInt();
		__obliv_c__genOblivInt(tmp1, i);
		__obliv_c__intEqual(tmp0, ith_pos, tmp1);
		__obliv_c__boolAnd(cond_res, tmp0, cond);
			for(int j = 0; j < entry_size; ++j) {
				// res[j] = data_chunk[i*entry_size + j];
				__obliv_c__bool tmp2 = __obliv_c__newBool();
				// TODO: Fix this
				// *tmp2.bits = data_chunk->bits[i*entry_size + j];
				__obliv_c__boolCondAssign(cond_res, res[j], tmp2);
			}
		// }
	}
}

void put_bits(__obliv_c__bool cond, __obliv_c__bool* data_chunk, __obliv_c__int ith_pos, int entry_size, __obliv_c__bool* res,
			  CircuitOram * oram) // obliv 
{
	for(int i = 0; i < oram->recursion_factor; ++i) {
		// obliv if(ith_pos == i) {
		__obliv_c__bool cond_res = __obliv_c__newBool();
		__obliv_c__bool tmp0 = __obliv_c__newBool();
		__obliv_c__int tmp1 = __obliv_c__newInt();
		__obliv_c__genOblivInt(tmp1, i);
		__obliv_c__intEqual(tmp0, ith_pos, tmp1);
		__obliv_c__boolAnd(cond_res, tmp0, cond);
		for(int j = 0; j < entry_size; ++j) {
			// data_chunk[i*entry_size + j] = res[j];
			__obliv_c__boolCondAssign(cond_res, data_chunk[i*entry_size + j], res[j]);
		}
		// }
	}
}

void get_rand_obliv(__obliv_c__bool cond, /*BCipherRandomGen*/void* gen, __obliv_c__bool * data, int len) // obliv
{
   // ~obliv(){
      bool* rand_bool = _mm_malloc(len * sizeof(bool), 32);
      // OblivInputs* specs = _mm_malloc(sizeof(OblivInputs) * len);
      // OblivInputs* specs2 = _mm_malloc(sizeof(OblivInputs) * len);
      __obliv_c__bool* data1 = _mm_malloc(sizeof(__obliv_c__bool) * len, 32);
	  
	  //***************************************************************
	  // TODO: Fix this code
	  for(int i = 0; i < len; ++i) {
		  __obliv_c__bool tmp0 = __obliv_c__newBool();
		  __obliv_c__genOblivBool(tmp0, true);
		  data[i] = tmp0;
	  }
	  for(int i = 0; i < len; ++i) {
		  __obliv_c__bool tmp0 = __obliv_c__newBool();
		  __obliv_c__genOblivBool(tmp0, false);
		  data1[i] = tmp0;
	  }
	  //***************************************************************
	  
      // randomizeBuffer(gen, rand_bool, len);
      /*for(int i = 0; i < len; ++i) {
         rand_bool[i] =( (rand_bool[i]) % 2 == 0);
         setupOblivBool(&specs[i], &data[i], rand_bool[i]);
         setupOblivBool(&specs2[i], &data1[i], rand_bool[i]);
      }
      feedOblivInputs(specs, len, 1);
      feedOblivInputs(specs2, len, 2);*/
      for(int i = 0; i < len; ++i)
         __obliv_c__boolXor(data[i], data[i], data1[i]);
      // _mm_free(rand_bool);
      /*// _mm_free(specs);
      // _mm_free(specs2);
      // _mm_free(data1);*/
   // }
}
