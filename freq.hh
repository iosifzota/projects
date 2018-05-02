#ifndef __freq_hh
#define __freq_hh

#include <vector>
#include <istream>
#include <memory>
#include <iostream>

struct h_char {
    char c;
    unsigned freq;
    std::shared_ptr<h_char> left, right;

    h_char(char c = 0, unsigned freq = 0,
           std::shared_ptr<h_char> left = nullptr,
           std::shared_ptr<h_char> right = nullptr);
};

std::vector<unsigned> find_freq(std::istream&);

#endif
