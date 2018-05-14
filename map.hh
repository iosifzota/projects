#ifndef __map_hh
#define __map_hh

/*
*/

#include <utility>
#include <functional>
#include <type_traits>

#include "rbtree.hh"
#include "RB-node.hh"
#include "generic.hh"
#include "utils.hh"

namespace iz {

	template <typename Key, typename Val, typename Less>
	class map;

	template <typename Key, typename Val, typename Less>
	struct Less_key;

	template <typename Key, typename Val>
	using shared_map_node = shared_rb_node< std::pair<Key, Val> >;

	template <typename Key, typename Val, typename Less>
	using rb_map_tree = rbtree < std::pair<Key, Val>, Less_key<Key, Val, Less> >;


	template <typename Key, typename Val, typename Less = std::less<Key> >
	class map : protected rb_map_tree <Key, Val, Less>
	{
	private:
		using rb_map_tree <Key, Val, Less>::root;
		using rb_map_tree <Key, Val, Less>::NIL;

		using rb_map_tree <Key, Val, Less>::min;
		using rb_map_tree <Key, Val, Less>::max;
		using rb_map_tree <Key, Val, Less>::successor;
		using rb_map_tree <Key, Val, Less>::predecessor;
		using rb_map_tree <Key, Val, Less>::static_search;

		using rb_map_tree<Key, Val, Less>::less;
		using rb_map_tree<Key, Val, Less>::greater;
		using rb_map_tree<Key, Val, Less>::equal;
		using rb_map_tree<Key, Val, Less>::not_equal;

		class base_iterator
		{
		protected:
			shared_map_node<Key, Val> current;

		public:

			bool operator == (const base_iterator& other) const;
			bool operator != (const base_iterator& other) const;
		};

		class forward_iterator : public base_iterator
		{
		protected:
			using base_iterator::current;

		public:
			forward_iterator(shared_map_node<Key, Val> begin_root = NIL);

			forward_iterator& operator ++ ();
			forward_iterator operator ++ (int);

			forward_iterator& operator -- ();
			forward_iterator operator -- (int);
		};

		class backward_iterator : public base_iterator
		{
		protected:
			using base_iterator::current;

		public:
			backward_iterator(shared_map_node<Key, Val> begin_root = NIL);

			backward_iterator& operator ++ ();
			backward_iterator operator ++ (int);

			backward_iterator& operator -- ();
			backward_iterator operator -- (int);
		};

	public:
		class const_iterator : public forward_iterator
		{
		private:
			using forward_iterator::current;

		public:
			const_iterator(shared_map_node<Key, Val> begin = NIL) : forward_iterator(begin) {}

			std::pair<const Key&, const Val&> operator * ();
			const_iterator& find(const std::pair<Key, Val>& data, const shared_map_node<Key, Val>& root) {
				current = static_search(data, root);
				return *this;
			}
		};

		class iterator : public forward_iterator
		{
		private:
			using forward_iterator::current;

		public:
			iterator(shared_map_node<Key, Val> begin = NIL) : forward_iterator(begin) {}

			std::pair<const Key, Val&> operator * ();
			iterator& find(const std::pair<Key, Val>& data, const shared_map_node<Key, Val>& root) {
				current = static_search(data, root);
				return *this;
			}
		};

		class const_reverse_iterator : public backward_iterator
		{
		private:
			using backward_iterator::current;

		public:
			const_reverse_iterator(shared_map_node<Key, Val> begin = NIL) : backward_iterator(begin) {}

			std::pair<const Key&, const Val&> operator * ();
			const_reverse_iterator& find(const std::pair<Key, Val>& data, const shared_map_node<Key, Val>& root) {
				current = static_search(data, root);
				return *this;
			}
		};

		class reverse_iterator : public backward_iterator
		{
		private:
			using backward_iterator::current;

		public:
			reverse_iterator(shared_map_node<Key, Val> begin = NIL) : backward_iterator(begin) {}

			std::pair<const Key, Val&> operator * ();
			reverse_iterator& find(const std::pair<Key, Val>& data, const shared_map_node<Key, Val>& root) {
				current = static_search(data, root);
				return *this;
			}
		};

		iterator find(const Key& key) const;

		iterator begin() const {
			iterator itr(root);
			return itr;
		}
		iterator end() const {
			iterator itr;
			return itr;
		}

		reverse_iterator rbegin() const {
			reverse_iterator itr(root);
			return itr;
		}
		reverse_iterator rend() const {
			reverse_iterator itr;
			return itr;
		}

		inline Val& operator [] (const Key& key);
	};

	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::iterator
		map<Key, Val, Less>::find(const Key& key) const
	{
		Val garbage;
		dev_null<Val>(garbage);

		return begin().find(std::make_pair(key, garbage), root);
	}

	template <typename Key, typename Val, typename Less>
	Val& map<Key, Val, Less>::operator[] (const Key& key)
	{
		Val garbage;
		dev_null<Val>(garbage);

		return ( this->insert_unique(std::make_pair(key, garbage)) ).second;
	}

	template <typename Key, typename Val, typename Less = std::less<Key> >
	struct Less_key {
		static Less less_key;

		inline bool operator () (const std::pair<Key, Val>& lhs, const std::pair<Key, Val>& rhs) {
			return less_key(lhs.first, rhs.first);
		}
	};

	/* Initialize static member. */
	template <typename Key, typename Val, typename Less>
	Less Less_key<Key, Val, Less>::less_key;

	/*
	 * base_iterator
	 */
	template <typename Key, typename Val, typename Less>
	bool map<Key, Val, Less>::base_iterator::operator == (const base_iterator& other) const {
		return current == other.current;
	}

	template <typename Key, typename Val, typename Less>
	bool map<Key, Val, Less>::base_iterator::operator != (const base_iterator& other) const {
		return !(*this == other);
	}



	/*
	 * Forward iterator
	 */
	template <typename Key, typename Val, typename Less>
	map<Key, Val, Less>::forward_iterator::forward_iterator(shared_map_node<Key, Val> begin_root)
	{
		current = min(begin_root);
	}

	/* Increment operators. */
	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::forward_iterator&
		map<Key, Val, Less>::forward_iterator::operator ++ ()
	{
		current = successor(current);
		return *this;
	}

	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::forward_iterator
		map<Key, Val, Less>::forward_iterator::operator ++ (int)
	{
		forward_iterator ret_val = *this;
		++(*this);
		return ret_val;
	}

	/* Decrement operators. */
	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::forward_iterator&
		map<Key, Val, Less>::forward_iterator::operator -- ()
	{
		current = predecessor(current);
		return *this;
	}

	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::forward_iterator
		map<Key, Val, Less>::forward_iterator::operator -- (int)
	{
		forward_iterator ret_val = *this;
		++(*this);
		return ret_val;
	}


	/*
	 * Const iterator (forward)
	 */
	template <typename Key, typename Val, typename Less>
	std::pair<const Key&, const Val&> map<Key, Val, Less>::const_iterator::operator * () {
		req(current != nullptr);
		req(current != NIL);

		return current->data;
	}

	/*
	 * Iterator (forward)
	 */
	template <typename Key, typename Val, typename Less>
	std::pair<const Key, Val&> map<Key, Val, Less>::iterator::operator * () {
		req(current != nullptr);
		req(current != NIL);

		std::pair<const Key&, Val&> temp({ current->data.first, current->data.second });

		return temp;
	}




	/*
	* Backward iterator
	*/
	template <typename Key, typename Val, typename Less>
	map<Key, Val, Less>::backward_iterator::backward_iterator(shared_map_node<Key, Val> begin_root)
	{
		current = max(begin_root);
	}

	/* Increment operators. */
	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::backward_iterator&
		map<Key, Val, Less>::backward_iterator::operator ++ ()
	{
		current = predecessor(current);
		return *this;
	}

	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::backward_iterator
		map<Key, Val, Less>::backward_iterator::operator ++ (int)
	{
		backward_iterator ret_val = *this;
		++(*this);
		return ret_val;
	}

	/* Decrement operators. */
	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::backward_iterator&
		map<Key, Val, Less>::backward_iterator::operator -- ()
	{
		current = successor(current);
		return *this;
	}

	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::backward_iterator
		map<Key, Val, Less>::backward_iterator::operator -- (int)
	{
		backward_iterator ret_val = *this;
		++(*this);
		return ret_val;
	}


	/*
	* Const iterator (backward)
	*/
	template <typename Key, typename Val, typename Less>
	std::pair<const Key&, const Val&> map<Key, Val, Less>::const_reverse_iterator::operator * () {
		req(current != nullptr);
		req(current != NIL);

		return current->data;
	}

	/*
	* Iterator (backward)
	*/
	template <typename Key, typename Val, typename Less>
	std::pair<const Key, Val&> map<Key, Val, Less>::reverse_iterator::operator * () {
		req(current != nullptr);
		req(current != NIL);

		std::pair<const Key&, Val&> temp({ current->data.first, current->data.second });

		return temp;
	}

}

#endif // !__map_hh
