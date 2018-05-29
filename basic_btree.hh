#ifndef __tree_hh
#define __tree_hh

// DONE: postorder
// DONE: print_tree(enum print_tree_option { ERR, preorder, inorder, postorder } )

// WAIT: More hooks vs augmenting base_btree


#include <utility>
#include <memory>
#include <functional>
#include <stack>
#include <queue>
#include <string>
#include <cmath>

/* delete_me */
#include <iostream>
#include <iomanip>

#include "req.hh"

namespace iz {

    template <typename T>
    using shared = std::shared_ptr<T>;

    template <typename T, typename T_Node, typename Less = std::less<T> >
    class basic_btree
    {
        private:
            struct Greater {
                inline bool operator () (const T& lhs, const T& rhs) {
                    return less(rhs, lhs);
                }
            };

            struct Equal {
                inline bool operator () (const T& lhs, const T& rhs) {
                    return !(less(lhs, rhs) || greater(lhs, rhs));
                }
            };

            struct Not_Equal {
                inline bool operator () (const T& lhs, const T& rhs) {
                    return !equal(lhs, rhs);
                }
            };

        protected:
            shared<T_Node> root;
            static const shared<T_Node> NIL;

            static Less less;
            static Greater greater;
            static Equal equal;
            static Not_Equal not_equal;

            static shared<T_Node> static_search(const T&, const shared<T_Node>&);
            static shared<T_Node> static_search_map(const T&, const shared<T_Node>&, std::function<void(const shared<T_Node>&)>);
            static shared<T_Node> static_search_interval_intersection(const T&, const T&, const shared<T_Node>&);
            static shared<T_Node> successor(shared<T_Node>);
            static shared<T_Node> predecessor(shared<T_Node>);
            static shared<T_Node> min(shared<T_Node>);
            static shared<T_Node> max(shared<T_Node>);

			/* Map functions. */
			static void static_preorder_map(shared<T_Node>, std::function<void(const shared<T_Node>&)>);
			static void static_inorder_map(shared<T_Node>, std::function<void(const shared<T_Node>&)>);
			static void static_endorder_map(shared<T_Node>, std::function<void(const shared<T_Node>&)>);

            void left_rotate(shared<T_Node>,
                    std::function<void(shared<T_Node> downlifted, shared<T_Node> uplifted)> post_hook);
            void left_rotate(shared<T_Node>);

            void right_rotate(shared<T_Node>,
                    std::function<void(shared<T_Node> downlifted, shared<T_Node> uplifted)> post_hook);
            void right_rotate(shared<T_Node>);

            void transplant(shared<T_Node>, shared<T_Node>);

        public:
            basic_btree() {
                root = NIL;
            }

            class const_iterator
            {
                protected:
                    // shared<T_Node> current;
                    std::weak_ptr<T_Node> current;

                public:
                    const_iterator(shared<T_Node> begin = NIL);

                    const_iterator& operator ++ ();
                    const_iterator operator ++ (int);

                    const_iterator& operator -- ();
                    const_iterator operator -- (int);

                    const_iterator& find(const T&, const shared<T_Node>&);

                    bool operator == (const const_iterator& other) const;
                    bool operator != (const const_iterator& other) const;

                    const T& operator * ();
            };

            const_iterator begin() const {
                return const_iterator(root);
            }
            const_iterator end() const {
                const_iterator itr;
                return itr;
            }
            const_iterator find(const T&) const;

            inline void clear();
            inline bool empty() const;
            shared<T_Node> search(const T&) const;
			shared<T_Node> search_map(const T&, std::function<void(const shared<T_Node>&)>) const;
            shared<T_Node> search_interval_intersection(const T&, const T&) const;

			void preorder_map(std::function<void(const shared<T_Node>&)>);
			void inorder_map(std::function<void(const shared<T_Node>&)>);
			void endorder_map(std::function<void(const shared<T_Node>&)>);

			void print_tree(unsigned short);

			void operator = (basic_btree& other);

            /* Interface. */
            virtual T& insert(const T&) = 0;
            virtual shared<T_Node> extract(shared<T_Node>) = 0;


			void print_levels(std::ostream&) const;

            /* TODO: Move outside */
			void pretty_print() {
				static_pretty_print(root);
			}

			void static_pretty_print(shared<T_Node> current, int indent = 0) {
				req(current != nullptr);

				if (current != NIL) {
					static_pretty_print(current->right, indent + 10);

					std::cout << '\n' << std::setw(indent) << ' ';
					std::cout << current << '\n';

					static_pretty_print(current->left, indent + 10);
				}
			}
    };

	template <typename T, typename T_Node, typename Less>
	void basic_btree<T, T_Node, Less>::print_levels(std::ostream& out) const
	{
		std::queue< std::pair<shared<T_Node>, unsigned> > fringe;
		shared<T_Node> current_node, temp;
		unsigned prev_level, current_level;

		req(root != nullptr);
		if (root == NIL) {
			return;
		}

		prev_level = 0;
		fringe.push({ root, 1 });

		unsigned tab, tabs;
		while (!fringe.empty()) {
			current_node = fringe.front().first;
			current_level = fringe.front().second;

			req(current_node != nullptr, "[Debug]");

			if (current_node != NIL) {
				fringe.push(std::pair<shared<T_Node>, unsigned>(current_node->left, current_level + 1));
				fringe.push(std::pair<shared<T_Node>, unsigned>(current_node->right, current_level + 1));
			}

			if (prev_level != current_level) {
				prev_level = current_level;
				out << '\n';

				/* Left margin. */
				tabs = (current_node->size > 1) ? ((unsigned)std::log2(current_node->size * 4)) : 2;
				for (
					tab = 0;
					tab < tabs;
					++tab)
				{
					out << ' ';
				}
			}
			else {
				/* Left padding. */
				tabs = (current_node->size > 1) ? ((unsigned)std::log2(current_node->size * 4)) : 1;
				for (
					tab = 0;
					tab < tabs;
					++tab)
				{
					out << ' ';
				}
			}

			if (current_node != NIL)
				out << current_node;
			else
				out << ' ';

			fringe.pop();
		}
	}

    template <typename T, typename T_Node, typename Less>
    typename basic_btree<T, T_Node, Less>::const_iterator
        basic_btree<T, T_Node, Less>::find(const T& key) const
    {
        return end().find(key, root);
    }

    /* # Begin::Iterator */
    template <typename T, typename T_Node, typename Less>
    basic_btree<T, T_Node, Less>::const_iterator::const_iterator(shared<T_Node> begin)
    {
        current = min(begin);
    }

    /* Relational operators. */
    template <typename T, typename T_Node, typename Less>
    bool basic_btree<T, T_Node, Less>::const_iterator::operator == (const const_iterator& other) const {
        req(!current.expired(), "Iterator was invalidated.");

        return static_cast< shared<T_Node> >(current) == static_cast< shared<T_Node> >(other.current);
    }

    template <typename T, typename T_Node, typename Less>
    bool basic_btree<T, T_Node, Less>::const_iterator::operator != (const const_iterator& other) const {
        return !(*this == other);
    }

    /* Increment operators. */
    template <typename T, typename T_Node, typename Less>
    typename basic_btree<T, T_Node, Less>::const_iterator&
        basic_btree<T, T_Node, Less>::const_iterator::operator ++ ()
    {
        req(!current.expired(), "Iterator was invalidated.");

        current = successor(static_cast< shared<T_Node> >(current));
        return *this;
    }

    template <typename T, typename T_Node, typename Less>
    typename basic_btree<T, T_Node, Less>::const_iterator
    basic_btree<T, T_Node, Less>::const_iterator::operator ++ (int)
    {
        const_iterator ret_val = *this;
        ++(*this);
        return ret_val;
    }

    /* Decrement operators. */
    template <typename T, typename T_Node, typename Less>
    typename basic_btree<T, T_Node, Less>::const_iterator&
        basic_btree<T, T_Node, Less>::const_iterator::operator -- ()
    {
        req(!current.expired(), "Iterator was invalidated.");

        current = predecessor(static_cast< shared<T_Node> >(current));
        return *this;
    }

    template <typename T, typename T_Node, typename Less>
    typename basic_btree<T, T_Node, Less>::const_iterator
        basic_btree<T, T_Node, Less>::const_iterator::operator -- (int)
    {
        const_iterator ret_val = *this;
        --(*this);
        return ret_val;
    }

    /* * - deref */
    template <typename T, typename T_Node, typename Less>
    const T& basic_btree<T, T_Node, Less>::const_iterator::operator * ()
    {
        shared<T_Node> temp(static_cast< shared<T_Node> >(current));

        req(temp != nullptr);
        req(temp != NIL);

        return temp->data;
    }

    /* Search for key btree of =root=. */
    template <typename T, typename T_Node, typename Less>
    typename basic_btree<T, T_Node, Less>::const_iterator&
        basic_btree<T, T_Node, Less>::const_iterator::find(const T& key, const shared<T_Node>& root)
    {
        current = static_search(key, root);
        return *this;
    }
    /* # End Of::iterator */


	/* Assign. */
	template <typename T, typename T_Node, typename Less>
	void basic_btree<T, T_Node, Less>::operator = (basic_btree<T, T_Node, Less>& other)
	{
		std::stack< shared<T_Node> > fringe;

		clear();

		if (other.empty()) {
			return;
		}

		root = std::make_shared<T_Node>(NIL, NIL, NIL);
		fringe.push(root);

		other.preorder_map([&](const shared<T_Node>& other_node) {

			shared<T_Node> current, backup_parent;

			req(!fringe.empty());
			req(fringe.top() != nullptr);
            req(fringe.top() != NIL);

			current = fringe.top();
			fringe.pop();

			backup_parent = current->parent;
			*current = *other_node;
			current->parent = backup_parent;

            /*
			 * FOR_EACH { child of =other_node= | child is !NIL} => current gets a child.
			 */
            if (other_node->right != NIL) {
                current->right = std::make_shared<T_Node>(NIL, NIL, current);
                fringe.push(current->right);
            }
            else {
                current->right = NIL;
            }

            if (other_node->left != NIL) {
                current->left = std::make_shared<T_Node>(NIL, NIL, current);
                fringe.push(current->left);
            }
            else {
                current->left = NIL;
            }
		});
	}

    /* Search =this= tree. */
    template <typename T, typename T_Node, typename Less>
    shared<T_Node> basic_btree<T, T_Node, Less>::search(const T& key) const
    {
        shared<T_Node> node(root);

        while (node != NIL && not_equal(node->data, key)) {
            if (less(key, node->data)) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }

        return node;
    }


	template <typename T, typename T_Node, typename Less>
	void basic_btree<T, T_Node, Less>::print_tree(unsigned short option)
	{
		switch (option) {
		case 1:
			preorder_map([](const shared<T_Node>& node) {
				std::cout << node << ", ";
			});
			std::cout << '\n';
			break;

		case 2:
			inorder_map([](const shared<T_Node>& node) {
				std::cout << node << ", ";
			});
			std::cout << '\n';
			break;

		case 3:
			endorder_map([](const shared<T_Node>& node) {
				std::cout << node << ", ";
			});
			std::cout << '\n';
			break;

		case 4:
			pretty_print();
			std::cout << '\n';
			break;

		default:
			std::cout << "Invalid option.\n";
			exit(-5);
		}
	}


	template <typename T, typename T_Node, typename Less>
	void basic_btree<T, T_Node, Less>::preorder_map(std::function<void(const shared<T_Node>&)> action)
	{
		return static_preorder_map(root, action);
	}

	template <typename T, typename T_Node, typename Less>
	void basic_btree<T, T_Node, Less>::static_preorder_map(shared<T_Node> begin, std::function<void(const shared<T_Node>&)> action)
	{
		std::stack< shared<T_Node> > fringe;
		shared<T_Node> current;

		if (begin != NIL) {
			req(begin != nullptr);

			fringe.push(begin);
		}

		while (!fringe.empty()) {
			current = fringe.top();
			fringe.pop();

			req(current != nullptr);

			action(current);

			if (current->right != NIL) {
				fringe.push(current->right);
			}

			if (current->left != NIL) {
				fringe.push(current->left);
			}
		}
	}

	template <typename T, typename T_Node, typename Less>
	void basic_btree<T, T_Node, Less>::endorder_map(std::function<void(const shared<T_Node>&)> action)
	{
		return static_endorder_map(root, action);
	}

	template <typename T, typename T_Node, typename Less>
	void basic_btree<T, T_Node, Less>::static_endorder_map(shared<T_Node> begin, std::function<void(const shared<T_Node>&)> action)
	{
		std::stack< shared<T_Node> > fringe;
		shared<T_Node> current;

		req(begin != NIL);
		fringe.push(begin);

		while (!fringe.empty()) {
			current = fringe.top();
			fringe.pop();

			req(current != nullptr);

			if (current == NIL) {
				req(!fringe.empty());

				current = fringe.top();
				fringe.pop();

				req(current != nullptr);
				req(current != NIL);

				action(current);
			}
			else {
				fringe.push(current);
				fringe.push(NIL);

				if (current->right != NIL) {
					fringe.push(current->right);
				}

				if (current->left != NIL) {
					fringe.push(current->left);
				}
			}
		}
	}

	template <typename T, typename T_Node, typename Less>
	void basic_btree<T, T_Node, Less>::inorder_map(std::function<void(const shared<T_Node>&)> action)
	{
		return static_inorder_map(root, action);
	}

	template <typename T, typename T_Node, typename Less>
	void basic_btree<T, T_Node, Less>::static_inorder_map(shared<T_Node> begin, std::function<void(const shared<T_Node>&)> action)
	{
		req(begin != nullptr);

		for (
			shared<T_Node> current = min(begin);
			current != NIL;
			current = successor(current)
			)
		{
			action(current);
		}
	}


    template <typename T, typename T_Node, typename Less>
    shared<T_Node> basic_btree<T, T_Node, Less>::search_interval_intersection(const T& keyA, const T& keyB) const
    {
        return static_search_interval_intersection(keyA, keyB, root);
    }

    template <typename T, typename T_Node, typename Less>
    shared<T_Node>
    basic_btree<T, T_Node, Less>::static_search_interval_intersection(
            const T& keyA, const T& keyB, const shared<T_Node>& begin
            )
    {
        req(begin != nullptr);

        shared<T_Node> node(begin);
        T lower_key, heigher_key;

        lower_key = std::min(keyA, keyB);
        heigher_key = std::max(keyA, keyB);

        while (
                node != NIL &&
                    (
                     less(node->data, lower_key) ||
                     ( greater(node->data, heigher_key) && not_equal(node->data, heigher_key) )
                    )
                )
        {
            if (less(heigher_key, node->data)) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }

        return node;
    }

    /* Search tree of root =begin=. */
    template <typename T, typename T_Node, typename Less>
    shared<T_Node> basic_btree<T, T_Node, Less>::static_search(const T& key, const shared<T_Node>& begin)
    {
        shared<T_Node> node(begin);

        while (node != NIL && not_equal(node->data, key)) {
            if (less(key, node->data)) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }

        return node;
    }

	/* Search tree of root =begin=. */
	template <typename T, typename T_Node, typename Less>
	shared<T_Node>
		basic_btree<T, T_Node, Less>
		::static_search_map(const T& key, const shared<T_Node>& begin, std::function<void(const shared<T_Node>&)> action)
	{
		shared<T_Node> node(begin);

		while (node != NIL && not_equal(node->data, key)) {
			action(node);

			if (less(key, node->data)) {
				node = node->left;
			}
			else {
				node = node->right;
			}
		}

		return node;
	}

	template <typename T, typename T_Node, typename Less>
	shared<T_Node>
		basic_btree<T, T_Node, Less>
		::search_map(const T& key, std::function<void(const shared<T_Node>&)> action) const
	{
		return static_search_map(key, root, action);
	}

    /* Make connection: parent of =discarded= <-> =replacement=. */
    template <typename T, typename T_Node, typename Less>
    void basic_btree<T, T_Node, Less>::transplant(shared<T_Node> discarded, shared<T_Node> replacement)
    {
        if (discarded->parent == NIL) {
            root = replacement;
        }
        else if (discarded == discarded->parent->left) {
            discarded->parent->left = replacement;
        }
        else {
            discarded->parent->right = replacement;
        }

        replacement->parent = discarded->parent;
    }


    /* Left rotate */
    template <typename T, typename T_Node, typename Less>
    void basic_btree<T, T_Node, Less>::left_rotate(shared<T_Node> downlifted,
            std::function<void(shared<T_Node>, shared<T_Node>)> post_hook)
    {
        shared<T_Node> uplifted;

        /* New parent. */
        uplifted = downlifted->right;

        req(downlifted != NIL);
        req(downlifted->right != NIL);

        /* Don't abandon any child. */
        downlifted->right = uplifted->left;

        if (uplifted->left != NIL) {
            uplifted->left->parent = downlifted;
        }

        /* Your parent is my parent. */
        uplifted->parent = downlifted->parent;

        if (downlifted->parent == NIL) {
            root = uplifted;
        }
        else if (downlifted == downlifted->parent->left) {
            downlifted->parent->left = uplifted;
        }
        else {
            downlifted->parent->right = uplifted;
        }

        /* Settle down old child. */
        uplifted->left = downlifted;
        downlifted->parent = uplifted;

        post_hook(downlifted, uplifted);
    }

    template <typename T, typename T_Node, typename Less>
    void basic_btree<T, T_Node, Less>::left_rotate(shared<T_Node> downlifted)
    {
        return left_rotate(downlifted, [](shared<T_Node>, shared<T_Node>) { return; });
    }


    /* Right rotate */
    template <typename T, typename T_Node, typename Less>
    void basic_btree<T, T_Node, Less>::right_rotate( shared<T_Node> downlifted,
            std::function<void(shared<T_Node>, shared<T_Node>)> post_hook)
    {
        shared<T_Node> uplifted;

        /* New parent. */
        uplifted = downlifted->left;

        req(downlifted != NIL);
        req(downlifted->left != NIL);

        /* Don't abandon any child. */
        downlifted->left = uplifted->right;

        if (uplifted->right != NIL) {
            uplifted->right->parent = downlifted;
        }

        /* Your parent is my parent. */
        uplifted->parent = downlifted->parent;

        if (downlifted->parent == NIL) {
            root = uplifted;
        }
        else if (downlifted == downlifted->parent->left) {
            downlifted->parent->left = uplifted;
        }
        else {
            downlifted->parent->right = uplifted;
        }

        /* Settle down old child. */
        uplifted->right = downlifted;
        downlifted->parent = uplifted;

        post_hook(downlifted, uplifted);
    }

    template <typename T, typename T_Node, typename Less>
    void basic_btree<T, T_Node, Less>::right_rotate(shared<T_Node> downlifted)
    {
        return right_rotate(downlifted, [](shared<T_Node>, shared<T_Node>) { return; });
    }


    /* Macro for generating the body of successor & predecessor. */
#define generate_predecessor__successor(left__right, max__min)	\
    shared<T_Node> aux(NIL);									\
                                                                \
    if (node == NIL)											\
    return aux;												    \
                                                                \
    if (node->left__right != NIL)								\
    return max__min(node->left__right);						    \
                                                                \
    aux = node->parent;											\
    while (aux != NIL && node == aux->left__right) {			\
        node = aux;												\
        aux = aux->parent;										\
    }															\
    return aux;

    template <typename T, typename T_Node, typename Less>
    shared<T_Node> basic_btree<T, T_Node, Less>::successor(shared<T_Node> node)
    {
        generate_predecessor__successor(right, min);
    }

    template <typename T, typename T_Node, typename Less>
    shared<T_Node> basic_btree<T, T_Node, Less>::predecessor(shared<T_Node> node)
    {
        generate_predecessor__successor(left, max);
    }


    /* Macro for generating the body of min & max. */
#define generate_min__max(left__right)	\
    shared<T_Node> aux(NIL);			\
                                        \
    while (node != NIL) {				\
        aux = node;						\
        node = node->left__right;		\
    }									\
    return aux;

    template <typename T, typename T_Node, typename Less>
    shared<T_Node> basic_btree<T, T_Node, Less>::min(shared<T_Node> node)
    {
        generate_min__max(left);
    }

    template <typename T, typename T_Node, typename Less>
    shared<T_Node> basic_btree<T, T_Node, Less>::max(shared<T_Node> node)
    {
        generate_min__max(right);
    }

    template <typename T, typename T_Node, typename Less>
    bool basic_btree<T, T_Node, Less>::empty() const
    {
        return root == NIL;
    }

    template <typename T, typename T_Node, typename Less>
    void basic_btree<T, T_Node, Less>::clear()
    {
        req(root != nullptr);
        req(NIL != nullptr);

        NIL->parent = nullptr;
        root = NIL;
    }


    /* Static members initialization. */
    template <typename T, typename T_Node, typename Less>
    const shared<T_Node> basic_btree<T, T_Node, Less>::NIL = std::make_shared<T_Node>();

    template <typename T, typename T_Node, typename Less>
    Less basic_btree<T, T_Node, Less>::less;

    template <typename T, typename T_Node, typename Less>
    typename basic_btree<T, T_Node, Less>::Greater basic_btree<T, T_Node, Less>::greater;

    template <typename T, typename T_Node, typename Less>
    typename basic_btree<T, T_Node, Less>::Equal basic_btree<T, T_Node, Less>::equal;

    template <typename T, typename T_Node, typename Less>
    typename basic_btree<T, T_Node, Less>::Not_Equal basic_btree<T, T_Node, Less>::not_equal;
}

#endif // ! __tree_hh
