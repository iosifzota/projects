#include "base.h"
#include "req.h"

#include "memory.h"

struct(Test) {
    int m;
};

int main()
{
    int m;
    Test_ptr p;

    p = reserve(1, sizeof(*p));
    m = p->m = 3;

    free(p);
    return m;
}
