#include "base.h"

struct(Test) {
    int p;
};

int main()
{
    Test p = { 3 };
    Test_ptr t_ptr;

    t_ptr = &p;

    t_ptr->p  = 3;

    return 0;
}
