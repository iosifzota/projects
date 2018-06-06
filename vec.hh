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
void new_req(T** ptr, size_t blocks)
{
    req(blocks, "Cannot allocate 0 blocks.");
    req(ptr != nullptr);
    req((*ptr = new T[blocks]{}) != nullptr); // ++ {}
}

namespace iz {
    template <typename T, size_t GROWTH>
    class vec;

    template <typename T, size_t GROWTH>
    std::istream& operator >> (std::istream&, vec<T, GROWTH>&);

    template <typename T, size_t GROWTH>
    std::ostream& operator << (std::ostream&, const vec<T, GROWTH>&);

    template<typename T, size_t GROWTH = 5>
    class vec
    {
        private:
            T* begin_ptr;
            T* end_ptr;
            size_t cap;

            /* Memory management */
            inline void reset_fields();
            T* alloc(size_t);
            T* realloc(size_t);

            static const size_t realloc_factor;
            inline size_t calc_realloc_size() const;

        public:
            vec();
            explicit vec(size_t size);
            vec(size_t size, const T& fill_value);
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
            inline void resize(size_t);
            inline void push_back(const T&);

            /* Access */
            inline T& operator [] (size_t);
            inline const T& operator [] (size_t) const;

            /* Stats */
            inline bool empty() const;
            inline size_t size() const;
            inline size_t capacity() const;

            /* vec <op> vec */
            inline const vec& operator = (const vec&);
            inline const vec& operator += (const vec&);

            /* I/O */
            friend std::istream& operator >> <>(std::istream&, vec<T, GROWTH>&);
            friend std::ostream& operator << <>(std::ostream&, const vec<T, GROWTH>&);

            /* Helpers. */
            static size_t copy_raw(T* begin_dest, T* end_dest, const T* begin_src, const T* end_src);
    };

    template <typename T, size_t GROWTH>
    const size_t vec<T, GROWTH>::realloc_factor = 2;

    template <typename T, size_t GROWTH>
    size_t vec<T, GROWTH>::calc_realloc_size() const
    {
        return capacity() + realloc_factor * (GROWTH + 1);
    }

    template <typename T, size_t GROWTH>
    void vec<T, GROWTH>::push_back(const T& value)
    {
        if (size() >= capacity()) {
            realloc(calc_realloc_size());
        }

        *end_ptr = value;
        ++end_ptr;
    }

    template<typename T, size_t GROWTH>
    T* vec<T, GROWTH>::alloc(size_t blocks)
    {
        req(begin_ptr == nullptr);
        req(end_ptr == nullptr);
        req(blocks, "Cannot allocate 0 blocks.");

        new_req(&begin_ptr, blocks);

        end_ptr = begin_ptr;
        cap = blocks;

        return begin_ptr;
    }

    template<typename T, size_t GROWTH>
    T* vec<T, GROWTH>::realloc(size_t blocks)
    {
        T* new_storage;
        size_t new_size; // store the new size (!capacity)

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

    template<typename T, size_t GROWTH>
    size_t vec<T, GROWTH>::copy_raw(        // end must be the ptr, after the last block
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

        size_t min_size = std::min(end_dest - begin_dest, end_src - begin_src);

        for (size_t i = 0; i < min_size; ++i)
        {
            begin_dest[i] = begin_src[i];
        }

        return min_size;
    }

    template <typename T, size_t GROWTH>
    void vec<T, GROWTH>::resize(size_t new_size)
    {
        req(new_size > 0);
        realloc(new_size);

        /* Move to the end. */
        if (empty()) {
            end_ptr = begin_ptr + capacity();
        }
    }

    /* Meta */
    template<typename T, size_t GROWTH>
    size_t vec<T, GROWTH>::size() const
    {
        if (empty()) {
            return 0;
        }
        return end_ptr - begin_ptr;
    }

    template<typename T, size_t GROWTH>
    size_t vec<T, GROWTH>::capacity() const
    {
        return cap;
    }

    template<typename T, size_t GROWTH>
    bool vec<T, GROWTH>::empty() const
    {
        if (begin_ptr == nullptr) {
            req(end_ptr == nullptr);
        }
        return begin_ptr == end_ptr;
    }

    /* Access. */
    template<typename T, size_t GROWTH>
    T& vec<T, GROWTH>::operator [] (size_t i)
    {
        req(i < size(), "Trying to access out of bounds.");
        return begin_ptr[i];
    }

    template<typename T, size_t GROWTH>
    const T& vec<T, GROWTH>::operator [] (size_t i) const
    {
        req(i < size(), "Trying to access out of bounds.");
        return begin_ptr[i];
    }

    template <typename T, size_t GROWTH>
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

    template <typename T, size_t GROWTH>
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

    template<typename T, size_t GROWTH>
    void vec<T, GROWTH>::reset_fields()
    {
        begin_ptr = end_ptr = nullptr;
        cap = 0;
    }

    template<typename T, size_t GROWTH>
    vec<T, GROWTH>::vec(const T* begin_arr, const T* end_arr) // end_arr must be the ptr, after the last block
    {
        req(begin_arr != nullptr);
        req(end_arr != nullptr);
        req(begin_arr < end_arr);

        reset_fields();
        alloc(end_arr - begin_arr);

        copy_raw(begin_ptr, (end_ptr = begin_ptr + capacity()), begin_arr, end_arr);
    }

    template<typename T, size_t GROWTH>
    vec<T, GROWTH>::vec(const vec<T, GROWTH>& other)
    {
        reset_fields();
        *this = other;
    }

    template<typename T, size_t GROWTH>
    vec<T, GROWTH>::vec(size_t size, const T& fill_value)
    {
        reset_fields();
        alloc(size);

        for (size_t i = 0; i < size; ++i) {
            push_back(fill_value);
        }
    }

    template<typename T, size_t GROWTH>
    vec<T, GROWTH>::vec(size_t size)
    {
        reset_fields();
        resize(size);
    }

    template<typename T, size_t GROWTH>
    vec<T, GROWTH>::vec()
    {
        reset_fields();
    }

    template<typename T, size_t GROWTH>
    vec<T, GROWTH>::vec(std::initializer_list<T> l)
    {
        reset_fields();

        for (auto itr : l) {
            push_back(itr);
        }
    }

    template<typename T, size_t GROWTH>
    vec<T, GROWTH>::~vec()
    {
        if (begin_ptr != nullptr) {
            delete[] begin_ptr;
        }
    }

    template <typename T, size_t GROWTH>
    std::istream& operator >> (std::istream& in, vec<T, GROWTH>& arr)
    {
        T temp;

        while (in >> temp) {
            arr.push_back(temp);
        }
        return in;
    }


    template <typename T, size_t GROWTH>
    std::ostream& operator << (std::ostream& out, const vec<T, GROWTH>& arr)
    {
        for (auto itr : arr) {
            out << itr << ' ';
        }
        return out << '\n';
    }
}


#endif // !__vec_hh
