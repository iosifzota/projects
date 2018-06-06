#include <string>
#include <iostream>
#include "dog.hh"

Dog::Dog(const std::string& init_name, bool init_fetch)
    : Pet(init_name), canFetch { init_fetch }
{
}

void Dog::Move(bool move) const
{
    std::cout << name << " ";
    if (move) {
        std::cout << "bounds.\n";
    }
    else {
        std::cout << "stride.\n";
    }
}

void Dog::Speak() const
{
    std::cout << "Barks.\n";
}
