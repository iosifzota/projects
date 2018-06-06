#ifndef __products_order_hh
#define __products_order_hh

#include "product.hh"
#include "hash_table.hh"
#include "hashes.hh"

class Stateful_Cmp_Products
{
private:
    static Products* products_info;

public:
    inline static void init_state(Products* products);
    inline static void reset_state(Products* products);
    inline static bool initialized();

    inline bool operator () (const std::string&, const std::string&) const;
};

/* Print products sorted by shop's net income. */
std::ostream& operator << (std::ostream& out, Products& products);

#endif // !__products_order_hh

