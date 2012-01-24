/*
 *  mab.c
 *  
 *
 *  Created by Joe on 11/2/11.
 *  Copyright 2011 Boston College. All rights reserved.
 *
 */

#include "mab.h"

MabPtr createnullMab(int size){
	MabPtr mem = (MabPtr)malloc(sizeof(Mab));
	mem->size = size;
	mem->offset = 0;
	mem->allocated = UNALLOCATED;
	mem->next = NULL;
	mem->prev = NULL;
	return mem;
}

MabPtr memChk(MabPtr m, int size, int alg){
	// added parameter to allow multiple different kinds of algorithms based on input
	
	
	
	// check if memory available 
	// m represents head of table
	
	// when checking memory, can combine free areas into contiguous structs
	// replacement algorithms go here
	
	// returns a block where the process should fit - BUT only called by memAlloc below
	// memAlloc splits the memory, changes the "allocated" part of the Mab, and returns to the process
	
	// cycle through the linked list to find the appropriate block of memory: best fit, first fit, next fit, worst fit
	
	if(alg == FIRSTFIT){
		//start with first fit:
		MabPtr ptr = m;
		while(ptr && ptr->next){
			if(ptr->allocated == UNALLOCATED){
				MabPtr ptr2 = ptr;
				while(ptr2->next && (ptr2->next)->allocated == UNALLOCATED){
					ptr2 = memMerge(ptr2);
				}
				if (ptr2->size > size) {
					//returns first section that is unallocated and large enough
					//first fit
					return ptr2;
				}
			}
		}
		if(ptr->allocated == UNALLOCATED && ptr->size > size){
			// the very last block is the ONLY fit
			return ptr;
		}
		return NULL;
	}
	if(alg == BESTFIT){
		MabPtr best = NULL;
		MabPtr ptr = m;
		while(ptr && ptr->next){
			if(ptr->allocated == UNALLOCATED){
				MabPtr ptr2 = ptr;
				while(ptr2->next && (ptr2->next)->allocated == UNALLOCATED){
					ptr2 = memMerge(ptr2);
				}
				
				//best fit: if the new block of accomodating size is SMALLER than the previously best block, replace it as the ideal
				if (ptr2->size > size) {
					if(best && best->size > ptr2->size)
						best = ptr2;
				}
			}
		}
		if(ptr->allocated == UNALLOCATED && ptr->size > size){
			// the very last block is the ONLY fit
			return ptr;
		}
		if(best) return best;
		
		return NULL;
	}
	if(alg == WORSTFIT){
		MabPtr worst = NULL;
		MabPtr ptr = m;
		while(ptr && ptr->next){
			if(ptr->allocated == UNALLOCATED){
				MabPtr ptr2 = ptr;
				while(ptr2->next && (ptr2->next)->allocated == UNALLOCATED){
					ptr2 = memMerge(ptr2);
				}
				
				//worst fit: if the new block of accomodating size is LARGER than the previously best block, replace it as the ideal
				if (ptr2->size < size) {
					if(worst && worst->size > ptr2->size)
						worst = ptr2;
				}
			}
		}
		if(ptr->allocated == UNALLOCATED && ptr->size > size){
			// the very last block is the ONLY fit
			return ptr;
		}
		if(worst) return worst;
		
		return NULL;
	}
	
	if(alg == NEXTFIT){
		MabPtr ptr = m;
		//find most recently used 
		while(ptr->next && ptr->justused != 1){
			ptr = ptr->next;
		}
		ptr->justused = 0;
		ptr = ptr->next;
		
		while(ptr && ptr->next){
			if(ptr->allocated == UNALLOCATED){
				MabPtr ptr2 = ptr;
				while(ptr2->next && (ptr2->next)->allocated == UNALLOCATED){
					ptr2 = memMerge(ptr2);
				}
				if (ptr2->size > size) {
					//returns NEXT section that is unallocated and large enough
					ptr2->justused = 0;
					return ptr2;
				}
			}
		}
		//if you need to loop around:
		if(ptr->allocated == ALLOCATED)
			ptr = m;
		//repeat procedure on beginning of the queue
		while(ptr && ptr->next){
			if(ptr->allocated == UNALLOCATED){
				MabPtr ptr2 = ptr;
				while(ptr2->next && (ptr2->next)->allocated == UNALLOCATED){
					ptr2 = memMerge(ptr2);
				}
				if (ptr2->size > size) {
					//returns NEXT section that is unallocated and large enough
					ptr2->justused = 0;
					return ptr2;
				}
			}
		}
		
		if(ptr->allocated == UNALLOCATED && ptr->size > size){
			// the very last block is the ONLY fit
			return ptr;
		}
		return NULL;
	}
	
	return NULL;
}
MabPtr memAlloc(MabPtr m, int size){
	// allocate memory block 
	
	// m is the head of a memory list
	// returns a pointer to the block of memory that the process can have
	
	MabPtr res = memChk(m, size);
	
	if(!res){
		perror("expected, but couldn't find a good block of memory");
		return NULL;
	}
	if(res->size > size){
		// split into two blocks
		res = memSplit(res, size);
	}
	res->allocated = ALLOCATED;
	return res;
}
MabPtr memFree(MabPtr m){
	// free memory block
	// m represents particular block of memory
	
	m->allocated = UNALLOCATED;

	return m;
	
}
MabPtr memMerge(MabPtr m){
	// merge two memory blocks 
	// m represents first of the two blocks
	//deal with size and offset
	if(!(m->allocated) && m->next && !((m->next)->allocated)){
		m->size = (m->next)->size;
		((m->next)->next)->prev = m;
		m->next = (m->next)->next;
		free(m->next);
		return m;
	}
	return NULL;
}
MabPtr memSplit(MabPtr m, int size){
	// split a memory block
	//deal with size and offset
	if(m && m->allocated == UNALLOCATED){
		MabPtr next = createnullMab((m->size)-size);
		m->size = size;
		next->next = m->next;
		next->prev = m;
		if(m->next) (m->next)->prev = next;
		m->next = next;
		//return the first of the two new 
		return m;
	}else {
		// fault: either m was null or process tried to split an allocated block
		return NULL;
	}
}
