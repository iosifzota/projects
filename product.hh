#ifndef __product_hh
#define __product_hh

#include <string>
#include "hashes.hh"

struct Product
{
    std::string name;
    float price;
    float profit;

    static const std::string invalid_name;
    static const float invalid_price, invalid_profit;

    Product();

    explicit Product(const std::string&, float init_price = invalid_price, float init_profit = invalid_profit);

    bool operator == (const Product&) const;
    bool operator != (const Product&) const;

    friend std::ostream& operator << (std::ostream&, const Product&);
    friend std::istream& operator >> (std::istream&, Product&);

    struct Hash {
        static const Djb_Hash djb_hash;
        size_t operator () (const Product&) const;
    };
};

class Product_Stats : public Product
{
private:
    size_t numberof_purchases;

public:
    Product_Stats();
    Product_Stats(const std::string&, float init_price = invalid_price, float init_profit = invalid_profit);

    const Product_Stats& operator = (const Product&);

    size_t gross() const;
    size_t net() const;

    /* meh */
    size_t priority;
};

#endif // !__product_hh
