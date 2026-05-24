#include "ExpandableArray.h"
#include "Constants.h"
#include "main.h"

// each Expandable array maintains a separate array of open indexes
// writes and reads are always done at the end
// when inserting into array check if length of indexes array is 0
// if so then expand array then insert
// regardless pop last item of index array and insert at that location
// when removing from array append the index to the indexes array

// Pros: faster than iterating through entire array checking for opening
// Cons: still slow requiring at least 1 cold cache hit per insert/removal, also sizeof(int) * N extra space required

// Note on last point. Can't use N/growth_factor array because even if array has less than half elements some may be in the larger section.
// Data cannot be moved by this manager so it must deal with it.

// overall not worth. neat idea if ram is less important than CPU
// TODO move to struct? with all flags because passing all this sucks
void* ArrayInit(const size_a init_element_count, const unsigned int segment_count, const unsigned int element_size, const unsigned int alignment) {
    void *location = _aligned_malloc(init_element_count * segment_count * element_size, alignment);
    if (location == NULL) {
        Log("Shitfuck god damnit", 19);
        SUICIDE_FLAG = 1;
        ExitThread(1);
    }
    return location;
}

void ArrayResize(void **location, size_a *element_count, const unsigned char growth_factor, const unsigned int segment_count, const size_t element_size, const unsigned int alignment) {
    const unsigned int new_element_count = *element_count * growth_factor;
    char *new_location = _aligned_malloc(new_element_count * segment_count * element_size, alignment);
    if (new_location == NULL) { // if malloc fails exit
        Log("Shitfuck god damnit", 19);
        SUICIDE_FLAG = 1;
        ExitThread(1);
    }
    char *new_loc_copy = new_location; // copies to mutate while transferring data
    const char *old_loc_copy = *location;
    const unsigned int copy_size = element_size * *element_count;
    const unsigned int offset_size = element_size * new_element_count;
    for (unsigned int i=0; i<segment_count; i++) {
        MemCpy(new_loc_copy, old_loc_copy, copy_size);
        new_loc_copy += offset_size;
        old_loc_copy += copy_size;
    }

    *element_count = new_element_count;
    _aligned_free(*location);
    *location = new_location;
}

void ArrayDestroy(void *location) {
    _aligned_free(location);
}