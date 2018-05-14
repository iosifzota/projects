#include <iostream>
#include <string>
#include <utility>
#include <functional>

#include "basic_btree.hh"
#include "rbtree.hh"
#include "req.hh"

#include "map.hh"

int main()
{
	iz::map<std::string, std::string> mapy;
	
	test_wrap("[] operator",
		mapy["Cluj"] = "North";
		mapy["Brasov"] = "Central";
		mapy["Eriador"] = "South-East";

		tt_wrap("already_inserted_print", 
			std::cout << mapy["Cluj"] << '\n';
			std::cout << mapy["Brasov"] << '\n';
			std::cout << mapy["Eriador"] << '\n';
		);

		tt_wrap("already_inserted_modif_print",
			mapy["Eriador"] = "EVERYWHERE";
			std::cout << mapy["Eriador"] << '\n';
		);
	);

    for (auto itr : mapy) {
        std::cout << itr.first << ' ';
    }

	
	test_wrap("range_loop",
		for (auto itr : mapy) {
			std::cout << itr.first << ' ';
		}
		std::cout << '\n';
	);

	test_wrap("reverse_iterator",
		for (auto rev = mapy.rbegin(); rev != mapy.rend(); ++rev) {
			std::cout << (*rev).first << ' ';
		}
		std::cout << '\n';
	);

	test_wrap("mutating with iterator + print",
		for (auto itr : mapy) {
			itr.second = "asdf";
		}
		
		for (auto itr : mapy) {
			std::cout << itr.second << ' ';
		}
		std::cout << '\n';
	);

	test_wrap("find()",
		for (auto i = mapy.find("Cluj"); i != mapy.end(); ++i) {
			std::cout << (*i).first << ' ';
		}
		std::cout << '\n';
	);

	return 0;
}
