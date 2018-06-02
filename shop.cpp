#include <iostream>
#include <fstream>
#include <limits>

#include "req.hh"
#include "utility.hh"
#include "hash_table.hh"
#include "hashes.hh"

#include "product.hh"
#include "customer.hh"

#if  defined (__GNUC__) || defined(__GNUG__)
#define SHOPPINGS_FILE "../input/shoppings.txt"
#define INVENTORY_FILE "../input/inventory.txt"
#else
#define SHOPPINGS_FILE "shoppings.txt"
#define INVENTORY_FILE "../input/inventory.txt"
#endif

void process_shoppings(std::ifstream&, Inventory&, Customers&);
void process_inventory(std::ifstream&, Inventory&);
void process_stats(std::ostream&, Inventory&, Customers&);

int main()
{
    std::ifstream input_data;
    Inventory products;
    Customers customers;

    input_data.open(INVENTORY_FILE);
    req(input_data.good(), "Could not open file" INVENTORY_FILE);
    /* */
    process_inventory(input_data, products);
    input_data.close();

    input_data.open(SHOPPINGS_FILE);
    req(input_data.good(), "Could not open file" SHOPPINGS_FILE);
    /* */
    process_shoppings(input_data, products, customers);
    input_data.close();

    process_stats(std::cout, products, customers);

    return 0;
}

void process_stats(std::ostream& out, Inventory& products, Customers& customers)
{
    // sort using priority_queue

    for (const auto& product : products) {
        out << product.first << '\n';
    }

    for (const auto& customer : customers) {
        out << customer.second << '\n';
    }
}

void process_inventory(std::ifstream& invetory, Inventory& products)
{
    const char comment_sign = ';';
    char first_char;

    while ((first_char = invetory.peek()) != EOF) {

        /* Ignore comment lines, trailing '\n'. */
        if (first_char == comment_sign || first_char == '\n') {
            skip_istream(invetory, "\n");
            continue;
        }

        Product temp_holder;
        invetory >> temp_holder;

        products[temp_holder] = temp_holder;
    }

    /* Hash stats. */
    std::cout << "Finished processing invetory!\n";
    std::cout << "Collisions: ";
    std::cout << products.collision_count() << '\n';
}

void process_shoppings(std::ifstream& shoppings, Inventory& products, Customers& customers)
{
    const char comment_sign = ';';
    char first_char;

    while ((first_char = shoppings.peek()) != EOF) {

        /* Ignore comment lines, trailing '\n'. */
        if (first_char == comment_sign || first_char == '\n') {
            skip_istream(shoppings, "\n");
            continue;
        }

        Customer temp_holder(&products);
        shoppings >> temp_holder;

        customers[temp_holder.id()] += temp_holder;
    }

    /* Hash stats. */
    std::cout << "Finished processing shoppings!\n";
    std::cout << "Collisions: ";
    std::cout << customers.collision_count() << '\n';
}
