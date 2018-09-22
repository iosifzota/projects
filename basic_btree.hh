#ifndef __tree_hh
#define __tree_hh

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

#define scoped static
#define In static
#define Ext static

namespace iz {

    template <typename T>
    using shared = std::shared_ptr<T>;

    template <typename T>
    using weak = std::weak_ptr<T>;

    template <typename T>
    using fn = std::function<T>;

    using std::cout;
    using std::make_shared;

#ifdef self
#undef self
#endif
#define self basic_btree<T, T_Node, Less>

#ifdef T8T_Node8Less
#undef T8T_Node8Less
#endif
#define T8T_Node8Less template <typename T, typename T_Node, typename Less>

    template <typename T, typename T_Node, typename Less = std::less<T> >
    class basic_btree
    {
        using shared_node = shared<T_Node>;
        using weak_node = weak<T_Node>;
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
        Ext shared_node static_search(const T&, const shared_node&);
        Ext shared_node static_search_map(const T&, const shared_node&, fn<void(const shared_node&)>);
        Ext shared_node static_search_interval_intersection(const T&, const T&, const shared_node&);
        Ext shared_node successor(shared_node);
        Ext shared_node predecessor(shared_node);
        Ext shared_node min(shared_node);
        Ext shared_node max(shared_node);

        /* Map functions. */
        Ext void static_preorder_map(shared_node, fn<void(const shared_node&)>);
        Ext void static_inorder_map(shared_node, fn<void(const shared_node&)>);
        Ext void static_endorder_map(shared_node, fn<void(const shared_node&)>);

        void left_rotate(shared_node, fn<void(shared_node, shared_node)>);
        void left_rotate(shared_node);
        void right_rotate(shared_node, fn<void(shared_node, shared_node)>);
        void right_rotate(shared_node);
        void transplant(shared_node, shared_node);

        shared_node root;
        In const shared_node NIL;
        /* helpers */
        In Less less;
        In Greater greater;
        In Equal equal;
        In Not_Equal not_equal;

    public:
        basic_btree() : root{ NIL } {}
        virtual ~basic_btree() { root = nullptr; }

        inline void clear();
        inline bool empty() const;
        shared_node search(const T&) const;
        shared_node search_map(const T&, fn<void(const shared_node&)>) const;
        shared_node search_interval_intersection(const T&, const T&) const;

        void preorder_map(fn<void(const shared_node&)>);
        void inorder_map(fn<void(const shared_node&)>);
        void endorder_map(fn<void(const shared_node&)>);

        void print_tree(unsigned short);
        /* TODO: fix return type & define copy constructor. */
        void operator = (basic_btree&);

        /* Interface. */
        virtual T& insert(const T&) = 0;
        virtual shared_node extract(shared_node) = 0;

        void print_levels(std::ostream&) const;
        void pretty_print();

        class const_iterator
        {
        protected:
            weak_node itr;
        public:
            explicit const_iterator(shared_node = NIL);
            const_iterator& operator ++ ();
            const_iterator operator ++ (int);
            const_iterator& operator -- ();
            const_iterator operator -- (int);
            const_iterator& find(const T&, const shared_node&);
            bool operator == (const const_iterator&) const;
            bool operator != (const const_iterator&) const;
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
    };

    T8T_Node8Less
    typename self::const_iterator
    self::find(const T& key) const {
        return end().find(key, root);
    }

    // ========================================================
    /* # Begin::Iterator */
    T8T_Node8Less
    self::const_iterator::const_iterator(shared<T_Node> begin) {
        itr = min(begin);
    }

    /* Relational operators. */
    T8T_Node8Less
    bool
    self::const_iterator::operator == (const const_iterator& other) const {
        req(!itr.expired());
        return
            static_cast<shared<T_Node>>(itr) == static_cast<shared<T_Node>>(other.itr);
    }
    T8T_Node8Less
    bool
    self::const_iterator::operator != (const const_iterator& other) const {
        return !(*this == other);
    }

    /* Increment operators. */
    T8T_Node8Less
    typename self::const_iterator&
    self::const_iterator::operator ++ () {
        req(!itr.expired());
        itr = successor(static_cast<shared<T_Node>>(itr));
        return *this;
    }

    T8T_Node8Less
    typename self::const_iterator
    self::const_iterator::operator ++ (int) {
        const_iterator ret_val = *this;
        ++(*this);
        return ret_val;
    }

    /* Decrement operators. */
    T8T_Node8Less
    typename self::const_iterator&
    self::const_iterator::operator -- () {
        req(!itr.expired());
        itr = predecessor(static_cast<shared<T_Node>>(itr));
        return *this;
    }

    T8T_Node8Less
    typename self::const_iterator
    self::const_iterator::operator -- (int) {
        const_iterator ret_val = *this;
        --(*this);
        return ret_val;
    }

    /* * - deref */
    T8T_Node8Less
    const T&
    self::const_iterator::operator * () {
        shared<T_Node> temp(static_cast<shared<T_Node>>(itr));
        req(temp != nullptr);
        req(temp != NIL);
        return temp->data;
    }

    /* Search for key btree of =root=. */
    T8T_Node8Less
    typename self::const_iterator&
    self::const_iterator::find(const T& key, const shared<T_Node>& root) {
        itr = static_search(key, root);
        return *this;
    }
    /* # End Of::iterator */

    /* Assign. */
    T8T_Node8Less
    void
    self::operator = (self& other) {
        std::stack< shared<T_Node> > fringe;
        if (other.empty()) {
            return;
        }
        clear(); // strange
        root = make_shared<T_Node>(NIL, NIL);
        root->parent = NIL;
        fringe.push(root);
        /* itr over both =this and other=, and copy */
        other.preorder_map([&](const shared<T_Node>& other_node) {
                shared<T_Node> this_itr, temp;
                req(!fringe.empty());
                req(fringe.top() != nullptr);
                req(fringe.top() != NIL);

                this_itr = fringe.top();
                fringe.pop();
                temp = this_itr->parent;
                /* copy data */
                *this_itr = *other_node; // TODO: copy only {meta,}data
                /* set links */ // above TODO will make this 2 obsolete
                this_itr->parent = temp;
                this_itr->right = this_itr->left = NIL;
                /* make "room" for the children of =this_itr= */
                if (other_node->right != NIL) {
                    this_itr->right = make_shared<T_Node>(NIL, NIL);
                    this_itr->right->parent = this_itr;

                    fringe.push(this_itr->right);
                }
                if (other_node->left != NIL) {
                    this_itr->left = make_shared<T_Node>(NIL, NIL);
                    this_itr->left->parent = this_itr;

                    fringe.push(this_itr->left);
                }
            });
    }

    T8T_Node8Less
    shared<T_Node>
    self::search(const T& key) const {
        return static_search(key, root);
    }

    T8T_Node8Less
    void
    self::preorder_map(fn<void(const shared<T_Node>&)> action) {
        return static_preorder_map(root, action);
    }
    T8T_Node8Less
    void
    self::static_preorder_map(shared<T_Node> begin,
                              fn<void(const shared<T_Node>&)> action)
    {
        std::stack< shared<T_Node> > fringe;
        shared<T_Node> current;
        if (begin != NIL) {
            req(begin != nullptr);
            fringe.push(begin);
        }
        while (!fringe.empty()) {
            req((current = fringe.top()) != nullptr);
            fringe.pop();
            action(current);

            if (current->right != NIL) {
                fringe.push(current->right);
            }
            if (current->left != NIL) {
                fringe.push(current->left);
            }
        }
    }

    T8T_Node8Less
    void
    self::endorder_map(fn<void(const shared<T_Node>&)> action) {
        return static_endorder_map(root, action);
    }

    T8T_Node8Less
    void
    self::static_endorder_map(shared<T_Node> begin,
                              fn<void(const shared<T_Node>&)> action)
    {
        std::stack< shared<T_Node> > fringe;
        shared<T_Node> current;
        req(begin != NIL);
        fringe.push(begin);
        while (!fringe.empty()) {
            req((current = fringe.top()) != nullptr);
            fringe.pop();

            if (current == NIL) { // touched sentinel => pop & act
                req(!fringe.empty());
                current = fringe.top();
                fringe.pop();

                req(current != nullptr);
                req(current != NIL);
                action(current);
            } else {
                fringe.push(current);
                fringe.push(NIL); // sentinel
                if (current->right != NIL) {
                    fringe.push(current->right);
                }
                if (current->left != NIL) {
                    fringe.push(current->left);
                }
            }
        }
    }

    T8T_Node8Less
    void
    self::inorder_map(fn<void(const shared<T_Node>&)> action) {
        return static_inorder_map(root, action);
    }

    T8T_Node8Less
    void
    self::static_inorder_map(shared<T_Node> begin,
                             fn<void(const shared<T_Node>&)> action)
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


    T8T_Node8Less
    shared<T_Node>
    self::search_interval_intersection(const T& keyA, const T& keyB) const {
        return static_search_interval_intersection(keyA, keyB, root);
    }

    T8T_Node8Less
    shared<T_Node>
    self::static_search_interval_intersection(const T& keyA, const T& keyB,
                                              const shared<T_Node>& begin)
    {
        shared<T_Node> node(begin);
        T lower_end, heigher_end;
        req(begin != nullptr);
        lower_end = std::min(keyA, keyB);
        heigher_end = std::max(keyA, keyB);
        while (
            node != NIL &&
            ( // node is outside of [lower_end, heigher_end]
                less(node->data, lower_end) ||
                ( greater(node->data, heigher_end) && not_equal(node->data, heigher_end) )
                )
            )
        {
            node = less(heigher_end, node->data) ? node->left : node->right;
        }
        return node;
    }

    T8T_Node8Less
    shared<T_Node>
    self::static_search(const T& key, const shared<T_Node>& begin) {
        shared<T_Node> node(begin);
        while (node != NIL && not_equal(node->data, key)) {
            node = less(key, node->data) ? node->left : node->right;
        }
        return node;
    }

    /* Search tree of root =begin=. */
    T8T_Node8Less
    shared<T_Node>
    self::static_search_map(const T& key, const shared<T_Node>& begin,
                            fn<void(const shared<T_Node>&)> action)
    {
        shared<T_Node> node(begin);
        while (node != NIL && not_equal(node->data, key)) {
            action(node);
            node = less(key, node->data) ? node->left : node->right;
        }
        return node;
    }

    T8T_Node8Less
    shared<T_Node>
    self::search_map(const T& key, fn<void(const shared<T_Node>&)> action) const {
        return static_search_map(key, root, action);
    }

    /* Make connection: parent_of_=discarded= <-> =replacement=. */
    T8T_Node8Less
    void
    self::transplant(shared<T_Node> discarded, shared<T_Node> replacement) {
        if (discarded->parent == NIL) {
            root = replacement;
        } else if (discarded == discarded->parent->left) {
            discarded->parent->left = replacement;
        } else {
            discarded->parent->right = replacement;
        }
        replacement->parent = discarded->parent;
    }

    /* Left rotate */
    T8T_Node8Less
    void
    self::left_rotate(shared<T_Node> downlifted,
                      fn<void(shared<T_Node>, shared<T_Node>)> post_hook)
    {
        shared<T_Node> uplifted;
        req(downlifted != NIL);
        req(downlifted->right != NIL);

        uplifted = downlifted->right;
        /* Don't abandon any child. */
        downlifted->right = uplifted->left;
        if (uplifted->left != NIL) {
            uplifted->left->parent = downlifted;
        }
        /* Your parent is my parent. */
        uplifted->parent = downlifted->parent;
        /* Oh, ma new son. */
        if (downlifted->parent == NIL) {
            root = uplifted;
        } else if (downlifted == downlifted->parent->left) {
            downlifted->parent->left = uplifted;
        } else {
            downlifted->parent->right = uplifted;
        }
        /* old child. */
        uplifted->left = downlifted;
        downlifted->parent = uplifted;

        post_hook(downlifted, uplifted);
    }

    T8T_Node8Less
    void self::left_rotate(shared<T_Node> downlifted) {
        return left_rotate(downlifted, [](shared<T_Node>, shared<T_Node>) { return; });
    }

    /* Right rotate */
    T8T_Node8Less
    void self::right_rotate(shared<T_Node> downlifted,
                            fn<void(shared<T_Node>, shared<T_Node>)> post_hook)
    {
        shared<T_Node> uplifted;
        req(downlifted != NIL);
        req(downlifted->left != NIL);

        uplifted = downlifted->left;
        /* Don't abandon any child. */
        downlifted->left = uplifted->right;
        if (uplifted->right != NIL) {
            uplifted->right->parent = downlifted;
        }
        /* Your parent is my parent. */
        uplifted->parent = downlifted->parent;
        /* Oh, ma new son. */
        if (downlifted->parent == NIL) {
            root = uplifted;
        } else if (downlifted == downlifted->parent->left) {
            downlifted->parent->left = uplifted;
        } else {
            downlifted->parent->right = uplifted;
        }
        /* old child. */
        uplifted->right = downlifted;
        downlifted->parent = uplifted;

        post_hook(downlifted, uplifted);
    }

    T8T_Node8Less
    void self::right_rotate(shared<T_Node> downlifted) {
        return right_rotate(downlifted, [](shared<T_Node>, shared<T_Node>) { return; });
    }

#define generate_predecessor__successor(left__right, max__min)  \
    shared<T_Node> aux(NIL);                                    \
                                                                \
    if (node == NIL)                                            \
        return aux;                                             \
                                                                \
    if (node->left__right != NIL)                               \
        return max__min(node->left__right);                     \
                                                                \
    aux = node->parent;                                         \
    while (aux != NIL && node == aux->left__right) {            \
        node = aux;                                             \
        aux = aux->parent;                                      \
    }                                                           \
    return aux;

    T8T_Node8Less
    shared<T_Node> self::successor(shared<T_Node> node) {
        generate_predecessor__successor(right, min);
    }

    T8T_Node8Less
    shared<T_Node> self::predecessor(shared<T_Node> node) {
        generate_predecessor__successor(left, max);
    }

    /* Macro for generating the body of min & max. */
#define generate_min__max(left__right)          \
    shared<T_Node> aux(NIL);                    \
                                                \
    while (node != NIL) {                       \
        aux = node;                             \
        node = node->left__right;               \
    }                                           \
    return aux;

    T8T_Node8Less
    shared<T_Node> self::min(shared<T_Node> node) {
        generate_min__max(left);
    }

    T8T_Node8Less
    shared<T_Node> self::max(shared<T_Node> node) {
        generate_min__max(right);
    }

    T8T_Node8Less
    bool self::empty() const {
        return root == NIL;
    }

    T8T_Node8Less
    void self::clear() {
        req(root != nullptr);
        req(NIL != nullptr);
        NIL->parent = nullptr;
        root = NIL;
    }

    T8T_Node8Less
    void
    self::print_tree(unsigned short option) {
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

    T8T_Node8Less
    void
    self::pretty_print() {
        cout << "Deprecated\n";
    }

    T8T_Node8Less
    void
    self::print_levels(std::ostream& out) const {
        req(root != nullptr);
        out << "Deprecated\n";
    }

    /* Static members initialization. */
    T8T_Node8Less
    const shared<T_Node>
    self::NIL = make_shared<T_Node>();

    T8T_Node8Less
    Less
    self::less;

    T8T_Node8Less
    typename self::Greater
    self::greater;

    T8T_Node8Less
    typename self::Equal
    self::equal;

    T8T_Node8Less
    typename self::Not_Equal
    self::not_equal;

}
#undef self
#undef T8T_Node8Less
#endif // ! __tree_hh
