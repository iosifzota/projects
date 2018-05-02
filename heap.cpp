#include "heap.hh"
#include "utils.hh"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#define input1 "1 2 3 4 5 6 7"
#define input2 "34 31 2 9 10 0 4 98 1 3 7"

int main()
{
    heap<int> h;

    std::stringstream in1(input1);
    in1 >> h;

    std::cout << h;

    h.push(14);
    nl; std::cout << h;

    h.push(10);
    nl; std::cout << h;

    h.push(0);
    nl; std::cout << h;

    std::cout << h.top(); nl;

    h.pop();
    nl; std::cout << h;

    h.sort_inplace();
    std::cout << h; nl;

    heap<int, std::greater<int> > heapy;

    std::stringstream in2(input2);
    in2 >> heapy;

    heapy.sort_inplace();
    std::cout << heapy; nl;


    return 0;
}
