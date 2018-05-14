#ifndef __rb_node_hh
#define __rb_node_hh

#include <ostream>
#include <memory>

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

		/* Reset left, right, parent to `nullptr`. */
		~RB_Node();

		/* Data access & print. */
		inline T& operator()();

		inline const T& operator()() const;

		inline std::ostream& print_data(std::ostream&, const char *) const;

		/* Print RB_Node/shared_rb_node. */
		friend std::ostream& operator<< <>(std::ostream&, const RB_Node<T>&);

		friend std::ostream& operator<< <>(std::ostream&, const shared_rb_node<T>&);

		/* Helper for `<<`. */
		static inline std::ostream& print_relative(std::ostream&, const shared_rb_node<T>&, const char *);

	private:
		/* Used in `print_data` to avoid printing garbage values. */
		bool init;
	};

	template<typename T>
	RB_Node<T>::RB_Node()
		: 
		left	{ nullptr },
		right	{ nullptr },
		parent	{ nullptr },
		color	{ RED },
		init	{ false }
	{}

	template<typename T>
	RB_Node<T>::RB_Node(RB_color c)
		:
		left	{ nullptr },
		right	{ nullptr },
		parent	{ nullptr },
		color	{ c },
		init	{ false }
	{}

	template<typename T>
	RB_Node<T>::RB_Node(const T& d)
		:
		data	{ d },
		left	{ nullptr },
		right	{ nullptr },
		parent	{ nullptr },
		color	{ RED },
		init	{ true }
	{}

	template<typename T>
	RB_Node<T>::RB_Node(const T& d, const shared_rb_node<T>& p)
		:
		data	{ d },
		parent	{ p },
		left	{ nullptr },
		right	{ nullptr },
		color	{ RED },
		init	{ true }
	{}

	template<typename T>
	RB_Node<T>::RB_Node(const T& d, const shared_rb_node <T>& l, const shared_rb_node<T>& r)
		:
		data	{ d },
		left	{ l },
		right	{ r },
		parent	{ nullptr },
		color	{ RED },
		init	{ true }
	{}

	template<typename T>
	RB_Node<T>::RB_Node(const T& d, const shared_rb_node<T>& l, const shared_rb_node<T>& r, const shared_rb_node<T>& p)
		:
		data	{ d },
		left	{ l },
		right	{ r },
		parent	{ p },
		color	{ RED },
		init	{ true }
	{}


	template<typename T>
	RB_Node<T>::~RB_Node()
	{
		parent = left = right = nullptr;
	}

	template<typename T>
	std::ostream& operator << (std::ostream& out, const RB_Node<T>& n)
	{
		n.print_data(out, "Node");

		RB_Node<T>::print_relative(out, n.parent, "Parent");
		RB_Node<T>::print_relative(out, n.left, "Left");
		RB_Node<T>::print_relative(out, n.right, "Right");

		return out;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const shared_rb_node<T>& n)
	{
		return (n != nullptr) ? out << *n : out << "Node: (null)\n";
	}

	template<typename T>
	std::ostream& RB_Node<T>::print_data(std::ostream& out, const char *node_str) const
	{
		out << node_str << ": ";

		if (!init) {
			out << "(uninitialized)";
		}
		else {
			out << data << "\t\t";
		}

		out << "\t\t\t\t\t::";

		if (color == BLACK) {
			out << "BLACK";
		}
		else {
			out << "RED";
		}

		return out << '\n';
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
