#include <iomanip>
#include "customer.hh"

Customer::Customer()
    :
    __id { invalid_id },
    __products_info { nullptr },
    __expenditure { no_expenditure },
    __coupon_bits { no_coupon }
{ ; }


Customer::Customer(Products* inventory, int32_t id, float expenditure, unsigned coupon_bits)
    :
    __id { id },
    __expenditure { expenditure },
    __coupon_bits { coupon_bits }
{
    req(inventory != nullptr);
    __products_info = inventory;
}

Customer::Customer(const Customer& other)
    :
    __id { other.__id },
    __shopping_cart { other.__shopping_cart },
    __products_info { other.__products_info },
    __expenditure { other.__expenditure },
    __coupon_bits { other.__coupon_bits }
{ ; }

/* Helpers */
const int32_t Customer::invalid_id = -1;
const float Customer::no_expenditure = 0;
const unsigned Customer::no_coupon = 0;

/* number of bits */
unsigned Customer::bit_count(unsigned n)
{
    unsigned count;
    for (count = 0; n; ++count) {
        n >>= 1;
    }
    return count;
}

void Customer::append_coupon(unsigned bit)
{
    while (bit && (__coupon_bits & bit)) {
        bit >>= 1;
    }
    __coupon_bits |= bit;
}

void Customer::apply_coupons(unsigned inc)
{
    for (
            unsigned val = inc;
            coupon_bits() && __expenditure;
            val += inc, __coupon_bits >>= 1
            )
    {
        if (coupon_bits() & 1) {
            if (val > __expenditure) {
                __expenditure = 0;
            }
            else {
                __expenditure -= val;
            }
        }
    }
}

unsigned Customer::coupons_value(unsigned coupon_bits, unsigned inc)
{
    unsigned total_value = 0;
    for (
            unsigned val = inc;
            coupon_bits;
            val += inc, coupon_bits >>= 1
            )
    {
        if (coupon_bits & 1) {
            total_value += val;
        }
    }
    return total_value;
}

unsigned Customer::coupon_bits() const
{
    return __coupon_bits;
}

unsigned Customer::coupon_count() const
{
    return bit_count(__coupon_bits);
}

float Customer::operator ~ () const
{
    return __expenditure;
}

void Customer::spend(float ammount)
{
    __expenditure += ammount;
}

int32_t Customer::id() const
{
    return __id;
}

bool Customer::registered_id() const
{
    return __id != invalid_id;
}

const Products* Customer::inventory() const
{
    return __products_info;
}

void Customer::update_inventory(Products* inventory)
{
    __products_info = inventory;
}

/* Cart */
Customer& Customer::operator << (const std::string& product_name)
{
    req(registered_id());
    req(__products_info != nullptr);

    __shopping_cart.push_back(product_name);

    req(__products_info->find(product_name) != __products_info->end(), "Product is not in the inventory.");

    spend((*__products_info)[product_name].price);
    ++((*__products_info)[product_name]); // inc nr of purchases

    return *this;
}

const Customer& Customer::operator += (const Customer& other)
{
    req(other.registered_id());
    req(other.__products_info != nullptr, "[Debug]");

    if (!registered_id()) {
        __id = other.__id;

        if (__products_info == nullptr) {
            __products_info = other.__products_info;
        }
    }

    req(__products_info != nullptr, "[Debug]");

    /* Append only if it's same customer. */
    if (id() == other.id()) {
        __shopping_cart  +=  other.__shopping_cart;
        __expenditure    +=  other.__expenditure;
        __coupon_bits     |=  other.__coupon_bits;
    }
    else {
        std::cout << "Diff. customers:"
                  << id() << " != " << other.id() << '\n';
    }

    return *this;
}

/* I/O */
std::ostream& operator << (std::ostream& out, const Customer& customer)
{
    req(customer.registered_id());
	req(customer.__products_info != nullptr, "[Debug]");

	out << customer.__id << ": " << std::setw(3) << ~customer;

	float expense_without_coupons{};

	out << "    { ";
    for (const auto& product_name : customer.__shopping_cart) {
        out << product_name << ' ';
		req(customer.__products_info->find(product_name) != customer.__products_info->end());
		expense_without_coupons += (*(customer.__products_info))[product_name].price;
    }
	out << " } :: Discount: " << expense_without_coupons - ~customer;

	return out << "\n";
}

std::istream& operator >> (std::istream& in, Customer& customer)
{
    req(customer.__products_info != nullptr);

    std::string product_name;
    int32_t current_customer_id;

    req(in >> current_customer_id);

    if (!customer.registered_id()) {
        customer.__id = current_customer_id;
    }

    req(customer.id() == current_customer_id, "Wrong customer.");

    req(in >> product_name);
    customer << product_name;

    return in;
}


/* Coupons */
