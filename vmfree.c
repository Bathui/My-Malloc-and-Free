#include "vm.h"
#include "vmlib.h"

/**
 * The vmfree() function frees the memory space pointed to by ptr,
 * which must have been returned by a previous call to vmmalloc().
 * Otherwise, or if free(ptr) has already been called before,
 * undefined behavior occurs.
 * If ptr is NULL, no operation is performed.
 */
void vmfree(void *ptr)
{
	// TODO
	if (ptr == NULL)
		return;
	struct block_header* cur_header = (struct block_header*)((char*)ptr - 8);
	cur_header -> size_status &= ~1;// free a block 	
	
	uint64_t tmp = (cur_header->size_status) & ~(1 <<1);
	tmp &= ~1;
	struct block_footer* update = (struct block_footer*)((char*)cur_header + tmp - 8);
	update->size = cur_header->size_status; 
	//focus on the right side
	if (*((char*)update + 8) % 2 == 0){
		update = (struct block_footer*)((char*)update + 8);
		tmp = (update->size) & ~(1<<1);
		tmp &= ~1;
		cur_header->size_status += tmp;
		
		tmp = cur_header->size_status & ~(1<<1);
		tmp &= ~1;

		update = (struct block_footer*)((char*)cur_header + tmp - 8);
		update -> size = cur_header ->size_status;
	}

	if (cur_header == heapstart)
		return;
	
	// start coalesce
	struct block_footer* footer = (struct block_footer* )((char*)cur_header-8);
	
	//trace back to the header
	
	if (footer->size % 2 == 0){// previous block is not busy
		uint64_t block_size = (footer->size) & (~1);
		block_size &= ~(1<<1);
		struct block_header* cur_header2 = (struct block_header*)((char*)cur_header - block_size);

		uint64_t block_size2 = (cur_header2->size_status) & (~1);
		block_size2 &= ~(1<<1);
		block_size = (cur_header->size_status ) & (~1);
		block_size &= ~(1<<1);

		block_size2 += tmp;

		cur_header2->size_status = block_size2 | (1<<1);
		
		
		struct block_footer* footer = (struct block_footer*)((char*)cur_header2 + block_size2 - 8);
	        footer->size = cur_header2->size_status;	

			
	}
	
	
}
