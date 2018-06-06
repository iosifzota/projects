#ifndef __utility_hh
#define __utility_hh

#include <istream>
#include <ostream>
#include <iostream>

/* Skip istream until a delimiter is read of until EOF. */
void skip_istream(std::istream&, const std::string&);

/* Used for calculating hash table size. */
size_t is_prime(size_t);
size_t next_prime(size_t);

#endif
