//
// Created by void on 4/28/2018.
//

#ifndef SD_BST_H
#define SD_BST_H

#include <memory>
#include <ostream>
#include <stack>
#include <functional>
#include <iostream>
#include "req.hh"
#include "Node.hh"

namespace iz {

    template <typename T, typename Tnode>
    class BST;

    template <typename T>
    using shared = std::shared_ptr<T>;

    /* BEGIN - BST - */
    template <typename T, typename Tnode = Node<T> >
    class BST {
    public:
        class iterator
        {
        private:
            shared<Tnode> current;

        public:
            explicit iterator(shared<Tnode> begin = nullptr);

            iterator& operator ++ ();
            iterator operator ++ (int);

            iterator& find(const T&, const shared<Tnode>&);

            bool operator == (const iterator& other) const;
            bool operator != (const iterator& other) const;

            const T& operator * ();
        };

        /* Iterators */
        iterator begin() const {
            return iterator(root);
        }

        iterator end() const {
            iterator itr;
            return itr;
        }

        shared<Tnode> print(std::ostream& out) {
            if (root != nullptr)
                out << root;
            return root;
        }

        /* Constructor & destructor. */
        BST() : root{ nullptr } {}
        ~BST() { root = nullptr; }

        /* Methods that mutate the root */
        shared<Tnode> insert(const T&);
        shared<Tnode> extract(shared<Tnode>);
        void transplant(shared<Tnode>, shared<Tnode>);

        /* Pure methods */
        /* UI */
        bool empty() const;
        iterator find(const T& key) const;

        /* Helpers. [TODO] => protected. */
        shared<Tnode> search(const T&) const;
        static shared<Tnode> static_search(const T&, const shared<Tnode>&);
        static shared<Tnode> successor(shared<Tnode>);
        static shared<Tnode> predecessor(shared<Tnode>);
        static shared<Tnode> min(shared<Tnode>);
        static shared<Tnode> max(shared<Tnode>);

    protected:
        shared<Tnode> root;
    };

    template <typename T, typename Tnode>
    typename BST<T, Tnode>::iterator
    BST<T, Tnode>::find(const T& key) const
    {
        return end().find(key, root);
    }

    template <typename T, typename Tnode>
    shared<Tnode> BST<T, Tnode>::search(const T& key) const
    {
        shared<Tnode> node(root);

        while (node != nullptr && node->data != key) {
            if (key < node->data) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }

        return node;
    }

    template <typename T, typename Tnode>
    shared<Tnode> BST<T, Tnode>::static_search(const T& key, const shared<Tnode>& begin)
    {
        shared<Tnode> node(begin);

        while (node != nullptr && node->data != key) {
            if (key < node->data) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }

        return node;
    }

    template <typename T, typename Tnode>
    shared<Tnode> BST<T, Tnode>::extract(shared<Tnode> extracted)
    {
        if (extracted == nullptr) {
            return nullptr;
        }

        if (extracted->left == nullptr) {
            transplant(extracted, extracted->right);
        }
        else if (extracted->right == nullptr) {
            transplant(extracted, extracted->left);
        }
        else {
            shared<Tnode> replacement = successor(extracted);

            if (replacement != extracted->right) {
                transplant(replacement, replacement->right);

                /* Settle right connection. */
                replacement->right = extracted->right;
                extracted->right->parent = replacement;
            }

            transplant(extracted, replacement);

            /* Settle left connection. */
            replacement->left = extracted->left;
            extracted->left->parent = replacement;
        }

        return extracted;
    }

    template <typename T, typename Tnode>
    void BST<T, Tnode>::transplant(shared<Tnode> discarded, shared<Tnode> replacement)
    {
        if (discarded->parent == nullptr) {
            root = replacement;
        }
        else if (discarded == discarded->parent->left) {
            discarded->parent->left = replacement;
        }
        else {
            discarded->parent->right = replacement;
        }

        if (replacement != nullptr) {
            replacement->parent = discarded->parent;
        }
    }

    template <typename T, typename Tnode>
    shared<Tnode> BST<T, Tnode>::insert(const T& val)
    {
        shared<Tnode> new_node = std::make_shared<Tnode>(val);

        shared<Tnode> current_node, current_node_parent;

        current_node = root;
        current_node_parent = nullptr;

        while (current_node != nullptr) {
            current_node_parent = current_node;

            if (new_node->data < current_node->data)
                current_node = current_node->left;
            else
                current_node = current_node->right;
        }

        new_node->parent = current_node_parent;

        /* If tree is empty. */
        if (current_node_parent == nullptr) {
            root = new_node;
            return new_node;
        }

        /* Else attach the new_node to the proper side. */
        if (new_node->data < new_node->parent->data)
            new_node->parent->left = new_node;
        else
            new_node->parent->right = new_node;

        return new_node;
    }


    /* Macro for generating the body of successor & predecessor. */
#define cessor(left__right, max__min, CMP)                      \
    shared<Tnode> aux(nullptr);                                 \
                                                                \
    if (node == nullptr)                                        \
        return aux;                                             \
                                                                \
    if (node->left__right != nullptr)                           \
        return max__min(node->left__right);                     \
                                                                \
    aux = node->parent;                                         \
    while (aux != nullptr && node->data CMP aux->data) {	\
        node = aux;                                             \
        aux = aux->parent;                                      \
    }                                                           \
    return aux;

    template <typename T, typename Tnode>
    shared<Tnode> BST<T, Tnode>::successor(shared<Tnode> node)
    {
        cessor(right, min, >);
    }

    template <typename T, typename Tnode>
    shared<Tnode> BST<T, Tnode>::predecessor(shared<Tnode> node)
    {
        cessor(left, max, <);
    }


    /* Macro for generating the body of min & max. */
#define mm(left__right)				\
    shared<Tnode> aux(nullptr);                 \
                                                \
    while (node != nullptr) {                   \
        aux = node;                             \
        node = node->left__right;               \
    }                                           \
    return aux;

    template <typename T, typename Tnode>
    shared<Tnode> BST<T, Tnode>::min(shared<Tnode> node)
    {
        mm(left);
    }

    template <typename T, typename Tnode>
    shared<Tnode> BST<T, Tnode>::max(shared<Tnode> node)
    {
        mm(right);
    }

    template <typename T, typename Tnode>
    bool BST<T, Tnode>::empty() const
    {
        return root == nullptr;
    }
    /* END - BST - */



    /* BEGIN - BST::iterator - */
    /* Constructor. */
    template <typename T, typename Tnode>
    BST<T, Tnode>::iterator::iterator(shared<Tnode> begin)
    {
        current = min(begin);
    }

    /* Relational operators. */
    template <typename T, typename Tnode>
    bool BST<T, Tnode>::iterator::operator == (const iterator& other) const {
        return current == other.current;
    }

    template <typename T, typename Tnode>
    bool BST<T, Tnode>::iterator::operator != (const iterator& other) const {
        return !(*this == other);
    }

    /* Increment operators. */
    template <typename T, typename Tnode>
    typename BST<T, Tnode>::iterator&
    BST<T, Tnode>::iterator::operator ++ ()
    {
        current = successor(current);
        return *this;
    }

    template <typename T, typename Tnode>
    typename BST<T, Tnode>::iterator
    BST<T, Tnode>::iterator::operator ++ (int)
    {
        iterator ret_val = *this;
        ++(*this);
        return ret_val;
    }

    /* Immutable access. */
    template <typename T, typename Tnode>
    const T& BST<T, Tnode>::iterator::operator * () {
        req(current != nullptr, "Iterator == end()");
        return current->data;
    }

    template<typename T, typename Tnode>
    typename BST<T, Tnode>::iterator&
    BST<T, Tnode>::iterator::find(const T& key, const shared<Tnode>& root)
    {
        current = static_search(key, root);
        return *this;
    }
    /* END - BST::iterator - */
}

#endif //SD_BST_H



/*
  template <typename T, typename Tnode>
  void BST<T, Tnode>::inorder(std::function<void(shared<Tnode>&)> apply)
  {
  std::stack< shared<Tnode> > st;
  shared<Tnode> current;

  if (root == nullptr)
  return;

  st.push(root);

  while (!st.empty()) {
  if (st.top() != nullptr) {
  st.push(st.top()->left);
  continue;
  }

  st.pop(); // pop() the nullptr.

  if (!st.empty()) {
  current = st.top();
  st.pop();

  apply(current);

  st.push(current->right);
  }
  }
  }
*/
