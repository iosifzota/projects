#ifndef __pet_hh

#define __pet_hh

#include <string>

class Pet
{
public:
    const std::string name;

    virtual void Move(bool) const;
    virtual void Speak() const = 0;

    Pet() {}

protected:
    Pet(const std::string& init_name);
};

#endif
