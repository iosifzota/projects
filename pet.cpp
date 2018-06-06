#include <string>
#include <iostream>
#include "pet.hh"


Pet::Pet(const std::string& init_name)
    : name { init_name }
{
    ;
}

void Pet::Move(bool move) const
{
    std::cout << name << " ";
    if (move) {
        std::cout << "runs.\n";
    }
    else {
        std::cout << "walks.\n";
    }
}
