#include <iostream>
#include <string>

#include "hash_table.hh"
#include "req.hh"

int main()
{
    iz::htable<std::string, int> test;

	test_wrap("insert",
		test.insert("Messi", 10);
		test.insert("Messi", 10);
		test.insert("Ronaldo", 7);
		test.insert("Pele", 3);
		test.insert("Rooney", 9);
		
		std::cout << test;
	);

	test_wrap("remove",
		test.remove("Messi");
		std::cout << test;
	);

	test_wrap("resize",
		test.resize(43);
		std::cout << test;
	);

	test_wrap("Modify data using iterators",
		print_green("Before:\n");
		std::cout << test << '\n';

		for (auto& itr : test) {
			itr.second = 0;
		}
		
		print_green("After:\n");
		std::cout << test << '\n';
	);

	test_wrap("operator[]",
		std::cout << "\tBefore: " << test["Messi"] << '\n';
		test["Messi"] = 10;
		std::cout << "\tAfter: " <<  test["Messi"] << '\n';
	);

    return 0;
}
