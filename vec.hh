#ifndef __vec_hh
#define __vec_hh

#include <memory>
#include <istream>
#include <ostream>
#include <initializer_list>
#include <iostream>
#include <algorithm>

#include "req.hh"

template <typename T>
void new_req(T** ptr, unsigned blocks)
{
    req(blocks, "Cannot allocate 0 blocks.");
    req(ptr != nullptr);
    req((*ptr = new T[blocks]{}) != nullptr); // ++ {}
}

namespace iz {
    template <typename T, unsigned GROWTH>
    class vec;

    template <typename T, unsigned GROWTH>
    std::istream& operator >> (std::istream&, vec<T, GROWTH>&);

    template <typename T, unsigned GROWTH>
    std::ostream& operator << (std::ostream&, const vec<T, GROWTH>&);

    template<typename T, unsigned GROWTH = 5>
    class vec
    {
        private:
            T* begin_ptr;
            T* end_ptr;
            unsigned cap;

            /* Memory management */
            inline void reset_fields();
            T* alloc(unsigned);
            T* realloc(unsigned);

            static const unsigned realloc_factor;
            inline unsigned calc_realloc_size() const;

        public:
            vec();
            explicit vec(unsigned size);
            vec(unsigned size, const T& fill_value);
            vec(const T*, const T*);
            ~vec();

            vec(const vec&);
            vec(std::initializer_list<T> l);

            /* Iterators */
            const T* begin() const {
                return begin_ptr;
            }
            const T* end() const {
                return end_ptr;
            }

            /**/
            inline void resize(unsigned);
            inline void push_back(const T&);

            /* Access */
            inline T& operator [] (unsigned);
            inline const T& operator [] (unsigned) const;

            /* Stats */
            inline bool empty() const;
            inline unsigned size() const;
            inline unsigned capacity() const;

            /* vec <op> vec */
            inline const vec& operator = (const vec&);
            inline const vec& operator += (const vec&);

            /* I/O */
            friend std::istream& operator >> <>(std::istream&, vec<T, GROWTH>&);
            friend std::ostream& operator << <>(std::ostream&, const vec<T, GROWTH>&);

            /* Helpers. */
            static unsigned copy_raw(T* begin_dest, T* end_dest, const T* begin_src, const T* end_src);
    };

    template <typename T, unsigned GROWTH>
    const unsigned vec<T, GROWTH>::realloc_factor = 2;

    template <typename T, unsigned GROWTH>
    unsigned vec<T, GROWTH>::calc_realloc_size() const
    {
        return capacity() + realloc_factor * (GROWTH + 1);
    }

    template <typename T, unsigned GROWTH>
    void vec<T, GROWTH>::push_back(const T& value)
    {
        if (size() >= capacity()) {
            realloc(calc_realloc_size());
        }

        *end_ptr = value;
        ++end_ptr;
    }

    template<typename T, unsigned GROWTH>
    T* vec<T, GROWTH>::alloc(unsigned blocks)
    {
        req(begin_ptr == nullptr);
        req(end_ptr == nullptr);
        req(blocks, "Cannot allocate 0 blocks.");

        new_req(&begin_ptr, blocks);

        end_ptr = begin_ptr;
        cap = blocks;

        return begin_ptr;
    }

    template<typename T, unsigned GROWTH>
    T* vec<T, GROWTH>::realloc(unsigned blocks)
    {
        T* new_storage;
        unsigned new_size; // store the new size (!capacity)

        req(blocks, "Cannot realloc 0 blocks.");

        if (begin_ptr == nullptr) {
            return alloc(blocks);
        }
        req(end_ptr != nullptr);

        new_req(&new_storage, blocks);
        new_size =
            copy_raw(
                new_storage, new_storage + blocks,
                begin_ptr, end_ptr
            );

        delete[] begin_ptr;
        begin_ptr = new_storage;

        end_ptr = new_storage + new_size;
        cap = blocks;

        return begin_ptr;
    }

    template<typename T, unsigned GROWTH>
    unsigned vec<T, GROWTH>::copy_raw(        // end must be the ptr, after the last block
            T* begin_dest, T* end_dest,
            const T* begin_src, const T* end_src
            )
    {
        req(begin_dest != nullptr);
        req(end_dest != nullptr);
        req(begin_dest <= end_dest);

        req(begin_src != nullptr);
        req(end_src != nullptr);
        req(begin_src <= end_src);

        unsigned min_size = std::min(end_dest - begin_dest, end_src - begin_src);

        for (unsigned i = 0; i < min_size; ++i)
        {
            begin_dest[i] = begin_src[i];
        }

        return min_size;
    }

    template <typename T, unsigned GROWTH>
    void vec<T, GROWTH>::resize(unsigned new_size)
    {
        req(new_size > 0);
        realloc(new_size);

        /* Move to the end. */
        if (empty()) {
            end_ptr = begin_ptr + capacity();
        }
    }

    /* Meta */
    template<typename T, unsigned GROWTH>
    unsigned vec<T, GROWTH>::size() const
    {
        if (empty()) {
            return 0;
        }
        return end_ptr - begin_ptr;
    }

    template<typename T, unsigned GROWTH>
    unsigned vec<T, GROWTH>::capacity() const
    {
        return cap;
    }

    template<typename T, unsigned GROWTH>
    bool vec<T, GROWTH>::empty() const
    {
        if (begin_ptr == nullptr) {
            req(end_ptr == nullptr);
        }
        return begin_ptr == end_ptr;
    }

    /* Access. */
    template<typename T, unsigned GROWTH>
    T& vec<T, GROWTH>::operator [] (unsigned i)
    {
        req(i < size(), "Trying to access out of bounds.");
        return begin_ptr[i];
    }

    template<typename T, unsigned GROWTH>
    const T& vec<T, GROWTH>::operator [] (unsigned i) const
    {
        req(i < size(), "Trying to access out of bounds.");
        return begin_ptr[i];
    }

    template <typename T, unsigned GROWTH>
    const vec<T, GROWTH>& vec<T, GROWTH>::operator += (const vec<T, GROWTH>& other)
    {
        if (other.empty()) {
            return *this;
        }

        realloc(size() + other.size());
        copy_raw(end_ptr, begin_ptr + capacity(), other.begin(), other.end());

        end_ptr = begin_ptr + capacity();

        return *this;
    }

    template <typename T, unsigned GROWTH>
    const vec<T, GROWTH>& vec<T, GROWTH>::operator = (const vec<T, GROWTH>& other)
    {
        if (begin_ptr != nullptr) {
            req(end_ptr != nullptr);
            delete[] begin_ptr;
        }

        reset_fields();

        if (other.empty()) {
            return *this;
        }

        alloc(other.size());
        copy_raw(begin_ptr, (end_ptr = begin_ptr + capacity()), other.begin(), other.end());

        return *this;
    }

    template<typename T, unsigned GROWTH>
    void vec<T, GROWTH>::reset_fields()
    {
        begin_ptr = end_ptr = nullptr;
        cap = 0;
    }

    template<typename T, unsigned GROWTH>
    vec<T, GROWTH>::vec(const T* begin_arr, const T* end_arr) // end_arr must be the ptr, after the last block
    {
        req(begin_arr != nullptr);
        req(end_arr != nullptr);
        req(begin_arr < end_arr);

        reset_fields();
        alloc(end_arr - begin_arr);

        copy_raw(begin_ptr, (end_ptr = begin_ptr + capacity()), begin_arr, end_arr);
    }

    template<typename T, unsigned GROWTH>
    vec<T, GROWTH>::vec(const vec<T, GROWTH>& other)
    {
        reset_fields();
        *this = other;
    }

    template<typename T, unsigned GROWTH>
    vec<T, GROWTH>::vec(unsigned size, const T& fill_value)
    {
        reset_fields();
        alloc(size);

        for (unsigned i = 0; i < size; ++i) {
            push_back(fill_value);
        }
    }

    template<typename T, unsigned GROWTH>
    vec<T, GROWTH>::vec(unsigned size)
    {
        reset_fields();
        resize(size);
    }

    template<typename T, unsigned GROWTH>
    vec<T, GROWTH>::vec()
    {
        reset_fields();
    }

    template<typename T, unsigned GROWTH>
    vec<T, GROWTH>::vec(std::initializer_list<T> l)
    {
        reset_fields();

        for (auto itr : l) {
            push_back(itr);
        }
    }

    template<typename T, unsigned GROWTH>
    vec<T, GROWTH>::~vec()
    {
        if (begin_ptr != nullptr) {
            delete[] begin_ptr;
        }
    }

    template <typename T, unsigned GROWTH>
    std::istream& operator >> (std::istream& in, vec<T, GROWTH>& arr)
    {
        T temp;

        while (in >> temp) {
            arr.push_back(temp);
        }
        return in;
    }


    template <typename T, unsigned GROWTH>
    std::ostream& operator << (std::ostream& out, const vec<T, GROWTH>& arr)
    {
        for (auto itr : arr) {
            out << itr << ' ';
        }
        return out << '\n';
    }
}


#endif // !__vec_hh
