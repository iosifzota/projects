#ifndef __Node_hh
#define __Node_hh

#include <ostream>
#include <memory>

namespace iz {
	/* BEGIN - Node - */
	template<typename T>
	struct Node;

	template <typename T>
	using shared_node = std::shared_ptr< Node<T> >;

	template<typename T>
	inline std::ostream& operator<<(std::ostream&, const Node<T>&);

	template<typename T>
	inline std::ostream& operator<<(std::ostream&, const shared_node<T>&);


	template<typename T>
	struct Node {
		T data;
		shared_node <T> left;
		shared_node <T> right;
		shared_node <T> parent;

		Node();

		/* Just data. */
		explicit Node(const T&);

		/* Data and parent. */
		Node(const T&, const shared_node<T>&);

		/* Data, left and right children. */
		Node(const T&, const shared_node<T>&, const shared_node<T>&);

		/* Data, left, right children and parent. */
		Node(const T&, const shared_node <T>&, const shared_node<T>&, const shared_node<T>&);

		/* Reset left, right, parent to `nullptr`. */
		~Node();

		/* Data access & print. */
		inline T& operator()();

		inline const T& operator()() const;

		inline std::ostream& print_data(std::ostream&, const char *) const;

		/* Print node/shared_node. */
		friend std::ostream& operator<< <>(std::ostream&, const Node<T>&);

		friend std::ostream& operator<< <>(std::ostream&, const shared_node<T>&);

		/* Helper for `<<`. */
		static inline std::ostream& print_relative(std::ostream&, const shared_node<T>&, const char *);

	private:
		/* Used in `print_data` to avoid printing garbage values. */
		bool init;
	};

	template<typename T>
	Node<T>::Node()
		: left{ nullptr }, right{ nullptr }, parent{ nullptr }, init{ false } {}

	template<typename T>
	Node<T>::Node(const T& d)
		: data{ d }, left{ nullptr }, right{ nullptr }, parent{ nullptr }, init{ true } {}

	template<typename T>
	Node<T>::Node(const T& d, const shared_node<T>& p)
		: data{ d }, parent{ p }, left{ nullptr }, right{ nullptr }, init{ true } {}

	template<typename T>
	Node<T>::Node(const T& d, const shared_node <T>& l, const shared_node<T>& r)
		: data{ d }, left{ l }, right{ r }, parent{ nullptr }, init{ true } {}

	template<typename T>
	Node<T>::Node(const T& d, const shared_node<T>& l, const shared_node<T>& r, const shared_node<T>& p)
		: data{ d }, left{ l }, right{ r }, parent{ p }, init{ true } {}


	template<typename T>
	Node<T>::~Node()
	{
		parent = left = right = nullptr;
	}

	template<typename T>
	std::ostream& operator << (std::ostream& out, const Node<T>& n)
	{

		n.print_data(out, "Node");

		Node<T>::print_relative(out, n.parent, "Parent");
		Node<T>::print_relative(out, n.left, "Left");
		Node<T>::print_relative(out, n.right, "Right");

		return out;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const shared_node<T>& n)
	{
		return (n != nullptr) ? out << *n : out << "Node: (null)\n";
	}

	template<typename T>
	std::ostream& Node<T>::print_data(std::ostream& out, const char *node_str) const
	{
		out << node_str << ": ";

		if (!init)
			return out << "(uninitialized)" << '\n';

		out << data << '\n';

		return out;
	}

	template<typename T>
	std::ostream&
		Node<T>::print_relative(std::ostream& out, const shared_node<T>& relative, const char *relative_str)
	{
		out << '\t';

		if (relative == nullptr)
			return out << relative_str << ": (null)" << '\n';

		(*relative).print_data(out, relative_str);

		return out;
	}

	template<typename T>
	T& Node<T>::operator() ()
	{
		return data;
	}

	template<typename T>
	const T& Node<T>::operator() () const
	{
		return data;
	}
	/* END - Node - */

}

#endif