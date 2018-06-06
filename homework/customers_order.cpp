#include "heap.hh"

#include "customers_order.hh"

std::ostream& operator << (std::ostream& out, Customers& customers)  // customers cannot be const because of init_state()
{
    Stateful_Cmp_Customers::init_state(&customers);
    heap<int32_t, Stateful_Cmp_Customers> sorter;

    for (const auto& id_info : customers) {
        sorter.push(id_info.first);
    }

    while (!sorter.empty()) {
        out << customers[sorter.top()] << '\n';
        sorter.pop();
    }

    Stateful_Cmp_Customers::reset_state(nullptr);

    return out;
}

bool Stateful_Cmp_Customers::operator () (int lhs, int rhs) const
{
    req(initialized());
    return ~(*customers_info)[lhs] < ~(*customers_info)[rhs]; // => max_heap
}

/* Mark invalid state. */
Customers* Stateful_Cmp_Customers::customers_info = nullptr;

bool Stateful_Cmp_Customers::initialized()
{
    return customers_info != nullptr;
}

void Stateful_Cmp_Customers::init_state(Customers* customers)
{
    req(!initialized());
    customers_info = customers;
}

void Stateful_Cmp_Customers::reset_state(Customers* customers)
{
    req(initialized());
    customers_info = customers;
}
