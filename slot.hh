#ifndef __slot_hh
#define __slot_hh

#include <vector>
#include <cmath>
#include "req.hh"
#include "factor.hh"

#ifdef self
#undef self
#endif
#define self slot_map<T>

using std::vector;
using std::pow;

namespace iz {
    struct slot_info
    {
        size_t index, timestamp;
    };

    template <typename T>
    class slot_map
    {
    public:
        slot_map();

        inline T& operator[](size_t);
        // inline const T& operator[](size_t) const;
        inline size_t deposit(const T&);

        inline bool registry_checkpoint(slot_info) const;
        static inline slot_info extract_info(size_t);
    private:
        size_t registry_open_index;
        vector<T> store;
        vector<slot_info> registry;
    };

    template <typename T>
    self::slot_map()
        : registry_checkpoint{} {
        // here
    }


    template <typename T>
    size_t
    self::deposit(const T& data) {
        // then here
    }


    template <typename T>
    T&
    self::operator[](size_t id) {
        slot_info info = extract_info(id);
        req(registry_checkpoint(info));
        return store[registry[info.index].index];
    }

    template <typename T>
    bool
    self::registry_checkpoint(slot_info info) const {
        if (info.index >= registry.size()) {
            return false;
        }
        return registry[info.index].timestamp == 0 ||
               registry[info.index].timestamp == info.timestamp;
    }

    template <typename T>
    slot_info
    self::extract_info(size_t id) {
        slot_info info;
        info.index = numextract(id/10, id%10, &info.timestamp);
        return info;
    }
}

#endif