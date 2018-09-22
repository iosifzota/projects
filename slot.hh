#ifndef __slot_hh
#define __slot_hh

#include <vector>
#include <cmath>
#include "req.hh"

#ifdef self
#undef self
#endif
#define self slot_map<T>

#ifdef T8
#undef T8
#endif
#define T8 template <typename T>

using std::vector;
using std::pow;

namespace iz {
    size_t numextract(size_t src, size_t count, size_t* remainder);
    struct slot_info
    {
        size_t index, timestamp;
    };

    T8
    class slot_map
    {
    public:
        inline T& operator[](size_t);
        inline const T& operator[](size_t) const;

        inline bool valid_slot(slot_info) const;
        static inline slot_info extract_info(size_t);
    private:
        vector<T> store;
        vector<slot_info> registry;
    };

    T8
    T&
    self::operator[](size_t id) {
        slot_info info = extract_info(id);
        req(valid_slot(info));
        return store[registry[info.index].index];
    }

    T8
    bool
    self::valid_slot(slot_info info) const {
        if (registry[info.index].timestamp == 0) {
            return true;
        }
        return registry[info.index].timestamp == info.timestamp;
    }

    T8
    slot_info
    self::extract_info(size_t id) {
        slot_info info;
        info.index = numextract(id/10, id%10, &info.timestamp);
        return info;
    }

    /* move out */
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