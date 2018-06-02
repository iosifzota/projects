#ifndef __customer_hh
#define __customer_hh

#include "vec.hh"
#include "product.hh"
#include "hash_table.hh"

class Customer;

using Inventory = iz::htable<Product, Product_Stats, Product::Hash>;
using Shopping_Cart = iz::vec<std::string>;
using Customers = iz::htable<uint32_t, Customer, Jenkins_Hash32>;

class Customer
{
private:
    int __id;
    Shopping_Cart __shopping_cart;
    Inventory* __inventory;

    float __expenditure, __cupon;

    /* Helpers */
    static const int invalid_id;
    static const float no_expenditure, no_cupon;

public:
    Customer();
    explicit Customer(Inventory*, int = invalid_id, float = no_expenditure, float = no_cupon);

    Customer(const Customer&);

    ~Customer() { }

    /* WAIT */
    bool eligble_for_cupon(float) const;
    void add_cupon(float);
    /* */

    float expenditure() const;
    void spend(float);

    void update_inventory(Inventory*);
    const Inventory* inventory() const;

    /* Id */
    int id() const;
    bool registered_id() const;

    /* Cart */
    Customer& operator << (const std::string&);

    /* op */
    const Customer& operator += (const Customer&);

    /* I/O */
    friend std::ostream& operator << (std::ostream&, const Customer&);
    friend std::istream& operator >> (std::istream&, Customer&);

    /* meh */
    size_t priority;
};

#endif // !__customer_hh
