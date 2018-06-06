#ifndef __dog_hh

#define __dog_hh

#include <string>
#include "pet.hh"

class Dog : public Pet
{
public:
    Dog(const std::string&, bool);

    void Move(bool) const override;
    void Speak() const override;

    const bool canFetch;

};

#endif
