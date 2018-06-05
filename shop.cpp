#include <iostream>
#include <fstream>
#include <limits>
#include <random>

#include "req.hh"
#include "utility.hh"
#include "hash_table.hh"
#include "hashes.hh"

#include "product.hh"
#include "products_order.hh"
#include "customer.hh"
#include "customers_order.hh"
#include "heap.hh"

#if  defined (__GNUC__) || defined(__GNUG__)
#define SHOPPINGS_FILE "../input/shoppings.txt"
#define INVENTORY_FILE "../input/inventory.txt"
#else
#define SHOPPINGS_FILE "shoppings.txt"
#define INVENTORY_FILE "inventory.txt"
#endif

#define OUTPUT_FILE "output_data.txt"

/* Process inventory data */
std::istream& operator >> (std::istream&, Products&);

void process_shoppings(std::ifstream&, Products&, Customers&);
void process_stats_and_apply_coupons(std::ostream&, Products&, Customers&);

/*
Programul prelucreaza niste cumparaturi aplicand aleator cupoane cumparatorilor.
Apoi se afiseaza clientii in ordine dupa suma cheltuita (tinand cont de cupoanele primite),
produsele in ordinea profitului net al "magazinului".

La sfarsit se afiseaza niste statistici despre cupoane: cate s-au dat in total,
cat valoareaza in total, apoi cate cupoane s-au folosit efectiv si care este valoare lor totala.
*/

int main()
{
    std::ifstream input_data;
    std::ofstream output_data;

    Products products;
    Customers customers;

	/* Process inventory data */
    input_data.open(INVENTORY_FILE);
    req(input_data.good(), "Could not open file" INVENTORY_FILE);

    input_data >> products;
    input_data.close();

	/* Process shoppings data */
    input_data.open(SHOPPINGS_FILE);
    req(input_data.good(), "Could not open file" SHOPPINGS_FILE);

    process_shoppings(input_data, products, customers);
    input_data.close();

	/* Process stats */
    output_data.open(OUTPUT_FILE);
    req(output_data.good());

    process_stats_and_apply_coupons(output_data, products, customers);
    output_data.close();

    std::cout << "Output written to ";
    print_green(OUTPUT_FILE);
    std::cout << ".\n";

    return 0;
}

/* Process inventory data */
std::istream& operator >> (std::istream& invetory_data, Products& products)
{
    const char comment_sign = ';';
    char first_char;

    while ((first_char = invetory_data.peek()) != EOF) {

        /* Ignore comment lines, trailing '\n'. */
        if (first_char == comment_sign || first_char == '\n') {
            skip_istream(invetory_data, "\n");
            continue;
        }

        Product temp_holder;
        invetory_data >> temp_holder;

        products[temp_holder.name] = temp_holder;
    }

    /* Hash stats. */
    std::cout << "Finished processing inventory data!\n";
    std::cout << "Collisions: ";
    std::cout << products.collision_count() << '\n';

    return invetory_data;
}

/* Read in shoppings data and randomly distribute coupons */
void process_shoppings(std::ifstream& shoppings, Products& products, Customers& customers)
{
    const char comment_sign = ';';
    char first_char;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::binomial_distribution<> d(12, 0.5);
	auto dice = std::bind (d, gen);

    int lucky_number = dice();
    int coupons_max_count = lucky_number / 2 + 1;

    while ((first_char = shoppings.peek()) != EOF) {

        /* Ignore comment lines, trailing '\n'. */
        if (first_char == comment_sign || first_char == '\n') {
            skip_istream(shoppings, "\n");
            continue;
        }

        Customer temp_holder(&products);
        shoppings >> temp_holder;

        customers[temp_holder.id()] += temp_holder;

        if (dice() == lucky_number) {
            customers[temp_holder.id()].append_coupon(1 << coupons_max_count);
        }
    }

    /* Hash stats. */
    std::cout << "Finished processing shoppings!\n";
    std::cout << "Collisions: ";
    std::cout << customers.collision_count() << '\n';
}


void process_stats_and_apply_coupons(std::ostream& out, Products& products, Customers& customers)
{
	const unsigned coupon_base_val = 20;

	/* Calculate total coupons count and value; and apply coupons. */
	unsigned total_coupon_count{}, total_coupon_value{};

	for (auto& customer : customers) {
		unsigned coupon_bits = customer.second.coupon_bits();
		customer.second.apply_coupons(coupon_base_val);

		total_coupon_count += Customer::bit_count(coupon_bits);
		total_coupon_value += Customer::coupons_value(coupon_bits, coupon_base_val);
	}

	/* Calculate remaining coupons count and value. */
	unsigned remaining_coupon_count{}, remaining_coupon_value{};

	for (const auto& customer : customers) {
		unsigned coupon_bits = customer.second.coupon_bits();

		remaining_coupon_count += Customer::bit_count(coupon_bits);
		remaining_coupon_value += Customer::coupons_value(coupon_bits, coupon_base_val);
	}

	/* Effective = Total - Remaining. */
	unsigned effective_coupon_count, effective_coupon_value;
	effective_coupon_count = total_coupon_count - remaining_coupon_count;
	effective_coupon_value = total_coupon_value - remaining_coupon_value;

	/* Printing stats... */
	std::cout << '\n';
	out << '\n';
	
	/* Ordered printing of products and customers */
	print_green("Products ordered by shop's net income:\n");
	std::cout << products << '\n'; // Sorted decreasing by shop's net income.
	out << "Product ordered by shop's net income:\n" << products << '\n';

	print_green("Customers ordered by expenditure:\n");
	std::cout << customers << '\n'; // Sorted decreasing by expenditure.
	out << "Customer ordered by expenditure:\n" << customers << '\n';

	/* Coupon stats */
	print_green("Coupons Stats\n");
	out << "Coupons Stats\n";

	std::cout << "Total coupons count: " << total_coupon_count << '\n';
	out << "Total coupons count: " << total_coupon_count << '\n';
	std::cout << "Total coupons value: " << total_coupon_value << '\n';
	out << "Total coupons value: " << total_coupon_value << '\n';

	std::cout << "Effective coupons count: " << effective_coupon_count << '\n';
	out << "Effective coupons count: " << effective_coupon_count << '\n';
	std::cout << "Effective coupons value: " << effective_coupon_value << "\n\n";
	out << "Effective coupons value: " << effective_coupon_value << "\n\n";
}