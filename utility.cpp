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
int is_prime(long long n)
{
        if (n < 2) {
                return -1;
        }
        if (n < 4) {
                return 1;
        }
        if (n % 2 == 0) {
                return 0;
        }

        for (long long i = 3; i <= std::floor(std::sqrt(n)); i += 2) {
                if (n % i == 0) {
                        return 0;
                }
        }

        return 1;
}

int next_prime(int n)
{
        while (is_prime(n) != 1) {
                ++n;
        }
        return n;
}
