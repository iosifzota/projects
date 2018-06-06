#include <iostream>
#include <vector>
#include "cat.hh"
#include "dog.hh"


void Exercise(std::vector<Pet*> pets)
{
    Dog* dog_ptr;
    Cat* cat_ptr;

    for (unsigned i = 0; i < pets.size(); ++i) {
        Pet *pet = pets[i];

        if( (cat_ptr = dynamic_cast<Cat*>(pet)) ) {
            cat_ptr->Speak();
            cat_ptr->Move(true);
        }
        else if( (dog_ptr = dynamic_cast<Dog*>(pet)) ) {
            dog_ptr->Speak();
            dog_ptr->Move(true);
        }
    }

}


template <int Duration>
void Exercise(Pet* pet)
{
    Dog* dog_ptr;
    Cat* cat_ptr;

    if( (cat_ptr = dynamic_cast<Cat*>(pet)) ) {
        cat_ptr->Speak();
        for (int i = Duration; i > 0; --i) {
            cat_ptr->Move(true);
        }
    }
    else if( (dog_ptr = dynamic_cast<Dog*>(pet)) ) {
        dog_ptr->Speak();
        for (int i = Duration; i > 0; --i) {
            dog_ptr->Move(true);
        }
    }

}

int main()
{
    Cat nana("Nana", false);
    Dog zac("Zac", true);

    /*
        ‘class Pet’ has no member named ‘isPicky’

        Campurile din clasele derivate nu pot fi accesat prin intermediul
        unei referinte catre baza. Doar capurile din baza pot fi accesate astfel.
       */
    // Pet& nana_ref = nana;
    // Pet& zac_ref = zac;
    //
    // if (!nana_ref.isPicky)) {
    //     std::cout << nana_ref.name << "isn't picky.\n";
    // }

    Exercise<3>(&zac);

    // -----------------------------------
    /*

    */
    std::cout << "---------------\n";
    std::vector<Pet*> pets;

    Cat* n = new Cat("Nana", false);
    Dog* z = new Dog("Zac", true);
    pets.push_back(n);
    pets.push_back(z);

    Exercise(pets);

    return 0;
}
