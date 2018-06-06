#ifndef __customers_order_hh
#define __customers_order_hh

#include "customer.hh"

class Stateful_Cmp_Customers
{
private:
    static Customers* customers_info;

public:
    inline static void init_state(Customers* customers);
    inline static void reset_state(Customers* customers);
    inline static bool initialized();

    inline bool operator () (int, int) const;
};

/* Print customers */
std::ostream& operator << (std::ostream& out, Customers& customers); // cannot be constant because of init_state

#endif // !__customers_order_hh

