#ifndef __priority_queue_hh
#define __priority_queue_hh

#include "heap.hh"
#include "req.hh"

template< typename T, class Compare = std::less<T> >
class priority_queue : public heap<T, Compare>
{
public:
    using heap<T, Compare>::heap;
    unsigned priority(unsigned, T);
};

template< typename T, class Compare >
unsigned priority_queue<T, Compare>::priority(unsigned i, T val)
{
    req(i < this->data.size());

    if (val, this->data[i]) {
        return this->sift_up(i, val);
    }

    this->data[i] = val;
    return this->sift_down(i);
}


#endif
