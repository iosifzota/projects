#include <ostream>
#include <istream>

#include "product.hh"
#include "req.hh"

const std::string Product::invalid_name = "invalid_name";
const float Product::invalid_price = -1;
const float Product::invalid_profit = -2;

Product::Product()
    :
    name { invalid_name },
    price { invalid_price },
    profit { invalid_profit },
    numberof_purchases { 0 }
{ ; }

Product::Product(const std::string& init_name, float init_price, float init_profit)
    :
    name { init_name },
    price { init_price },
    profit { init_profit },
    numberof_purchases { 0 }
{
    req(!init_name.empty(), "Invalid product name.");
}


bool Product::operator == (const Product& other) const
{
    return name == other.name;
}

bool Product::operator != (const Product& other) const
{
    return !(*this == other);
}

std::ostream& operator << (std::ostream& out, const Product& product)
{
    return out << product.name  << ' '
               << product.price << ' '
               << product.profit;
}

std::istream& operator >> (std::istream& in, Product& product)
{
    req(in >> product.name);
    req(in >> product.price);
    req(in >> product.profit);

    return in;
}

const Product& Product::operator = (const Product& other)
{
    this->name = other.name;
    this->price = other.price;
    this->profit = other.profit;
    this->numberof_purchases = other.numberof_purchases;

    return *this;
}

float Product::gross() const
{
    return numberof_purchases * this->price;
}

float Product::net() const
{
    return numberof_purchases * this->profit;
}

size_t Product::purchases() const
{
    return numberof_purchases;
}

Product& Product::operator ++ ()
{
    ++numberof_purchases;
    return *this;
}
