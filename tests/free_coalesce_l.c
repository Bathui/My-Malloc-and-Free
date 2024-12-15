#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "vmlib.h"
#include "vm.h"

int main()
{
    vminit(4096);

    void *ptr1 = vmalloc(64);
    void *ptr2 = vmalloc(64);
    //if (ptr1==ptr2)
//	    puts("wrong here");
    //vmalloc(64); // ptr3
    
    uint64_t size = *((char*)ptr1-8) & (~1);
    size &= ~(1<<1); 
    printf("ptr1 size is %d\n", size ); 
    printf("footer is %d\n", *((char*)ptr1+size-8) );
    //printf("in the header %d\n", (char*)ptr1+);
    //printf("Before header is %d\n", *((char*)ptr1-8));
    /* create a free block at ptr1*/
    vmfree(ptr1);
    /* this free should coalesce with the free block from ptr1*/
    vmfree(ptr2);
    
    /* this allocation should utilize the coalesced free block */
    void *ptr4 = vmalloc(144);
    assert(ptr1 == ptr4);
    //printf("in the header %d\n", *((char*)ptr1-8));

    vmdestroy();
    return 0;
}
