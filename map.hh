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
	struct rb_item;

	template <typename Key, typename Val>
	using shared_map_node = shared_rb_node< rb_item<Key, Val> >;

	template <typename Key, typename Val, typename Less>
	using rb_map_tree = rbtree < rb_item<Key, Val>, Less_key<Key, Val, Less> >;


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
			std::weak_ptr< RB_Node< rb_item<Key, Val> > > current;

		public:

			bool operator == (const base_iterator& other) const;
			bool operator != (const base_iterator& other) const;

			rb_item<Key, Val>& operator * () {
				shared_map_node<Key, Val> temp(static_cast<shared_map_node<Key, Val>>(current)); // current: weak_ptr

				req(temp != nullptr);
				req(temp != NIL);

				return temp->data;
			}


			base_iterator& find(const rb_item<Key, Val>& data, const shared_map_node<Key, Val>& root) {
				current = static_search(data, root);
				return *this;
			}
		};

	public:
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

		class reverse_iterator : public base_iterator
		{
		protected:
			using base_iterator::current;

		public:
			reverse_iterator(shared_map_node<Key, Val> begin_root = NIL);

			reverse_iterator& operator ++ ();
			reverse_iterator operator ++ (int);

			reverse_iterator& operator -- ();
			reverse_iterator operator -- (int);
		};

		forward_iterator find(const Key& key) const;

		forward_iterator begin() const {
			forward_iterator itr(root);
			return itr;
		}
		forward_iterator end() const {
			forward_iterator itr;
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

		/* Auguments. */
		using rb_map_tree<Key, Val, Less>::size;
		using rb_map_tree<Key, Val, Less>::empty;
		using rb_map_tree<Key, Val, Less>::clear;
	};

	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::forward_iterator
		map<Key, Val, Less>::find(const Key& key) const
	{
		Val garbage;
		dev_null<Val>(garbage);

		forward_iterator itr = begin();
		itr.find(rb_item<Key, Val>(key, garbage), root);

		return itr;
	}

	template <typename Key, typename Val, typename Less>
	Val& map<Key, Val, Less>::operator[] (const Key& key)
	{
		Val garbage;
		dev_null<Val>(garbage);

		return (this->insert_unique(rb_item<Key, Val>(key, garbage))).second;
	}

	template <typename Key, typename Val, typename Less = std::less<Key> >
	struct Less_key {
		static Less less_key;

		inline bool operator () (const rb_item<Key, Val>& lhs, const rb_item<Key, Val>& rhs) {
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
		return static_cast<shared_map_node<Key, Val>>(current) == static_cast<shared_map_node<Key, Val>>(other.current);
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
		req(!current.expired());

		current = successor(static_cast<shared_map_node<Key, Val>>(current));
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
		req(!current.expired());

		current = predecessor(static_cast<shared_map_node<Key, Val>>(current));
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
		 * Reverse iterator
		 */
	template <typename Key, typename Val, typename Less>
	map<Key, Val, Less>::reverse_iterator::reverse_iterator(shared_map_node<Key, Val> begin_root)
	{
		current = max(begin_root);
	}

	/* Increment operators. */
	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::reverse_iterator&
		map<Key, Val, Less>::reverse_iterator::operator ++ ()
	{
		req(!current.expired());

		current = predecessor(static_cast<shared_map_node<Key, Val>>(current));
		return *this;
	}

	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::reverse_iterator
		map<Key, Val, Less>::reverse_iterator::operator ++ (int)
	{
		reverse_iterator ret_val = *this;
		++(*this);
		return ret_val;
	}

	/* Decrement operators. */
	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::reverse_iterator&
		map<Key, Val, Less>::reverse_iterator::operator -- ()
	{
		req(!current.expired());

		current = successor(static_cast<shared_map_node<Key, Val>>(current));
		return *this;
	}

	template <typename Key, typename Val, typename Less>
	typename map<Key, Val, Less>::reverse_iterator
		map<Key, Val, Less>::reverse_iterator::operator -- (int)
	{
		reverse_iterator ret_val = *this;
		++(*this);
		return ret_val;
	}



	template <typename Key, typename Val>
	struct rb_item : public std::pair<const Key, Val>
	{
		using std::pair<const Key, Val>::first;
		using std::pair<const Key, Val>::second;

		rb_item() : std::pair<const Key, Val>() {}
		rb_item(const Key& key, const Val& val) : std::pair<const Key, Val>(key, val) {}

		const rb_item<Key, Val>& operator + (const rb_item<Key, Val>& rhs) {
			this->second = this->second + rhs.second;
			return *this;
		}

		const rb_item<Key, Val>& operator = (const rb_item<Key, Val>& rhs) {
			this->second = rhs.second;
			return *this;
		}

		const rb_item<Key, Val>& operator += (const rb_item<Key, Val>& rhs) {
			*this = *this + rhs;
			return *this;
		}

		const rb_item<Key, Val>& operator / (const rb_item<Key, Val>& rhs) {
			this->second = this->second / rhs.second;
			return *this;
		}

		const rb_item<Key, Val>& operator / (const Val& rhs) {
			this->second = this->second / rhs;
			return *this;
		}
	};

	template <typename Key, typename Val>
	std::ostream& operator << (std::ostream&out, const rb_item<Key, Val>& i)
	{
		return out << i.first << ", " << i.second;
	}
}

#endif // !__map_hh
