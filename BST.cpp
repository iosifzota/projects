//
// Created by void on 4/28/2018.
//

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>

#include "BST.h"
#include "utils.hh"
#include "req.hh"

template <typename T>
using shared_node = std::shared_ptr< iz::Node<T> >;


int main()
{
	std::stringstream in("7 16 12 14 1 6 2 9 3 8 17 19 13 4 5 10 15 11 18 20");

	iz::BST<int> test;

	test_wrap("iterator",
		req(test.empty());
		for (auto itr : test) {
			std::cout << itr << ' ';
		}

		req(test.empty());
		test.print(std::cout);

		for (int i; in >> i;) {
			test.insert(i);
		}
		req(!test.empty());

		test.print(std::cout);

		for (auto itr : test) {
			std::cout << itr << ' ';
		}
	);

	test_wrap("min & cessor for 7",
		shared_node<int> ptr_mm, ptr_sp;
		print_green("min:\n");
		ptr_mm = test.min(test.search(7));
		std::cout << ptr_mm;

		tt_wrap("pred",
			ptr_sp = test.predecessor(ptr_mm);
			std::cout << ptr_sp;
		);
		tt_wrap("succ",
			ptr_sp = test.successor(ptr_mm);
			std::cout << ptr_sp;
		);
	);

	test_wrap("max & cessor for 7",
		shared_node<int> ptr_mm, ptr_sp;
		print_green("max:\n");
		ptr_mm = test.max(test.search(7));
		std::cout << ptr_mm;

		tt_wrap("pred",
			ptr_sp = test.predecessor(ptr_mm);
		std::cout << ptr_sp;
		);
		tt_wrap("succ",
			ptr_sp = test.successor(ptr_mm);
		std::cout << ptr_sp;
		);
	);

	test_wrap("extract for 7",
		shared_node<int> extracted, save_for_assertion;

		std::cout << test.search(7);
		save_for_assertion = test.search(7);

		extracted = test.extract(test.search(7));
		std::cout << extracted;
		req(extracted == save_for_assertion);

		for (auto itr : test) {
			std::cout << itr << ' ';
		}
	);

	test_wrap("min & cessor for 8",
		shared_node<int> ptr_mm, ptr_sp;
		print_green("min:\n");
		ptr_mm = test.min(test.search(8));
		std::cout << ptr_mm;

		tt_wrap("pred",
			ptr_sp = test.predecessor(ptr_mm);
			std::cout << ptr_sp;
		);
		tt_wrap("succ",
			ptr_sp = test.successor(ptr_mm);
			std::cout << ptr_sp;
		);
	);

	test_wrap("max & cessor for 8",
		shared_node<int> ptr_mm, ptr_sp;
		print_green("max:\n");
		ptr_mm = test.max(test.search(8));
		std::cout << ptr_mm;

		tt_wrap("pred",
			ptr_sp = test.predecessor(ptr_mm);
			std::cout << ptr_sp;
		);
		tt_wrap("succ",
			ptr_sp = test.successor(ptr_mm);
			std::cout << ptr_sp;
		);
	);

	test_wrap("search for 8",
		shared_node<int> ptr_mm, ptr_sp;

		std::cout << test.search(8);

		tt_wrap("pred",
			ptr_sp = test.predecessor(ptr_mm);
			std::cout << ptr_sp;
		);
		tt_wrap("succ",
			ptr_sp = test.successor(ptr_mm);
			std::cout << ptr_sp;
		);
	);

	test_wrap("search for 6",
		shared_node<int> t_s;

		t_s = test.search(6);
		req(t_s != nullptr);
		req(t_s->data == 6);

		const shared_node<int> test_const(t_s);
		std::cout << test_const->data << '\n';
	);


	test_wrap("find all",
		for (auto itr : test) {
			req(*(test.find(itr)) == itr);
		}
	);

	return 0;
}

