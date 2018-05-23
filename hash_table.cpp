#include <iostream>
#include <string>

#include "hash_table.hh"

template <typename Key, typename Val>
void print_htable(const iz::htable<Key, Val>& htable)
{
    std::cout << "Printing...\n";

    for (auto& bucket : htable.data) {
        for (auto& item : bucket) {
            std::cout << "( " << item.first << ", " << item.second << " )\n";
        }
    }
}


int main()
{
    iz::htable<std::string, int> test;

    test.insert("Messi", 10);
    test.insert("Messi", 10);
    test.insert("Ronaldo", 7);
    test.insert("Pele", 3);
    test.insert("Rooney", 9);

    std::cout << test;

    test.remove("Messi");
    std::cout << test;

    test.resize(43);
    std::cout << test;


    /* HERE */
    iz::htable<std::string, int>::iterator itr;

    itr = test.begin();

    while (itr != test.end()) {
        //std::cout << (*itr).first << " : " << (*itr).second << '\n';
    }


    return 0;
}
