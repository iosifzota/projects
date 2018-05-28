#include <iostream>
#include <string>
#include <fstream>

#include "hash_table.hh"
#include "req.hh"

/*
  DONE: more input
  DONE: count collision
  TODO: compare str hash functions
*/
void test_base();
void pause_then_clear();
void test_hash();

int main()
{
	test_base();
	pause_then_clear();
	test_hash();

	return 0;
}


#if  defined (__GNUC__) || defined(__GNUG__)
#define INPUT_FILE "../input/rand_strings.txt"
#else
#define INPUT_FILE "Text.txt"
#endif

void test_hash()
{
	std::ifstream input(INPUT_FILE);
	std::string word;
        iz::htable<std::string, int> test(15000U);

	req(input.good(), "Error opening file: " INPUT_FILE);

	int i = 0;
	for (;input >> word; ++i) {
		test.insert(word, i);
		std::cout << '.';
	}

	req(test.count() == (unsigned)i);

	std::cout << test << '\n';
	std::cout << "Number of collisions: " << test.collision_count() << '\n';
	std::cout << "Load: " << test.load() << '\n';

	input.close();
}

void test_base()
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
                  std::cout << "\tBefore: " << test["Joni"] << '\n';
                  test["Joni"] = 10;
                  std::cout << "\tAfter: " <<  test["Joni"] << '\n';
                );

        test_wrap("find",
                  req( "Joni" == (*test.static_search(test.data, "Joni")).first );
                  req( "Joni" == (*(test.find("Joni"))).first );

                  for (auto itr = test.find("Joni"); itr != test.end(); ++itr) {
                          std::cout << (*itr).first << " : " << (*itr).second << '\n';
                  }
                );

	auto bitr = test.find_bucket("Ronaldo");

	for (; !bitr.reached_end(); ++bitr) {
		std::cout << (*bitr).first << '\n';
	}
}


void pause_then_clear()
{
#ifdef _WIN32
	system("pause");
	system("cls");
#endif
}
