#include "customer.hh"

Customer::Customer() :
    __id { invalid_id },
    __inventory { nullptr },
    __expenditure { no_expenditure },
    __cupon { no_cupon }
{ ; }


Customer::Customer(Inventory* inventory, int id, float expenditure, float cupon) :
    __id { id },
    __expenditure { expenditure },
    __cupon { cupon }
{
    req(inventory != nullptr);
    __inventory = inventory;
}

Customer::Customer(const Customer& other) :
    __id { other.__id },
    __shopping_cart { other.__shopping_cart },
    __inventory { other.__inventory },
    __expenditure { other.__expenditure },
    __cupon { other.__cupon }
{ ; }

/* Helpers */
const int Customer::invalid_id = -1;
const float Customer::no_expenditure = 0;
const float Customer::no_cupon = 0;

float Customer::expenditure() const
{
    return __expenditure;
}

void Customer::spend(float ammount)
{
    __expenditure += ammount;
}

int Customer::id() const
{
    return __id;
}

bool Customer::registered_id() const
{
    return __id != invalid_id;
}

const Inventory* Customer::inventory() const
{
    return __inventory;
}

void Customer::update_inventory(Inventory* inventory)
{
    __inventory = inventory;
}

/* Cart */
Customer& Customer::operator << (const std::string& product_name)
{
    req(registered_id());
    req(__inventory != nullptr);

    __shopping_cart.push_back(product_name); // maybe alias +=

    Product temp(product_name); // declared just to access price
    req(__inventory->find(temp) != __inventory->end(), "Product is not in the inventory.");
    spend((*__inventory)[temp].price);

    return *this;
}

const Customer& Customer::operator += (const Customer& other)
{
    req(other.registered_id());
    req(other.__inventory != nullptr, "[Debug]"); // delete_me

    if (!registered_id()) {
        __id = other.__id;

        if (__inventory == nullptr) {
            __inventory = other.__inventory;
        }
    }

    req(__inventory != nullptr, "[Debug]"); // delete_me

    /* Append only if it's same customer. */
    if (id() == other.id()) {
        __shopping_cart  +=  other.__shopping_cart;
        __expenditure    +=  other.__expenditure;
        __cupon          +=  other.__cupon;
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

    out << customer.__id
        << '('
        << customer.__shopping_cart.size()
        << "):\n";

    for (const auto& product_name : customer.__shopping_cart) {
        out << product_name << ' ';
    }

    return out << "\tTotal: " << customer.__expenditure << '\n';
}

std::istream& operator >> (std::istream& in, Customer& customer)
{
    req(customer.__inventory != nullptr);

    std::string product_name;
    int current_customer_id;

    req(in >> current_customer_id);

    if (!customer.registered_id()) {
        customer.__id = current_customer_id;
    }

    req(customer.id() == current_customer_id, "Wrong customer.");

    req(in >> product_name);
    customer << product_name;

    return in;
}
