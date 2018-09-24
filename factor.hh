#ifndef __factor_hh
#define __factor_hh

#include "math.h"

namespace iz {
    size_t numextract(size_t src, size_t count, size_t* remainder) {
        size_t dest = 0;
        for(size_t rank = 0; src && (rank < count); ++rank) {
            dest += (src % 10) * pow(10, rank);
            src /= 10;
        }
        if(remainder) {
            *remainder = src;
        }
        return dest;
    }
}

#endif