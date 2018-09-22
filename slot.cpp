#include "slot.hh"
#include <iostream>

using std::cout;
using std::cin;
using iz::slot_map;
using iz::numextract;
using iz::slot_info;
int main()
{
    slot_map<int> m;
    slot_info test = slot_map<int>::extract_info(12343);
    cout << test.index << ' ' << test.timestamp << '\n';
    return 0;
}