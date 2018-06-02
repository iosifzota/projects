#include <string>
#include <ostream>
#include <cstdint>

#include "hashes.hh"

unsigned FNV_Hash::operator () (const std::string& str) const
{
    unsigned h;

    h = 2166136261; // 32-bit offset

    for (const auto& chr : str) {
        h = (16777619 * h) ^ chr;
    }

    return h;
}

unsigned Djb_Hash::operator () (const std::string& str) const
{
    unsigned h = 0;

    for (const auto& chr : str) {
        h = 33 * h + chr;
    }

    return h;
}

unsigned Rot_Hash::operator () (const std::string& str) const
{
    unsigned h = 0;

    for (const auto& chr : str) {
        h = (h << 5) ^ (h >> 27) ^ chr;
    }

    return h;
}

unsigned Xor_Hash::operator () (const std::string& str) const
{
    unsigned h = 0;

    for (const auto& chr : str) {
        h ^= chr;
    }

    return h;
}

unsigned Add_Hash::operator () (const std::string& str) const
{
    unsigned h = 0;

    for (const auto& chr : str) {
        h += chr;
    }

    return h;
}

uint32_t Jenkins_Hash32::operator () (uint32_t n) const
{
    n = (n+0x7ed55d16) + (n<<12);
    n = (n^0xc761c23c) ^ (n>>19);
    n = (n+0x165667b1) + (n<<5);
    n = (n+0xd3a2646c) ^ (n<<9);
    n = (n+0xfd7046c5) + (n<<3);
    n = (n^0xb55a4f09) ^ (n>>16);

    return n;
}

/* Helpers */
bool Hash_Stats::operator < (const Hash_Stats& other) const
{
    return collisions_count < other.collisions_count;
}

std::ostream& operator << (std::ostream& out, const Hash_Stats& hs) {
    return out << hs.hash_name << ": " << hs.collisions_count;
}
