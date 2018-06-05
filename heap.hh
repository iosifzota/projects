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
    size_t sift_down(size_t);

    /*
     * Modify value at the given index.
     * Go up the heap to the final index of value.
     */
    size_t sift_up(size_t, T);

    const T& top() const;
    void pop();
    size_t push(T);
    bool empty() const;

    void sort_inplace();

    inline T& operator [] (size_t);
    inline const T& operator [] (size_t) const;

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
    req(!data.empty(), "Trying to access empty heap.");

    return data[0];
}


template< typename T, class Compare >
size_t heap<T, Compare>::push(T val)
{
    data.push_back(val);
    return sift_up(data.size() - 1, val);
}

/* TODO: Cleanup. */
template< typename T, class Compare >
size_t heap<T, Compare>::sift_up(size_t i, T val)
{
    req(i < data.size(), "Trying o access array out of bounds.");

    size_t parent_index, current_index;

    /* sift_UP */
    if (cmp(val, data[i]) || i < 1) {
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
        current_index  = parent_index, parent_index = (parent_index == 0) ? 0 : (current_index - 1) / 2
        )
    {
        data[current_index] = data[parent_index];
    }

    data[current_index] = val;
    return current_index;
}

template< typename T, class Compare >
size_t heap<T, Compare>::sift_down(size_t i)
{
    size_t left, right, max;

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
    return data.empty();
}

/* Data access. */
template< typename T, class Compare >
T& heap<T, Compare>::operator [] (size_t i)
{
    req(i < data.size(), "Trying to access array out of bounds!");

    return data[i];
}

template< typename T, class Compare >
const T& heap<T, Compare>::operator [] (size_t i) const
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
