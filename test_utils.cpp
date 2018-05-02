#include <iostream>

#include "utils.hh"
#include "req.hh"

int main()
{
    //assert_(x != 0 || (( "x is zero" && 0)));

    //assert__(x != 0 || (( "x is zero" && 0)));

    int x = 0;

    req( x == 0, "Supposed to not be zero" );


    return 0;
}
