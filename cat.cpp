#include <string>
#include <iostream>
#include "cat.hh"

Cat::Cat(const std::string& init_name, bool init_picky)
    : Pet(init_name), isPicky { init_picky }
{
}

void Cat::Move(bool move) const
{
    std::cout << name << " ";
    if (move) {
        std::cout << "pounces.\n";
    }
    else {
        std::cout << "slinks.\n";
    }
}

void Cat::Speak() const
{
    std::cout << "Meows.\n";
}
