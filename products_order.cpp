#include <iomanip>

#include "heap.hh"

#include "products_order.hh"

std::ostream& operator << (std::ostream& out, Products& products) // products cannot be const because of init_state()
{
    Stateful_Cmp_Products::init_state(&products);
    heap<std::string, Stateful_Cmp_Products> sorter;

    for (const auto& name_stats : products) {
        sorter.push(name_stats.first);
    }

    while (!sorter.empty()) {
        out << sorter.top() << ": "
            << std::setprecision(2) << std::setw(3) << products[sorter.top()].net()
            << " from "
            << products[sorter.top()].purchases();

        (products[sorter.top()].purchases() == 1) ?
            out << " purchase.\n" : out << " purchases.\n";

        sorter.pop();
    }

    Stateful_Cmp_Products::reset_state(nullptr);

    return out;
}

bool Stateful_Cmp_Products::operator () (const std::string& lhs, const std::string& rhs) const
{
    req(initialized());
    return (*products_info)[lhs].net() < (*products_info)[rhs].net(); // => max_heap
}

/* Mark invalid state. */
Products* Stateful_Cmp_Products::products_info = nullptr;

bool Stateful_Cmp_Products::initialized()
{
    return products_info != nullptr;
}

void Stateful_Cmp_Products::init_state(Products* products)
{
    req(!initialized());
    products_info = products;
}

void Stateful_Cmp_Products::reset_state(Products* products)
{
    req(initialized());
    products_info = products;
}
