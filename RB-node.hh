#ifndef __rb_node_hh
#define __rb_node_hh

#include <ostream>
#include <memory>
#include <cstring>

#include "req.hh"

/*
TODO: remove memset
   */

namespace iz {
	enum RB_color {
		RED,
		BLACK
	};

	using RB_color = enum RB_color;

	/* BEGIN - RB_Node - */
	template<typename T>
	struct RB_Node;

	template <typename T>
	using shared_rb_node = std::shared_ptr< RB_Node<T> >;

	template<typename T>
	inline std::ostream& operator<<(std::ostream&, const RB_Node<T>&);

	template<typename T>
	inline std::ostream& operator<<(std::ostream&, const shared_rb_node<T>&);


	template<typename T>
	struct RB_Node {
		T data;
		shared_rb_node <T> left;
		shared_rb_node <T> right;
		shared_rb_node <T> parent;
		RB_color color;

        /* Extra */
        unsigned size;
        T sum;

		RB_Node();

		/* Just data. */
		explicit RB_Node(const T&);

		explicit RB_Node(RB_color);

		/* Data and parent. */
		RB_Node(const T&, const shared_rb_node<T>&);

		/* Data, left and right children. */
		RB_Node(const T&, const shared_rb_node<T>&, const shared_rb_node<T>&);

		/* Data, left, right children and parent. */
		RB_Node(const T&, const shared_rb_node <T>&, const shared_rb_node<T>&, const shared_rb_node<T>&);

        /* mehh */
		RB_Node(const shared_rb_node <T>&, const shared_rb_node<T>&, const shared_rb_node<T>&);

		/* Reset left, right, parent to `nullptr`. */
		~RB_Node();

		/* Data access & print. */
		inline T& operator()();

		inline const T& operator()() const;

		inline std::ostream& print_data(std::ostream&, const char *) const;

		const RB_Node& operator = (const RB_Node&); // meh ...

		/* Print RB_Node/shared_rb_node. */
		friend std::ostream& operator<< <>(std::ostream&, const RB_Node<T>&);

		friend std::ostream& operator<< <>(std::ostream&, const shared_rb_node<T>&);

		/* Helper for `<<`. */
		static inline std::ostream& print_relative(std::ostream&, const shared_rb_node<T>&, const char *);


        /* TODO: Move outside */
        /* Helpers for augumenting. */
        void update_size() {
            unsigned new_size{};

            if (left != nullptr) {
                new_size += left->size;
            }
            if (right != nullptr) {
                new_size += right->size;
            }
            new_size += 1;

            size = new_size;
        }

        void update_sum() {
            T new_sum{};

            if (left != nullptr) {
                new_sum += left->sum;
            }
            if (right != nullptr) {
                new_sum += right->sum;
            }
            new_sum += data;

            sum = new_sum;
        }

        void update_metadata() {
            update_size();
            update_sum();
        }

	private:
		/* Used in `print_data` to avoid printing garbage values. */
		bool init;
	};

	template <typename T>
	const RB_Node<T>& RB_Node<T>::operator = (const RB_Node& other)
	{
		//req(other.init);

		init	= other.init;
		data	= other.data;
		color	= other.color;
		size	= other.size;
        sum     = other.sum;
		left	= other.left;
		right	= other.right;
		parent	= other.parent;

		return *this;
	}


        template<typename T>
        RB_Node<T>::RB_Node()
            :
            data    {},
		left	{ nullptr },
		right	{ nullptr },
		parent	{ nullptr },
		color	{ RED },
        size    { 0 },
		init	{ false }
	{ }

	template<typename T>
	RB_Node<T>::RB_Node(RB_color c)
		:
        data    {},
		left	{ nullptr },
		right	{ nullptr },
		parent	{ nullptr },
		color	{ c },
        size    { 0 },
		init	{ false }
	{ }

	template<typename T>
	RB_Node<T>::RB_Node(const T& d)
		:
		data	{ d },
		left	{ nullptr },
		right	{ nullptr },
		parent	{ nullptr },
		color	{ RED },
        size    { 1 },
        sum     { d },
		init	{ true }
	{ }

	template<typename T>
	RB_Node<T>::RB_Node(const T& d, const shared_rb_node<T>& p)
		:
		data	{ d },
		parent	{ p },
		left	{ nullptr },
		right	{ nullptr },
		color	{ RED },
        size    { 1 },
        sum     { d },
		init	{ true }
	{ }

	template<typename T>
	RB_Node<T>::RB_Node(const T& d, const shared_rb_node <T>& l, const shared_rb_node<T>& r)
		:
		data	{ d },
		left	{ l },
		right	{ r },
		parent	{ nullptr },
		color	{ RED },
        size    { 1 },
        sum     { d },
		init	{ true }
	{ }

	template<typename T>
	RB_Node<T>::RB_Node(const T& d, const shared_rb_node<T>& l, const shared_rb_node<T>& r, const shared_rb_node<T>& p)
		:
		data	{ d },
		left	{ l },
		right	{ r },
		parent	{ p },
		color	{ RED },
        size    { 1 },
        sum     { d },
		init	{ true }
	{ }


	template<typename T>
	RB_Node<T>::RB_Node(const shared_rb_node<T>& l, const shared_rb_node<T>& r, const shared_rb_node<T>& p)
		:
        data{},
		left{ l },
		right{ r },
		parent{ p },
		color{ RED },
		size{ 0 },
		init{ false }
	{ }

	template<typename T>
	RB_Node<T>::~RB_Node()
	{
		parent = left = right = nullptr;
	}

	template<typename T>
	std::ostream& operator << (std::ostream& out, const RB_Node<T>& n)
	{
		n.print_data(out, "Node");

		//RB_Node<T>::print_relative(out, n.parent, "Parent");
		//RB_Node<T>::print_relative(out, n.left, "Left");
		//RB_Node<T>::print_relative(out, n.right, "Right");

		return out;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const shared_rb_node<T>& n)
	{
		return (n != nullptr) ? out << *n : out << "Node: (null)\n";
	}


    /* HERE - does not belong here ... ostream << std::pair  ... */
    template <typename Key, typename Val>
    std::ostream& operator << (std::ostream& out, const std::pair<Key, Val>& data)
    {
        return out << data.first << " => " << data.second;
    }

	template<typename T>
	std::ostream& RB_Node<T>::print_data(std::ostream& out, const char *) const
	{
		//out << node_str << ": ";

		if (!init) {
			out << "(uninitialized)";
		}
		else {
			out << data << '{' << size << ", " << sum << '}' << "\t\t";
		}

		//out << "\t\t\t\t\t::";

		if (color == BLACK) {
			//out << "BLACK";
			out << "::B";
		}
		else {
			//out << "RED";
			out << "::R";
		}

		//return out << '\n';
		return out;
	}

	template<typename T>
	std::ostream&
		RB_Node<T>::print_relative(std::ostream& out, const shared_rb_node<T>& relative, const char *relative_str)
	{
		out << '\t';

		if (relative == nullptr)
			return out << relative_str << ": (null)" << '\n';

		(*relative).print_data(out, relative_str);

		return out;
	}

	template<typename T>
	T& RB_Node<T>::operator() ()
	{
		return data;
	}

	template<typename T>
	const T& RB_Node<T>::operator() () const
	{
		return data;
	}
	/* END - RB_Node - */

}

#endif
