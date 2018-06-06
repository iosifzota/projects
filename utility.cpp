#include <istream>
#include <string>
#include <cmath>

#include "utility.hh"

void skip_istream(std::istream& stream, const std::string& delim)
{
    char c;

    while ( (c = stream.get()) != EOF &&
            delim.find(c) == std::string::npos )
    { ; }
}

/* Helpers */
size_t is_prime(size_t n)
{
        if (n < 2 || n % 2 == 0) {
                return 0;
        }
        if (n < 4) {
                return 1;
        }

        for (size_t i = 3; i <= std::floor(std::sqrt((double)n)); i += 2) {
                if (n % i == 0) {
                        return 0;
                }
        }

        return 1;
}

size_t next_prime(size_t n)
{
        while (is_prime(n) != 1) {
                ++n;
        }
        return n;
}
