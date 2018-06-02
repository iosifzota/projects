#ifndef __heap_hh
#define __heap_hh

#include "req.hh"
#include "utils.hh"

#include <istream>
#include <ostream>
#include <vector>

template< typename T, class Compare> class heap;

template< typename T, class Compare >
std::istream& operator >> (std::istream&, heap<T, Compare>&);

template< typename T, class Compare >
std::ostream& operator << (std::ostream&, const heap<T, Compare>&);


/* Heap. */
template< typename T, class Compare = std::less<T> >
class heap
{
protected:
    std::vector<T> data;
    static Compare cmp;

public:
    /*
     * Fix heap property from current index down,
     * assuming left and right subtrees are heaps.
     */
    unsigned sift_down(unsigned);

    /*
     * Modify value at the given index.
     * Go up the heap to the final index of value.
     */
    unsigned sift_up(unsigned, T);

    const T& top() const;
    void pop();
    unsigned push(T);
    bool empty() const;

    void sort_inplace();

    inline T& operator [] (unsigned);
    inline const T& operator [] (unsigned) const;

    friend std::istream& operator >> <T, Compare>(std::istream&, heap<T, Compare>&);
    friend std::ostream& operator << <T, Compare>(std::ostream&, const heap<T, Compare>&);

};

template <typename T, class Compare>
Compare heap<T, Compare>::cmp;

template< typename T, class Compare >
void heap<T, Compare>::pop()
{
    if (empty()) {
        return;
    }

    data[0] = data[data.size() - 1];
    data.pop_back();

    if (data.size() > 1) {
        sift_down(0);
    }
}

template< typename T, class Compare >
const T& heap<T, Compare>::top() const
{
    req(data.size(), "Trying to access empty heap.");

    return data[0];
}


template< typename T, class Compare >
unsigned heap<T, Compare>::push(T val)
{
    data.push_back(val);
    return sift_up(data.size() - 1, val);
}

template< typename T, class Compare >
unsigned heap<T, Compare>::sift_up(unsigned i, T val)
{
    req(i < data.size(), "Trying o access array out of bounds.");

    int parent_index, current_index;

    /* sift_UP */
    if (cmp(val, data[i])) {
        return i;
    }

    current_index = i;
    data[current_index] = val;

    /*
     * Iterate from parent to parent of parent <=> Insertion sort.
     */
    for (
        parent_index = (current_index - 1) / 2;
        current_index != parent_index && cmp(data[parent_index], val);
        current_index  = parent_index, parent_index = (current_index - 1) / 2
        )
    {
        data[current_index] = data[parent_index];
    }

    data[current_index] = val;
    return current_index;
}

template< typename T, class Compare >
unsigned heap<T, Compare>::sift_down(unsigned i)
{
    unsigned left, right, max;

    req(i < data.size(), "Trying o access array out of bounds.");

    left = 2*i + 1;
    right = 2*i + 2;
    max = i;

    if (left < data.size() && cmp(data[max], data[left])) {
        max = left;
    }

    if (right < data.size() && cmp(data[max], data[right])) {
        max = right;
    }

    if (max != i) {
        swap(data, i, max);
        return sift_down(max);
    }

    return i;
}

template< typename T, class Compare >
void heap<T, Compare>::sort_inplace()
{
    std::vector<T> sorted;

    while (!empty()) {
        sorted.push_back(top());
        pop();
    }

    data = sorted;
}

template< typename T, class Compare >
bool heap<T, Compare>::empty() const
{
    return data.size() == 0;
}

/* Data access. */
template< typename T, class Compare >
T& heap<T, Compare>::operator [] (unsigned i)
{
    req(i < data.size(), "Trying to access array out of bounds!");

    return data[i];
}

template< typename T, class Compare >
const T& heap<T, Compare>::operator [] (unsigned i) const
{
    req(i < data.size(), "Trying to access array out of bounds!");

    return data[i];
}


/* Printing. */
template< typename T, class Compare >
std::ostream& operator << (std::ostream& out, const heap<T, Compare>& h)
{
    out << h.data;
    return out;
}

template< typename T, class Compare >
std::istream& operator >> (std::istream& in, heap<T, Compare>& h)
{
    T val;
    while (in >> val) {
        h.push(val);
    }

    return in;
}

#endif
