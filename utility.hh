#ifndef __utility_hh
#define __utility_hh

#include <istream>
#include <ostream>
#include <iostream>


#define nl std::cout << std::endl
#define nll nl; nl;
#define nlll nll; nl;
#define nn(n) for (int i = 0; i < n; ++i) std::cout << std::endl;


/* Skip istream until a delimiter is read of until EOF. */
void skip_istream(std::istream&, const std::string&);
int is_prime(long long);
int next_prime(int);

#endif
