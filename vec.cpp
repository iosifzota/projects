#include "vec.hh"
#include "utils.hh"
#include "req.hh"

#include <vector>


int main()
{
        iz::vector<int> x(5);

        std::cout << x;
        x[0] = 3;
        std::cout << x;

        x[4] = 2341;
        std::cout << x;

        x.resize(10, true);
        std::cout << x;

        // x.resize(12);
        // std::cout << x;

        print_green("\nstd::vector:\n");
        std::vector<int> v(5);

        v.push_back(1);
        std::cout << v; nl;


        for (int i = 2; i <= 100; ++i) {
                v.push_back(i);
        }

        std::cout << v;


        return 0;
}
