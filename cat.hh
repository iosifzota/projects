#ifndef __cat_hh

#define __cat_hh

#include <string>
#include "pet.hh"

class Cat : public Pet
{
public:
    Cat(const std::string&, bool);

    void Move(bool) const override;
    void Speak() const override;

    const bool isPicky;
};

#endif
