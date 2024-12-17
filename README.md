# My Malloc and Free
## Introduction of the project
- This project focuses on system programming and involves implementing the malloc() and free() functions in C. Through this project, we aim to gain insights into how malloc dynamically allocates memory chunks in the heap and how free releases these allocations. By recreating these fundamental functions, we can better understand the inner workings of memory management in C.

## Design Details
1. **Some key ideas**
     - In this project, we assume we are using default 64-bits architecture. The heap we allocated uses 16 bytes alignment.
     
     - How the heap is organized:
     ```C
     /**
     * Heap diagram:
     *
     *     |<<<---------------- FREE MEMORY SIZE --------------->>>|
     * +---+---+-----------------------------------------------+---+---+
     * | A | B | ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ | C | D |
     * +---+---+-----------------------------------------------+---+---+
     *     |   |                                               |   |
     *     |   ^                                               |   |
     *     |   ptr: the start of allocatable memory            |   |
     *     |                                                   ^   |
     *     ^                       the footer for the first block  |
     *     the first block header                                  ^
     *
     *
     *                                                 the end mark
     */
     ```
     - We use 4 meta data block to help us allocate and release the data chunks (each takes up 8 bytes)
        1. **Block Header**: 
           - This metadata block tells us the size of following data chunks and whether previous and following data chunks are currently allocated.
        2. **Block Footer**:
           - Block footer contains the same information as block header, but this block is for our convenience. When we try to allocate some data or free some blocks, we do not have to traverse back to block header every time. 
        3. **End Marker**:
           - This block tells us we are at the end of the heap we allocated. The size information is zero but it is in busy state(value 1).
  
     - How do we present whether data chunks are busy
        1. We set least significant bit to 1 to say the following data chunks are busy and vice versa.
        2. We set second least significant bit to 1 to say the previous data chunks are busy and vice versa.
    
    - What does 16 bytes alignment mean:
        1. The start of the actual content (8 bytes after the block header) must be the multiple of 16, and the size of a single data chunk (block header + assigned data blocks) is the multiple of 16.
    
2. Malloc() and Free()
   - **vmalloc()**:
     1. The allocation strategy we use in here is "Best-Fit" strategy, which means the function traverses through the heap from the start and find the most suitable data blocks.
   
     2. The size passed to the parameter is the actual size of data we stored, but we also have to count block header into the amount we are going to allocate. The reason we do not count block footer is that we do not actually assign values when we call malloc, so we can just use the last 8 bytes as the block footer, and since we will update the next block header once we call vmalloc(), there is no side effects for overwritting the end footer. Also, we always need to follow the rule of 16 bytes alignment. 
     
     3. Once vmalloc() finds the approriate chunk, there are two cases we have to consider after allocation.
        - Remaining blocks have more than 16 bytes (include 16 bytes): It is possible that the entire original chunk spilt into two independent chunks. So, in this case, we need to update the block header and footer of the orginal chunk and also create block header and footer for the new chunk.  
        - Remaining is zero: There is no way to have the chunk which size is between 0 bytes and 16 bytes if we follow the rule of alignment. 
     
   - **Free()**:
     1. There are two important cases we need to handle in impementing free().
        - No coalescing blocks: This means previous one chunk or following chunk is busy. So, the only thing we need to do here is to update the information in both block header and block footer. 
        - Have coalescing blocks: There are three scenarios that might occur. 
            - **Only previous one chunk is not busy**: We update the information in the block header of the previous chunk and the current block footer. The size becomes the sum of the two chunks, and we set state as not busy.
            - **Only following one chunk is not busy**: We update the information in the block header of the current chunk and the block footer of following chunk. The size becomes the sum of the two chunks, and we set state as not busy.
            - **Both previous one and following one chunk are not busy**: We update the information in the block header of the previous chunk and the block footer of the following one chunk. The size becomes the sum of the three chunks, and we set state as not busy.
  






