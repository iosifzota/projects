#include <iostream>
#include <functional>
#include <string.h>
#include <stdlib.h>

#include "c_array.hh"
#include "utils.hh"
#include "req.hh"
#include "which_compiler.h"

int main()
{
        char *arr1 = strdup("Home is ahead.");
        char *arr2 = strdup("World is behind.");

        iz::c_array<char> test1(arr1, arr1 + (strlen(arr1) - 1)), test2(arr2, arr2 + (strlen(arr2) - 1));

        test1 >> std::cout; nl;
        test2 >> std::cout; nll;

        test1 += test2;
        test1 >> std::cout; nl;

        free(arr1);
        free(arr2);
        return 0;
}
