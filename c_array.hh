#ifndef __c_array_hh
#define __c_array_hh

#include <memory>
#include <istream>
#include <ostream>
#include <initializer_list>

#include "req.hh"

template <typename T>
void new_req(T** ptr, unsigned blocks)
{
	req(ptr != nullptr);
	req((*ptr = new T[blocks]) != nullptr);
}

namespace iz {
	template <typename T, unsigned GROWTH>
	class c_array;

	template <typename T, unsigned GROWTH>
	std::istream& operator >> (std::istream&, c_array<T, GROWTH>&);

	template <typename T, unsigned GROWTH>
	std::ostream& operator << (std::ostream&, const c_array<T, GROWTH>&);

	template<typename T, unsigned GROWTH = 5>
	class c_array
	{
	private:
		T* begin_ptr;
		T* end_ptr;
		unsigned cap;

		/* Internal memory management */
		void init();
		T* alloc(unsigned);

		static const unsigned realloc_factor;

	public:
		c_array();
		explicit c_array(unsigned size);
		c_array(unsigned size, const T& fill_value);
		c_array(const T*, const T*);
		~c_array();

		/* Copy constuctors */
		c_array(const c_array&);
		c_array(const c_array*);

		c_array(std::initializer_list<T> l);

		/* Iterators */
		const T* begin() const {
			return begin_ptr;
		}
		const T* end() const {
			return end_ptr;
		}

		inline bool empty() const;
		inline unsigned size() const;
		inline unsigned capacity() const;
		
		/* Memory management */
		T* realloc(unsigned);
		inline unsigned calc_realloc_size() const;

		/* [begin_src, end_src) is copied in [begin_dest, end_dest).  */
		static void copy_raw(T* begin_dest, T* end_dest, const T* begin_src, const T* end_src);

		/* Delete lhs storage, get new_storage and copy_raw from rhs.*/
		inline const c_array& operator = (const c_array&);
		inline const c_array& operator = (const c_array*);

		/* Concatenation */
		inline const c_array& operator += (const c_array&);
		inline const c_array& operator += (const c_array*);

		
		inline void push_back(const T&);

		/* Access & modify data. (i < capacity)*/
		inline T& operator [] (unsigned);

		/* Read data. (i < size) */
		inline const T& operator [] (unsigned) const;

		/* I/O */
		friend std::istream& operator >> <>(std::istream&, c_array<T, GROWTH>&);
		friend std::ostream& operator << <>(std::ostream&, const c_array<T, GROWTH>&);
	};

	template <typename T, unsigned GROWTH>
	const unsigned c_array<T, GROWTH>::realloc_factor = 2;

	template <typename T, unsigned GROWTH>
	unsigned c_array<T, GROWTH>::calc_realloc_size() const
	{
		return capacity() + realloc_factor * (GROWTH + 1);
	}

	template <typename T, unsigned GROWTH>
	void c_array<T, GROWTH>::push_back(const T& value)
	{
		if (size() >= capacity()) {
			realloc(calc_realloc_size());
		}

		*end_ptr = value;
		++end_ptr;
	}

	template<typename T, unsigned GROWTH>
	T* c_array<T, GROWTH>::alloc(unsigned blocks)
	{
		req(begin_ptr == nullptr);
		req(end_ptr == nullptr);

		new_req(&begin_ptr, blocks);

		end_ptr = begin_ptr;
		cap = blocks;

		return begin_ptr;
	}

	template<typename T, unsigned GROWTH>
	T* c_array<T, GROWTH>::realloc(unsigned blocks)
	{
		T* new_storage;
		unsigned old_size;

		if (begin_ptr == nullptr) {
			return alloc(blocks);
		}
		req(end_ptr != nullptr);

		new_req(&new_storage, blocks);
		copy_raw(
			new_storage, new_storage + blocks,
			begin_ptr, end_ptr
		);

		old_size = size();
		delete[] begin_ptr;

		begin_ptr = new_storage;
		end_ptr = new_storage + std::min(old_size, blocks);

		cap = blocks;

		return begin_ptr;
	}

	template<typename T, unsigned GROWTH>
	void c_array<T, GROWTH>::copy_raw(        // end is the ptr, after the last block
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
	}

	/* Meta */
	template<typename T, unsigned GROWTH>
	unsigned c_array<T, GROWTH>::size() const
	{
		if (empty()) {
			return 0;
		}
		return end_ptr - begin_ptr;
	}

	template<typename T, unsigned GROWTH>
	unsigned c_array<T, GROWTH>::capacity() const
	{
		return cap;
	}

	template<typename T, unsigned GROWTH>
	bool c_array<T, GROWTH>::empty() const
	{
		if (begin_ptr == nullptr) {
			req(end_ptr == nullptr);
		}
		return begin_ptr == end_ptr;
	}

	/* Access. */
	template<typename T, unsigned GROWTH>
	T& c_array<T, GROWTH>::operator [] (unsigned i)
	{
		req(i < capacity());
		if (i >= size()) {
			end_ptr = begin_ptr + i + 1;
		}
		return begin_ptr[i];
	}

	template<typename T, unsigned GROWTH>
	const T& c_array<T, GROWTH>::operator [] (unsigned i) const
	{
		req(i < size());
		return begin_ptr[i];
	}


	template <typename T, unsigned GROWTH>
	const c_array<T, GROWTH>& c_array<T, GROWTH>::operator += (const c_array<T, GROWTH>& other)
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
	const c_array<T, GROWTH>& c_array<T, GROWTH>::operator += (const c_array<T, GROWTH>* other)
	{
		return *this += *other;
	}

	template <typename T, unsigned GROWTH>
	const c_array<T, GROWTH>& c_array<T, GROWTH>::operator = (const c_array<T, GROWTH>& other)
	{
		if (begin_ptr != nullptr) {
			req(end_ptr != nullptr);
			delete[] begin_ptr;
		}

		init();

		if (other.empty()) {
			return *this;
		}

		alloc(other.size());
		copy_raw(begin_ptr, (end_ptr = begin_ptr + capacity()), other.begin(), other.end());

		return *this;
	}

	template<typename T, unsigned GROWTH>
	const c_array<T, GROWTH>& c_array<T, GROWTH>::operator = (const c_array<T, GROWTH>* other)
	{
		return *this = *other;
	}

	template<typename T, unsigned GROWTH>
	void c_array<T, GROWTH>::init() {
		begin_ptr = end_ptr = nullptr;
		cap = 0;
	}

	template<typename T, unsigned GROWTH>
	c_array<T, GROWTH>::c_array(const T* begin_arr, const T* end_arr) // end_arr is the ptr, after the last block
	{
		req(begin_arr != nullptr);
		req(end_arr != nullptr);
		req(begin_arr < end_arr);

		init();
		alloc(end_arr - begin_arr);

		copy_raw(begin_ptr, (end_ptr = begin_ptr + capacity()), begin_arr, end_arr);
	}

	template<typename T, unsigned GROWTH>
	c_array<T, GROWTH>::c_array(const c_array<T, GROWTH>& other)
	{
		init();
		*this = other;
	}

	template <typename T, unsigned GROWTH>
	c_array<T, GROWTH>::c_array(const c_array<T, GROWTH>* other)
	{
		init();
		*this = *other;
	}


	template<typename T, unsigned GROWTH>
	c_array<T, GROWTH>::c_array(unsigned size, const T& fill_value)
	{
		init();
		alloc(size);

		for (unsigned i = 0; i < size; ++i) {
			push_back(fill_value);
		}
	}

	template<typename T, unsigned GROWTH>
	c_array<T, GROWTH>::c_array(unsigned size)
	{
		init();
		alloc(size);
	}

	template<typename T, unsigned GROWTH>
	c_array<T, GROWTH>::c_array()
	{
		init();
	}

	template<typename T, unsigned GROWTH>
	c_array<T, GROWTH>::c_array(std::initializer_list<T> l)
	{
		init();

		for (auto itr : l) {
			push_back(itr);
		}
	}

	template<typename T, unsigned GROWTH>
	c_array<T, GROWTH>::~c_array()
	{
		if (begin_ptr) {
			delete[] begin_ptr;
		}
	}

	template <typename T, unsigned GROWTH>
	std::istream& operator >> (std::istream& in, c_array<T, GROWTH>& arr)
	{
		T temp;

		while (in >> temp) {
			arr.push_back(temp);
		}
		return in;
	}


	template <typename T, unsigned GROWTH>
	std::ostream& operator << (std::ostream& out, const c_array<T, GROWTH>& arr)
	{
		for (auto itr : arr) {
			out << itr << ' ';
		}
		return out << '\n';
	}
}


#endif
