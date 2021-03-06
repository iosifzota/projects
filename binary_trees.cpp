#include <iostream>
#include <string>
#include <utility>
#include <functional>

#include "basic_btree.hh"
#include "rbtree.hh"
#include "req.hh"

#include "map.hh"

struct int_str {
    int x;
    std::string str;

    const int_str& operator += (const int_str& other) {
        this->x += other.x;
        return *this;
    }
};

namespace std {
    template<>
    struct less<int_str> {
        bool operator () (const int_str& lhs, const int_str& rhs)
            {
                return lhs.x < rhs.x;
            }
    };
}

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

    iz::rbtree<int> test_eq;
    iz::rbtree<int> redblack;
    test_wrap("rbtree - insert, size, itr, empty",
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

              tt_wrap("Preorder",
                      redblack.print_tree(1);
                  );

              tt_wrap("Inorder",
                      redblack.print_tree(2);
                  );

              tt_wrap("Endorder",
                      redblack.print_tree(3);
                  );

              tt_wrap("Levels",
                      redblack.print_tree(4);
                  );

              tt_wrap("operator = 0",
                      req(test_eq.empty());

                      test_eq = redblack;
                      req(!test_eq.empty());
                      req(test_eq.size() == redblack.size());

                      /* check test_eq == redblack */
                      {
                          auto redblack_itr = redblack.begin();
                          auto test_eq_itr = test_eq.begin();
                          for (;
                               redblack_itr != redblack.end() && test_eq_itr != test_eq.end();
                               ++redblack_itr, ++test_eq_itr
                              )
                          {
                              req(*redblack_itr == *test_eq_itr);
                          }
                      }

                  );

              tt_wrap("rbtree - clear",
                      redblack.clear();
                      req(redblack.empty());
                  );

              tt_wrap("print test_eq",
                      for (auto& itr : test_eq) {
                          std::cout << itr << ", ";
                      }
                      std::cout << '\n';
                  );
	);

    iz::rbtree<int> test_median;
    test_wrap("Median",
              for (auto i : { 1, 2, 3, 4, 5 }) {
                  test_median.insert(i);
              }
              std::cout << "Result: " << test_median.median(3, 5) << '\n';
	);

    test_wrap("level printing",
              test_median.print_levels(std::cout);
              std::cout << "\n\n";
              test_eq.print_levels(std::cout);
        );


    test_wrap("Median",
              std::cout << "Result: " << test_eq.median(32, 302) << '\n';
	);

    //iz::rbtree<int_str> test;

    //test.insert({ 3,  "child" });
    //test.insert({ 21, "bchild" });
    //test.insert({ 4, "ahild" });

    //for (auto& itr : test) {
    //	std::cout << itr.str << '(' << itr.x << ") ";
    //}
    //std::cout << '\n';

    return 0;
}
