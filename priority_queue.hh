#ifndef __priority_queue_hh
#define __priority_queue_hh

#include "heap.hh"

template< typename T, class Compare = std::less<T> >
class priority_queue : public heap<T, Compare>
{
public:
    using heap<T, Compare>::heap;
    void priority(unsigned, T);
};

template< typename T, class Compare >
void priority_queue<T, Compare>::priority(unsigned i, T val)
{
    heap<T, Compare>::sift_up(i, val);
}


#endif
