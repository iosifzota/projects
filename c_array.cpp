#include <iostream>
#include <functional>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdlib.h>

#include "c_array.hh"
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
	char *arr1 = strdup("Home is ahead.");
	char *arr2 = strdup("World is behind.");

	iz::c_array<char>
		test1(arr1, arr1 + strlen(arr1) + 1),
		test2(arr2, arr2 + strlen(arr2) + 1);
	free(arr1);
	free(arr2);

	std::cout << test1;
	std::cout << test2.begin() << '\n';

	test_wrap("with itr",
		iz::c_array<char> itr_cpy(test2.begin(), test2.end());
		req(itr_cpy.size() == test2.size());
		std::cout << itr_cpy.begin() << '\n';
	);

	test_wrap("op = ",
		iz::c_array<char> copied;
		copied = test1;
		std::cout << copied.begin() << '\n';
	);

	test_wrap("Concatenation",
		test1 += &test2;
		std::cout << test1;
	);

	
	test_wrap("in >>",
		std::stringstream in("1 2 3 4 5 6 7 8 9 10");

		iz::c_array<int> t;
		in >> t;
		req(!t.empty());
		req(t.size() == 10);
		std::cout << t;
	);


	test_wrap("size & push_back",
		iz::c_array<int> fd(10);
		req(fd.empty());
		req(fd.size() == 0);
		for (int i = 0; i < 3; ++i)
			fd.push_back(i);
		req(!fd.empty());
		req(fd.size() == 3);
		req(fd.capacity() == 10);
		std::cout << fd;
	);

	
	test_wrap("size & push_back on null arr",
		iz::c_array<int, 0> null;
		req(null.empty());
		req(null.size() == 0);
		for (int i = 0; i < 20; ++i)
			null.push_back(i);
		req(!null.empty());
		req(null.size() == 20);
		std::cout << null;
	);

	test_wrap("init_list",
		iz::c_array<int> ini{ 1, 2, 3, 4, 5 };
		req(!ini.empty());
		req(ini.size() == 5);
		std::cout << ini;
	);


	test_wrap("fill",
		iz::c_array<int> fill(10, -1);
		req(!fill.empty());
		req(fill.size() == 10);
		req(fill.capacity() == 10);
		std::cout << fill;
	);

	test_wrap("fill",
		iz::c_array<int> fill(20, 0);
		req(!fill.empty());
		req(fill.size() == 20);
		req(fill.capacity() == 20);
		std::cout << fill;
	);



	return 0;
}
