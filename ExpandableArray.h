#ifndef CPUPHYSICSENGINE_EXPANDABLEARRAY_H
#define CPUPHYSICSENGINE_EXPANDABLEARRAY_H

#include "Constants.h"

void *ArrayInit(unsigned int init_element_count, unsigned int segment_count, unsigned int element_size, unsigned int alignment);

void ArrayResize(void **location, size_a *element_count, unsigned char growth_factor, unsigned int segment_count, size_t element_size, unsigned int alignment);

void ArrayDestroy(void *location);

#endif //CPUPHYSICSENGINE_EXPANDABLEARRAY_H