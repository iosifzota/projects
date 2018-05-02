#include <stdio.h>
#include "req.hh"

#include "utils.hh"
#include "generic.hh"

generic(N)
void test(N x) {
	printf("%d\n", sizeof(N));
}


int main()
{
	test(1);
    req(0, "1");
    req(0);
    return 0;
}
