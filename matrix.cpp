#include "matrix.hh"
#include "req.hh"

#include <iostream>
#include <fstream>

#ifdef _WIN32
#define input_file "mat.txt"
#else
#define input_file "../input/mat.txt"
#endif

void test_matrix(std::ifstream& in);

int main()
{
        /* Input */
        std::ifstream in(input_file);
        req(in, input_file " does not exist.");

        test_matrix(in);

        return 0;
}

void test_matrix(std::ifstream& in)
{
        print_green("\ntest_matrix():\n");
        matrix<int> mat(3, 3);

        in >> mat;
        std::cout << mat;
}
