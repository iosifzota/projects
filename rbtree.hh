#ifndef __rbtree_hh
#define __rbtree_hh

#include <iostream>

#include "basic_btree.hh"
#include "RB-node.hh"
#include "req.hh"

namespace iz {
	template <typename T, typename Less = std::less<T> >
	class rbtree : public basic_btree<T, RB_Node<T>, Less >
	{
	protected:
		using basic_btree<T, RB_Node<T>, Less>::root;
		using basic_btree<T, RB_Node<T>, Less>::NIL;

		using basic_btree<T, RB_Node<T>, Less>::less;
		using basic_btree<T, RB_Node<T>, Less>::greater;
		using basic_btree<T, RB_Node<T>, Less>::equal;
		using basic_btree<T, RB_Node<T>, Less>::not_equal;

		using basic_btree<T, RB_Node<T>, Less>::left_rotate;
		using basic_btree<T, RB_Node<T>, Less>::right_rotate;
		using basic_btree<T, RB_Node<T>, Less>::transplant;

		using basic_btree<T, RB_Node<T>, Less>::min;
		using basic_btree<T, RB_Node<T>, Less>::max;

		using basic_btree<T, RB_Node<T>, Less>::successor;
		using basic_btree<T, RB_Node<T>, Less>::predecessor;



	private:
		void setup_sentinel(const shared_rb_node<T>& node) {
			req(node != nullptr);
			req(node == NIL);

			node->parent = node->left = node->right = nullptr;
			node->color = BLACK;
		}
		void insert_fixup(shared_rb_node<T>);
		void extract_fixup(shared_rb_node<T>);

	public:
		rbtree() : basic_btree<T, RB_Node<T>, Less>::basic_btree() {
			setup_sentinel(NIL);
		}
		~rbtree() {
			setup_sentinel(NIL);
		}

		void temp() {
			std::cout << root;
		}

        unsigned size() const;

        static void rotate_metadata_fix(const shared_rb_node<T>&, const shared_rb_node<T>&);
        static void upwards_branch_metadata_refresh(shared_rb_node<T>);

		void construct(T*, T*);

		T& insert(const T&);
		T& insert_unique(const T&);
		shared_rb_node<T> extract(shared_rb_node<T>);
		shared_rb_node<T> extract(const T&);

        /* Auguments. */
		using basic_btree<T, RB_Node<T>, Less>::empty;
		using basic_btree<T, RB_Node<T>, Less>::clear;

		/* */
		using basic_btree<T, RB_Node<T>, Less>::preorder_map;
		using basic_btree<T, RB_Node<T>, Less>::operator=;
	};

	template <typename T, typename Less>
	void rbtree<T, Less>::construct(T* begin, T* end)
	{
		req(begin != nullptr);
		req(end != nullptr);
		req(begin <= end);

		for (T* itr = begin; itr != end; ++itr) {
			insert(*itr);
		}
	}


    /* Called only by =insert_unique()=. */
    template <typename T>
    static void undo_size_inc(shared_rb_node<T> node, const shared_rb_node<T>& NIL)
    {
        req(node != nullptr);

        for (; node != NIL; node = node->parent) {
            req(node->size != 0, "[Debug]");
            node->size -= 1;
        }
    }

	template <typename T, typename Less>
	T& rbtree<T, Less>::insert_unique(const T& val)
	{
		shared_rb_node<T> new_node(nullptr), current_node(root), current_node_parent(NIL);

		req(root != nullptr);

		/* --> NIL. */
		while (current_node != NIL && not_equal(current_node->data, val)) {
            current_node->size += 1;

			current_node_parent = current_node;

			if (less(val, current_node->data))
				current_node = current_node->left;
			else
				current_node = current_node->right;
		}

		/* Don't insert key twice. */
		if (current_node != NIL) {
            undo_size_inc(current_node_parent, NIL);

			return current_node->data;
		}

		new_node = std::make_shared< RB_Node<T> >(val, NIL, NIL);

		/* =new_node= -> parent */
		new_node->parent = current_node_parent;

		/* parent -> =new_node= */
		if (current_node_parent == NIL) {
			root = new_node;
		}
		else if (less(new_node->data, new_node->parent->data)) {
			new_node->parent->left = new_node;
		}
		else {
			new_node->parent->right = new_node;
		}

		insert_fixup(new_node);

		return new_node->data;
	}


	template <typename T, typename Less>
	T& rbtree<T, Less>::insert(const T& val)
	{
		shared_rb_node<T> new_node, current_node, current_node_parent;

		new_node = std::make_shared< RB_Node<T> >(val, NIL, NIL);

		current_node = root;
		current_node_parent = NIL;

		/* Follow crumbs to NIL. */
		while (current_node != NIL) {
            current_node->size += 1;

			current_node_parent = current_node;

			if (less(new_node->data, current_node->data))
				current_node = current_node->left;
			else
				current_node = current_node->right;
		}

		/* =new_node= -> parent */
		new_node->parent = current_node_parent;

		/* parent -> =new_node= */
		if (current_node_parent == NIL) {
			root = new_node;
		}
		else if (less(new_node->data, new_node->parent->data)) {
			new_node->parent->left = new_node;
		}
		else {
			new_node->parent->right = new_node;
		}

		insert_fixup(new_node);

		return new_node->data;
	}

	template <typename T, typename Less>
	void rbtree<T, Less>::insert_fixup(shared_rb_node<T> inserted)
	{
		req(inserted != NIL);
		req(inserted != nullptr);

		shared_rb_node<T> uncle, grandpa;

		while (inserted->parent->color == RED) {

			if (inserted->parent == inserted->parent->parent->left) {
				grandpa = inserted->parent->parent;
				uncle = grandpa->right;

				/* Steal grandpa's color. */
				if (uncle->color == RED) {
					grandpa->color = RED;

					inserted->parent->color = BLACK;
					uncle->color = BLACK;

					/* Is grandpa tucked in? */
					inserted = grandpa;
					continue;
				}
				/* Red in line. */
				else if (inserted == inserted->parent->right) {
					inserted = inserted->parent;
					left_rotate(inserted, rotate_metadata_fix);
				}

				/* Slurp one red to the other left. */
				inserted->parent->color = BLACK;
				grandpa->color = RED;

				right_rotate(inserted->parent->parent, rotate_metadata_fix);
			}
			else {
				grandpa = inserted->parent->parent;
				uncle = grandpa->left;

				/* Steal grandpa's color. */
				if (uncle->color == RED) {
					grandpa->color = RED;

					inserted->parent->color = BLACK;
					uncle->color = BLACK;

					/* Is grandpa tucked in? */
					inserted = grandpa;
					continue;
				}
				/* Red in line. */
				else if (inserted == inserted->parent->left) {
					inserted = inserted->parent;
					right_rotate(inserted, rotate_metadata_fix);
				}

				/* Slurp one red to the other left. */
				inserted->parent->color = BLACK;
				grandpa->color = RED;

				left_rotate(grandpa, rotate_metadata_fix);
			}
		}
		root->color = BLACK;
	}

	template <typename T, typename Less>
	shared_rb_node<T> rbtree<T, Less>::extract(const T& data)
    {
        extract(search(data));
    }

	template <typename T, typename Less>
	shared_rb_node<T> rbtree<T, Less>::extract(shared_rb_node<T> extracted)
	{
		shared_rb_node<T> barfed, uplifted;
		RB_color barfed_original_color;

		if (extracted == nullptr) {
			return nullptr;
		}

		req(extracted != NIL);

		barfed = extracted;
		barfed_original_color = extracted->color;

		if (extracted->left == NIL) {
			uplifted = extracted->right;

			transplant(extracted, uplifted);
            upwards_branch_metadata_refresh(uplifted->parent);
		}
		else if (extracted->right == NIL) {
			uplifted = extracted->left;

			transplant(extracted, uplifted);
            upwards_branch_metadata_refresh(uplifted->parent);
		}
		else {
			req((barfed = successor(extracted)) != NIL);
			barfed_original_color = barfed->color;

			uplifted = barfed->right;

			if (barfed->parent == extracted) {
				uplifted->parent = barfed;  // The crumbs (!only in case =uplifted= is NIL).
			}
			else {
				transplant(barfed, barfed->right);

				barfed->right = extracted->right;
				extracted->right->parent = barfed;
			}

			transplant(extracted, barfed);

			barfed->left = extracted->left;
			extracted->left->parent = barfed;

			/* Monkey move.  */
			barfed->color = extracted->color;

            upwards_branch_metadata_refresh(uplifted->parent);
		}

		if (barfed_original_color == BLACK) {
			extract_fixup(uplifted);
		}

		return extracted;
	}

	template <typename T, typename Less>
	void rbtree<T, Less>::extract_fixup(shared_rb_node<T> uplifted)
	{
		shared_rb_node<T> bro;
		while (uplifted != root && uplifted->color == BLACK) {

			if (uplifted == uplifted->parent->left) {
				bro = uplifted->parent->right;

				if (bro->color == RED) {
					bro->color = BLACK;
					uplifted->parent->color = RED;

					left_rotate(uplifted->parent, rotate_metadata_fix);

					bro = uplifted->parent->right;
					continue;
				}
				else if (bro->left->color == BLACK && bro->right->color == BLACK) {
					bro->color = RED;
					uplifted = uplifted->parent;
					continue;
				}
				else if (bro->right->color == BLACK) {
					bro->left->color = BLACK;
					bro->color = RED;

					right_rotate(bro, rotate_metadata_fix);

					bro = uplifted->parent->right;
				}

				bro->color = uplifted->parent->color;
				uplifted->parent->color = BLACK;

				left_rotate(uplifted->parent, rotate_metadata_fix);

				uplifted = root;
			}
			else {
				bro = uplifted->parent->left;

				if (bro->color == RED) {
					bro->color = BLACK;
					uplifted->parent->color = RED;

					right_rotate(uplifted->parent, rotate_metadata_fix);

					bro = uplifted->parent->left;
					continue;
				}
				else if (bro->right->color == BLACK && bro->left->color == BLACK) {
					bro->color = RED;
					uplifted = uplifted->parent;
					continue;
				}
				else if (bro->left->color == BLACK) {
					bro->right->color = BLACK;
					bro->color = RED;

					left_rotate(bro, rotate_metadata_fix);

					bro = uplifted->parent->left;
				}

				bro->color = uplifted->parent->color;
				uplifted->parent->color = BLACK;

				right_rotate(uplifted->parent, rotate_metadata_fix);

				uplifted = root;
			}
		}
	}

	template <typename T, typename Less>
    void rbtree<T, Less>::rotate_metadata_fix(
            const shared_rb_node<T>& downlifted,
            const shared_rb_node<T>& uplifted)
    {
        uplifted->size = downlifted->size;

        downlifted->size = downlifted->left->size + downlifted->right->size + 1;
    }

	template <typename T, typename Less>
    void rbtree<T, Less>::upwards_branch_metadata_refresh(shared_rb_node<T> node)
    {
        req(node != nullptr);

        /* 'til root refresh metadata. */
        for (; node != NIL; node = node->parent)
            node->size = node->left->size + node->right->size + 1;
    }

	template <typename T, typename Less>
    unsigned rbtree<T, Less>::size() const
    {
        req(root != nullptr, "[Debug]");

        return root->size;
    }
}

#endif
