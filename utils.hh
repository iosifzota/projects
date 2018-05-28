#ifndef __utils_hh
#define __utils_hh

#include <vector>
#include <stack>
#include <istream>
#include <ostream>
#include <functional>
#include <fstream>
#include <initializer_list>

#define nl std::cout << std::endl
#define nll nl; nl;
#define nlll nll; nl;
#define nllll nll; nll;
#define nn(n) for (int i = 0; i < n; ++i) std::cout << std::endl;

template<typename T>
T min(const T& x, const T& y) {
    return (x < y) ? x : y;
}

template<typename T>
T max(const T& x, const T& y) {
    return (x > y) ? x : y;
}


template<typename T, class Container = std::vector<T> >
void map(Container& ct, std::function<void(T&)> fn)
{
    for (auto& elem : ct) {
        fn(elem);
    }
}

template<typename T, class Container = std::vector<T> >
void map(const Container& ct, std::function<void(const T&)> fn)
{
    for (const auto& elem : ct) {
        fn(elem);
    }
}

template<typename T>
std::istream& operator >> (std::istream& in, std::vector<T>& v)
{
    T data;

    while (in >> data) {
        v.push_back(data);
    }

    return in;
}

template<typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& v)
{
    for (const auto& itr : v) {
        out << itr << ' ';
    }
    out << '\n';

    return out;
}

template<typename T>
void swap (std::vector<T>& v, unsigned i, unsigned j)
{
    T aux;
    aux = v[i];
    v[i] = v[j];
    v[j] = aux;
}

namespace iz {
	template <typename T>
	void dev_null(const T&) {
		;
	}

	template <typename T>
	void clear_stack(std::stack<T>& st)
	{
		while (!st.empty())
			st.pop();
	}

	template <typename T1, typename T2>
	struct pair {
		T1 first;
		T2 second;

		pair() {};

		pair(const T1& __first, const T2& __second)
			: first{ __first }, second{ __second } {}

 		pair(const pair& other) {
			first = other.first;
			second = other.second;
		}

		const pair& operator = (const pair& other) {
			first = other.first;
			second = other.second;
		}
	};

    template <typename T1, typename T2>
    std::ostream& operator << (std::ostream& out, const pair<T1, T2>& p) {
        return out << p.first << ", " << p.second;
    }
}



// template<typename T>
// void map(std::vector<T>& v, void (*fn)(T&))
// {
//     for (auto& itr : v) {
//         (*fn)(itr);
//     }
// }

// template<typename T>
// void map(const std::vector<T>& v, void (*fn)(T&))
// {
//     for (auto& itr : v) {
//         (*fn)(itr);
//     }
// }

// template<typename T>
// void map(const std::vector<T>& v, void (*fn)(const T&))
// {
//     for (const auto& itr : v) {
//         (*fn)(itr);
//     }
// }

#endif
