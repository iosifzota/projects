#ifndef __memory_h
#define __memory_h

#include <malloc.h>

#include "base.h"
#include "req.h"

header_fn
void_ptr emalloc(size_t bytes)
{ return checkpoint_void(malloc(bytes)); }

header_fn
void_ptr erealloc(void_ptr ptr, size_t bytes)
{ return checkpoint_void(realloc(ptr, bytes)); }

header_fn
void_ptr reserve(size_t this_manny, size_t of_size)
{ return emalloc(this_manny * of_size); }


#endif // !__memory_h
