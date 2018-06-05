#ifndef __customer_hh
#define __customer_hh

#include <cstddef>

#include "vec.hh"
#include "product.hh"
#include "hash_table.hh"
#include "hashes.hh"

class Customer;

using Shopping_Cart = iz::vec<std::string>;
using Customers = iz::htable<uint32_t, Customer, Jenkins_Hash32>;

class Customer
{
private:
    int32_t __id;
    Shopping_Cart __shopping_cart;
    Products* __products_info; // external state

    float __expenditure;
    unsigned __coupon_bits;

    /* Helpers */
    static const int32_t invalid_id;
    static const float no_expenditure;
    static const unsigned no_coupon;

public:
    Customer();
    explicit Customer(Products*, int32_t = invalid_id, float = no_expenditure, unsigned = no_coupon);

    Customer(const Customer&);

    ~Customer() { }

    void append_coupon(unsigned);
    void apply_coupons(unsigned);
    unsigned coupon_bits() const;
    unsigned coupon_count() const;
    static unsigned coupons_value(unsigned, unsigned);
    static unsigned bit_count(unsigned);

    void spend(float);

    void update_inventory(Products*);
    const Products* inventory() const;

    /* Id */
    int32_t id() const;
    bool registered_id() const;

    float operator ~ () const;

    /* Cart */
    Customer& operator << (const std::string&);

    /* op */
    const Customer& operator += (const Customer&);

    /* I/O */
    friend std::ostream& operator << (std::ostream&, const Customer&);
    friend std::istream& operator >> (std::istream&, Customer&);
};

#endif // !__customer_hh
