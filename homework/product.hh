#ifndef __product_hh
#define __product_hh

#include <string>

#include "hash_table.hh"
#include "hashes.hh"

struct Product;

using Products = iz::htable<std::string, Product, Djb_Hash>;

struct Product
{
public:
    std::string name;
    float price;
    float profit;

    static const std::string invalid_name;
    static const float invalid_price, invalid_profit;

    Product();
    explicit Product(const std::string&, float = invalid_price, float = invalid_profit);

    bool operator == (const Product&) const;
    bool operator != (const Product&) const;

    const Product& operator = (const Product&);

	friend std::ostream& operator << (std::ostream&, const Product&);
	friend std::istream& operator >> (std::istream&, Product&);

    /* Product stats */
    float gross() const;
    float net() const;
    size_t purchases() const;

	/* Inc numberof_purchases */
    Product& operator ++ ();

private:
    size_t numberof_purchases;
};

#endif // !__product_hh
