#include "priority_queue.hh"
#include "utils.hh"

#include <iostream>
#include <sstream>

#define input1 "1 2 3 4 5 6 7 8"

int main()
{
    priority_queue<int, std::less<int> > l;

    std::stringstream in(input1);

    in >> l;
    std::cout << l; nl;

    l.priority(3, 99);
    std::cout << l; nl;

    l.sort_inplace();
    std::cout << l; nl;

    return 0;
}
