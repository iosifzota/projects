#ifndef __c_array_hh
#define __c_array_hh

#include <memory>
#include <istream>
#include <ostream>
#include <functional>
#include <iostream>
#include <sstream>

#include "req.hh"
#include "utils.hh"

namespace iz {
        template<typename T>
        class c_array
        {
        private:
                T* begin;
                T* end;

                bool safe;

                void init();
                T* alloc(unsigned);


        public:
                c_array();
                c_array(unsigned size);
                /* */
                c_array(const c_array&);
                c_array(const c_array*);
                c_array(const T*, const T*);
                /* */
                ~c_array();

                const T* ptr() const {
                        return begin;
                }
                const T* bptr() const {
                        return begin;
                }
                const T* eptr() const {
                        return end;
                }

                T* realloc(unsigned);
                static void copy_raw(T*, T*, const T*, const T*);

                inline const c_array& operator = (const c_array&);
                inline const c_array& operator += (const c_array&);
                inline const c_array& operator += (const char*);

                unsigned size() const;
                inline T& operator [] (unsigned);
                inline const T& operator [] (unsigned) const;

                void map(std::function<void(T&)>);
                void map(std::function<void(const T&)>) const;
                const c_array& operator >> (std::ostream&) const;

                /* TODO */
                const c_array& operator << (std::istream&) const;
        };

        template<typename T>
        c_array<T>::c_array(const T* begin_arr, const T* end_arr)
        {
                init();
                alloc((end_arr - begin_arr) + 1);

                copy_raw(begin, end, begin_arr, end_arr);
        }


        template<typename T>
        const c_array<T>& c_array<T>::operator += (const char* str)
        {
                c_array<T> temp;

                req(str);
                std::istringstream in(str);

                // HERE <-


                return *this;
        }


        template<typename T>
        const c_array<T>& c_array<T>::operator += (const c_array<T>& other)
        {
                unsigned old_size, new_size;

                if (!other.size())
                        return *this;

                old_size = size();
                new_size = old_size + other.size();

                realloc(new_size);
                copy_raw(begin + old_size, end, other.bptr(), other.eptr());

                return *this;
        }


        template<typename T>
        const c_array<T>& c_array<T>::operator = (const c_array<T>& other)
        {
                if (begin != nullptr) {
                        req(end != nullptr);
                        delete[] begin;
                }

                init();
                alloc(other.size());

                copy_raw(begin, end, other.bptr(), other.eptr());

                return *this;
        }


        template<typename T>
        c_array<T>::c_array(const c_array<T>& other)
        {
                init();
                alloc(other.size());

                copy_raw(begin, end, other.bptr(), other.eptr());
        }


        template<typename T>
        c_array<T>::c_array(unsigned size) {
                init();
                alloc(size);
        }

        template<typename T>
        c_array<T>::c_array()
        {
                init();
        }

        template<typename T>
        void c_array<T>::init() {
                begin = nullptr;
                end = nullptr;
                safe = true;
        }

        template<typename T>
        T* c_array<T>::alloc(unsigned size) {
                req(begin == nullptr);
                req(end == nullptr);

                begin = new T[size];
                req(begin != nullptr);

                end = begin + (size - 1);

                return begin;
        }

        template<typename T>
        T* c_array<T>::realloc(unsigned size) {

                req(safe);

                if (begin == nullptr)
                        return alloc(size);

                req(end != nullptr);

                T* new_storage;

                new_storage = new T[size];
                req(new_storage != nullptr);

                copy_raw(new_storage, new_storage + (size - 1),
                         begin, end);

                delete[] begin;

                begin = new_storage;
                end = new_storage + (size - 1);

                return begin;
        }


        template<typename T>
        void c_array<T>::copy_raw(
                T* d_begin, T* d_end,
                const T* s_begin, const T* s_end
                )
        {
                req(d_begin != nullptr);
                req(d_end != nullptr);
                req(s_begin != nullptr);
                req(s_end != nullptr);

                unsigned d_size, s_size, size_min;

                /* Make sure `s` can fit into `d`. */
                d_size = (d_end - d_begin) + 1;
                s_size = (s_end - s_begin) + 1;

                size_min = min(d_size, s_size);

                for (unsigned i = 0; i < size_min; ++i)
                {
                        d_begin[i] = s_begin[i];
                }
        }



        template<typename T>
        void c_array<T>::map(std::function<void(T&)> action)
        {
                req(safe);

                if (begin == nullptr)
                        return;

                req(end != nullptr);

                for(unsigned i = 0; i < size(); ++i) {
                        action(*(begin + i));
                }
        }

        template<typename T>
        void c_array<T>::map(std::function<void(const T&)> action) const
        {
                req(safe);

                if (begin == nullptr)
                        return;

                req(end != nullptr);

                for(unsigned i = 0; i < size(); ++i) {
                        action(*(begin + i));
                }
        }


        template<typename T>
        const c_array<T>& c_array<T>::operator >> (std::ostream &out) const
        {
                req(safe);

                map([&](const T& elem) {
                                out << elem << " ";
                        } );

                out << '\n';

                return *this;
        }

        /* Meta */
        template<typename T>
        unsigned c_array<T>::size() const
        {
                req(safe);

                if (begin == nullptr)
                        return 0;

                return (end - begin) + 1;
        }


        /* Access. */
        template<typename T>
        T& c_array<T>::operator [] (unsigned i) {
                req(safe);

                req (i < size());
                return *(begin + i);
        }

        template<typename T>
        const T& c_array<T>::operator [] (unsigned i) const {
                req(safe);

                req (i < size());
                return *(begin + i);
        }


        template<typename T>
        c_array<T>::~c_array()
        {
                if (!safe)
                        print_red("[Warning] Unsafe delete.\n");
                if (begin)
                        delete[] begin;
        }
}


#endif
