#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "vmlib.h"
#include "vm.h"

int main()
{
    vmload("img/no_free.img");

    vminfo(); /* uncomment this to see the contents of the image. */

    void *ptr = heapstart + 1;
    vmfree(ptr);
    
    vminfo();
    printf("size status is %d\n", heapstart->size_status);
    assert(heapstart->size_status == 82);
    
    uint64_t size1 = 82 & ~(1 << 1);
    size1 &= ~1;
    printf("size is %d\n", size1);
   
    printf("%d\n", *((char*)ptr+size1-8));
    ptr = vmalloc(64);
    assert(ptr != NULL);

    vmdestroy();
    return 0;
}
