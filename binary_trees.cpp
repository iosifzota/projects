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
    req(mapy.empty());

	test_wrap("[] operator + size",
		mapy["Cluj"] = "North";
		mapy["Brasov"] = "Central";
		mapy["Eriador"] = "South-East";
        req(mapy.size() == 3);

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

	test_wrap("find() + size, empty",
		for (auto i = mapy.find("Cluj"); i != mapy.end(); ++i) {
			std::cout << (*i).first << ' ';
		}
		std::cout << '\n';

        /* final map size check */
        req(mapy.size() == 3);
        req(!mapy.empty());
	);

    test_wrap("map clear()",
        mapy.clear();
        req(mapy.empty());
    );


    test_wrap("rbtree - insert, size, itr, empty",
        iz::rbtree<int> redblack;
        req(redblack.empty());

        std::vector<int> aux;

        for (auto itr : { 95, 196, 302, 32, 22, 399, 490, 171, 497, 114, 91, 110, 69}) {
            aux.push_back(itr);
            redblack.insert(itr);
        }
        req(aux.size() == redblack.size());

        for (auto& itr : redblack) {
            std::cout << itr << ' ';
        }
        std::cout << '\n';

        req(!redblack.empty());

        tt_wrap("rbtree - clear",
            redblack.clear();
            req(redblack.empty());
        );
    );

	return 0;
}
