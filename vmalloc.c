#include "vm.h"
#include "vmlib.h"

 
void *vmalloc(size_t size)
{
	if(size == 0)
		return NULL;	
	// TODO
	uint64_t* cur_pos = (uint64_t*)heapstart;

	uint64_t* min_pos = cur_pos;
	
	uint64_t min_size = 2147483647;
       	
	if (size % 16 ==0)
		size += 16;
	if (size % 16 > 8)
		size += 8;
	size = (size + 15) / 16 * 16;
	
	while(((struct block_header*)cur_pos)->size_status != 1){
		uint64_t cur_size;

		cur_size = (((struct block_header*)cur_pos) -> size_status);
		cur_size &= ~(1<<1);
	       	cur_size &= ~1;	
		
		if (*cur_pos % 2 == 0 && cur_size <= min_size && size <= cur_size){
			min_size = cur_size;
			min_pos = cur_pos;
		}

		cur_pos = (uint64_t*)((char*) cur_pos + cur_size);
	}
	uint64_t* footer = (uint64_t*)((char*)min_pos + min_size - 8);
	
	if (min_pos != (uint64_t*)heapstart||min_size != 2147483647){ 
		
		uint64_t cur_size = *min_pos & (~1); 
		cur_size &= ~(1<<1);

		uint64_t remaining = cur_size - size; // include the header
			
		
		//update next block's header and the footer
		if (remaining > 16){ 
			footer = (uint64_t*)((char*)min_pos + size -8);
			*min_pos = size+1;
			*min_pos |= 1 << 1;
			uint64_t* nextHeader = (uint64_t*)((char*)min_pos + size);
			remaining |= 1 << 1;
			
			*nextHeader = remaining ; 
			*footer = *min_pos;
		}
		else{
			*min_pos = cur_size + 1;
			*min_pos |= 1 << 1;

			*footer = *min_pos;
		}
		return (char*)min_pos+8;
		
		
	}



	return NULL;
}
