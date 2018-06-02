#ifndef __hashes_hh
#define __hashes_hh

#include <string>
#include <ostream>
#include <cstdint>

struct FNV_Hash
{
    unsigned operator () (const std::string& str) const;
};

struct Djb_Hash
{
    unsigned operator () (const std::string& str) const;
};

struct Rot_Hash
{
    unsigned operator () (const std::string& str) const;

};

struct Xor_Hash
{
    unsigned operator () (const std::string& str) const;
};

struct Add_Hash
{
    unsigned operator () (const std::string& str) const;
};


/* Integer hashes. */
struct Jenkins_Hash32
{
    uint32_t operator () (uint32_t n) const;
};

struct Hash_Stats
{
    std::string hash_name;
    unsigned collisions_count;

    bool operator < (const Hash_Stats& other) const;

};

std::ostream& operator << (std::ostream& out, const Hash_Stats& hs);

#endif // !__hashes_hh
