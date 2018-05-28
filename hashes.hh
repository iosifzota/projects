#include <string>
#include <ostream>

struct FNV_Hash
{
        unsigned operator () (const std::string& str)
                {
                        unsigned h;

                        h = 2166136261; // 32-bit offset

                        for (const auto& chr : str) {
                                h = (16777619 * h) ^ chr;
                        }

                        return h;
                }
};


struct Djb_Hash
{
        unsigned operator () (const std::string& str)
                {
                        unsigned h = 0;

                        for (const auto& chr : str) {
                                h = 33 * h + chr;
                        }

                        return h;
                }
};

struct Rot_Hash
{
        unsigned operator () (const std::string& str)
                {
                        unsigned h = 0;

                        for (const auto& chr : str) {
                                h = (h << 5) ^ (h >> 27) ^ chr;
                        }

                        return h;
                }

};

struct Xor_Hash
{
        unsigned operator () (const std::string& str)
                {
                        unsigned h = 0;

                        for (const auto& chr : str) {
                                h ^= chr;
                        }

                        return h;
                }
};

struct Add_Hash
{
        unsigned operator () (const std::string& str)
                {
                        unsigned h = 0;

                        for (const auto& chr : str) {
                                h += chr;
                        }

                        return h;
                }
};


struct Hash_Stats
{
        std::string hash_name;
        unsigned collisions_count;

        bool operator < (const Hash_Stats& other) {
                return collisions_count < other.collisions_count;
        }

};

std::ostream& operator << (std::ostream& out, const Hash_Stats& hs) {
        return out << hs.hash_name << ": " << hs.collisions_count;
}
