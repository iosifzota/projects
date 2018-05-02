#include <istream>
#include <vector>

#include "freq.hh"

std::vector<unsigned> find_freq(std::istream& in)
{
    std::vector<unsigned> freq(200, 0);

	int c;

	while ((c = in.get()) != EOF) {
		freq[c]++;
	}
    
    return freq;
}


/* Constructor for h_char. */
h_char::h_char(
    char c, unsigned freq,
    std::shared_ptr<h_char> left,
    std::shared_ptr<h_char> right
    )
{
    this->c = c;
    this->freq = freq;
    this->left = left;
    this->right = right;
}
