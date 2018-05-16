#include <iostream>
#include <sstream>

#include "rbtree.hh"
#include "req.hh"

#include "utils.hh"

int test();

int main()
{
	test();
	return 0;
}

int test()
{
	iz::rbtree<int> x;

	x.insert(11);

	for (int i = 20; i > 0; --i) {
		if (i == 11) {
			continue;
		}
		x.insert(i);
	}

	for (auto itr : x) {
		std::cout << itr << ' ';
	}
	std::cout << '\n';

	req(x.find(5) != x.end());
	req(*(x.find(5)) == 5);
	req(x.extract(x.search(5))->data == 5);
	req(x.find(5) == x.end());

	req(x.find(234) == x.end());

	for (auto itr : x) {
		std::cout << itr << ' ';
	}
	std::cout << '\n';

	for (auto itr = x.find(10); itr != x.end(); ++itr) {
		std::cout << *itr << ' ';
	}
	std::cout << '\n';

	req(x.find(1) != x.end());
	req(*(x.find(1)) == 1);
	req(x.extract(x.search(1))->data == 1);
	req(x.find(1) == x.end());

	for (auto itr : x) {
		std::cout << itr << ' ';
	}
	std::cout << '\n';

/*
	test_wrap("levels",
		x.print_levels(std::cout);
	); */

 	return 0;
}
