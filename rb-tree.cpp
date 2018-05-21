#include <iostream>
#include <sstream>
#include <vector>

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
	std::cout << "\n\n";

	x.print_tree(4);

	std::cout << "\n\n\n";


	std::vector<int> test_vec{ 9, 5, 12, 10, 2, 3, 8, 11, 20, 6 };
	iz::rbtree<int> rbtree_cstr_test;

	req(rbtree_cstr_test.empty());
	req(!test_vec.empty());

	rbtree_cstr_test.construct(&test_vec[0], &test_vec[test_vec.size() - 1] + 1);
	req(!rbtree_cstr_test.empty());

	for (auto itr : rbtree_cstr_test) {
			std::cout << itr << ' ';
		}
	std::cout << '\n';



 	return 0;
}
