#ifndef __vec_hh
#define __vec_hh

#include "utils.hh"
#include "req.hh"
#include "generic.hh"

#include <iostream>

namespace iz {
        generic(T)
        struct neutral_element;

        generic(T, E = neutral_element<T>)
        class vector;

        generic(T, E)
        std::ostream& operator << (std::ostream&, const vector<T, E>&);

        generic(T, E)
        class vector
        {
        private:
                T *first, *current, *last;

        public:
                void init_ptrs() { first = current = last = 0; };
                vector() { init_ptrs(); };
                explicit vector(unsigned size) { init_ptrs(); resize(size, true); };

                ~vector() { if (first) delete[] first; init_ptrs(); }

                void resize(unsigned, bool neutralize = false);
                void init_data(T*);

                unsigned capacity() const;
                unsigned size() const;

                void push_back(const T&);

                inline T& operator[] (unsigned);
                inline const T& operator[] (unsigned) const;

                friend std::ostream& operator << <T, E>(std::ostream&, const vector<T, E>&);

                static void duplicate(T *dest, const T *src, unsigned nelems);
        };

        generic(T, E)
        void vector<T, E>::push_back(const T& val)
        {
                req(first < last, "p_back() [Debug]");
                req(current < last, "p_back() [Debug]");

                *current++ = 12;

                if (current == last) {
                        resize(2 * size() + 1);
                }
        }

        generic(T, E)
        void vector<T, E>::resize(unsigned new_size, bool neutralize)
        {
                T *new_storage;
                unsigned old_size;

                /* Reset. */
                if (!new_size) {
                        if (first) {
                                delete[] first;
                        }
                        init_ptrs();
                        return;
                }

                /* TODO: Catch allocation throw. */
                new_storage = new T[new_size];
                old_size = size();

#define print_arr(arr_ptr, nelems)                                      \
                for (unsigned i = 0, size = nelems; i < size; ++i)      \
                        std::cout << arr_ptr[i] << ' ';                 \
                std::cout << '\n';

                /* Transfer data and free old storage. */
                if (first) {
                        std::cout << "Old storage: ";
                        print_arr(first, old_size);

                        std::cout << "Before: ";
                        print_arr(new_storage, new_size);

                        duplicate(new_storage, first, min(new_size, old_size));

                        std::cout << "After: ";
                        print_arr(new_storage, new_size);
                        std::cout << '\n';
                        delete[] first;
                }

                /* Update storage. */
                first = new_storage;
                last = first + new_size;

                req(capacity() == new_size, "up_storage(): Cap not checking out.");

                /* If required assin new storage the =neutral_element=. */
                if (neutralize && old_size < new_size) {
                        std::cout << *(first + old_size);
                        init_data(first + old_size);
                }
                else if (neutralize) {
                        init_data(first);
                }
                else if (old_size <= new_size) {
                        current = first + (old_size - 1);
                }
                else {
                        current = first + (new_size - 1);
                }
        }

        generic(T, E)
        void vector<T, E>::duplicate(T *dest, const T *src, unsigned nelems)
        {
                req(dest, "dup(): Null dest.");
                req(src, "dup(): Null src.");

                for (unsigned i = 0; i < nelems; ++i) {
                        *dest++ = *(src + i);
                }
        }

        generic(T, E)
        void vector<T, E>::init_data(T *begin)
        {
                if (!first) {
                        return;
                }
                req(begin, "init_data(): Null begin.");
                req(begin < last, "init_data() [Debug]");

                E neutral;
                for (current = begin; (current + 1) != last; ++current) {
                        *current = neutral();
                }
                *current = 0;
        }

        generic(T, E)
        unsigned vector<T, E>::capacity() const
        {
                req(last >= first, "capacity() [Debug]");
                return last - first;
        }

        generic(T, E)
        unsigned vector<T, E>::size() const
        {
                if (!current) {
                        return 0;
                }
                req(current >= first, "size(): Dangling current pointer. [Debug]");
                return current - first + 1;
        }

        generic(T, E)
        std::ostream& operator << (std::ostream& out, const vector<T, E>& v)
        {
                if (!v.first) {
                        out << "(empty)\n";
                        return out;
                }

                req(v.first < v.last, "operator<< [Debug]");

                /* TODO: with iterators. */
                for (unsigned i = 0; (v.first + i) != v.current; ++i) {
                        out << *(v.first + i) << ' ';
                }
                out << *(v.current) << '\n';

                return out;
        }

        generic(T, E)
        T& vector<T, E>::operator[] (unsigned i)
        {
                nll; std::cout << "cap: " << capacity(); nl;


                req(i < capacity(), "Tring to access vector out of bounds.");

                if (!current || i >= size()) {
                        current = first + i;
                }

                return *(first + i);
        }

        generic(T, E)
        const T& vector<T, E>::operator[] (unsigned i) const
        {
                nll; std::cout << "cap: " << capacity(); nl;

                req(i < capacity(), "Tring to read(const) vector out of bounds.");
                return *(first + i);
        }


        template<>
        struct neutral_element<int>
        {
                int operator ()() {
                        return 0;
                }
        };
        template<>
        struct neutral_element<unsigned>
        {
                unsigned operator ()() {
                        return 0;
                }
        };
        template<>
        struct neutral_element<long>
        {
                long operator ()() {
                        return 0;
                }
        };
        template<>
        struct neutral_element<unsigned long>
        {
                unsigned long operator ()() {
                        return 0;
                }
        };
        template<>
        struct neutral_element<long long>
        {
                long long operator ()() {
                        return 0;
                }
        };
        template<>
        struct neutral_element<unsigned long long>
        {
                unsigned long long operator ()() {
                        return 0;
                }
        };
        template<>
        struct neutral_element<float>
        {
                float operator ()() {
                        return 0;
                }
        };
        template<>
        struct neutral_element<double>
        {
                double operator ()() {
                        return 0;
                }
        };
}

#endif
