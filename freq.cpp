#include <istream>
#include <vector>
#include <wchar.h>
#include <iostream>

#include "freq.hh"

/* c == 0 if EOF is read. */
bool next_char(istream& in, utf16& c)
{
    int temp = in.get();

    if (temp == EOF) {
        c = 0;
        return false;
    }

    c = temp;
    return true;
}

vec<size_t> find_freq(istream& in)
{
    vec<size_t> freq(MAX_UTF16, 0);
    for (
            utf16 c;
            next_char(in, c);
            freq[c]++
        )
    {}

    return freq;
}


hchar_t::hchar_t(
    utf16 c_, size_t freq_,
    sptr<hchar_t> left_,
    sptr<hchar_t> right_
    ) :
    c {c_},
    freq {freq_},
    left {left_},
    right {right_}
{}




