#ifndef __freq_hh
#define __freq_hh

#include <vector>
#include <istream>
#include <memory>
#include <fstream>
#include <wchar.h>
#include <stddef.h>

// TODO: Factor out these decls.
typedef unsigned wchar_t utf16;
typedef unsigned char utf8;

constexpr size_t MAX_UTF16 = (1 << 15) |
                             (1 << 14) |
                             (1 << 13) |
                             (1 << 12) |
                             (1 << 11) |
                             (1 << 10) |
                             (1 <<  9) |
                             (1 <<  8) |
                             (1 <<  7) |
                             (1 <<  6) |
                             (1 <<  5) |
                             (1 <<  4) |
                             (1 <<  3) |
                             (1 <<  2) |
                             (1 <<  1) |
                             (1);

template <typename T>
using vec = std::vector<T>;

template <typename T>
using sptr = std::shared_ptr<T>;

using std::ifstream;
using std::istream;

using byte_t = unsigned char;

struct hchar_t {
    utf16 c;
    size_t freq;
    sptr<hchar_t> left, right;

    hchar_t(utf16 c = 0, size_t freq = 0,
           sptr<hchar_t> left = nullptr,
           sptr<hchar_t> right = nullptr);
};

vec<size_t> find_freq(istream&);


/* Helpers */
bool next_char(istream& in, utf16& c);

#endif
