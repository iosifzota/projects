#ifndef __interval_h
#define __interval_h

#include <stdlib.h>
#include <stdbool.h>

#include "base.h"

struct(Interval) {
    int edge1, edge2;
};

header_fn
size_t int_length(int edge1, int edge2)
    RET( (size_t)abs(edge1 - edge2) );

header_fn
bool int_big_enough(size_t this_much, Interval inter)
    RET( this_much <= int_length(inter.edge1, inter.edge2) );

header_fn
bool int_too_small(size_t this_much, Interval inter)
    RET( !int_big_enough(this_much, inter) );

#endif // !__interval_h
