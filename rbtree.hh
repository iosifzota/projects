#ifndef __rbtree_hh
#define __rbtree_hh

#include <iostream>

#include "basic_btree.hh"
#include "RB-node.hh"
#include "req.hh"

#ifdef self
#undef self
#endif
#define self rbtree<T, Less>

#ifdef T8Less
#undef T8Less
#endif
#define T8Less template <typename T, typename Less>

namespace iz {
    template <typename T, typename Less = std::less<T> >
    class rbtree : public basic_btree<T, RB_Node<T>, Less>
    {
    public:
        rbtree();
        virtual ~rbtree() override;

        static void rotate_metadata_fix(const shared_rb_node<T>&, const shared_rb_node<T>&);
        static void upwards_branch_metadata_refresh(shared_rb_node<T>);
        unsigned size() const;
        T median(const T&, const T&) const;

        void construct(T*, T*);

        T& insert(const T&) override;
        T& insert_unique(const T&);
        shared_rb_node<T> extract(shared_rb_node<T>) override;
        shared_rb_node<T> extract(const T&);
        /* Auguments. */
        using basic_btree<T, RB_Node<T>, Less>::empty;
        using basic_btree<T, RB_Node<T>, Less>::clear;
        /* */
        using basic_btree<T, RB_Node<T>, Less>::preorder_map;
        using basic_btree<T, RB_Node<T>, Less>::operator=;

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

        using basic_btree<T, RB_Node<T>, Less>::static_search_map;
        using basic_btree<T, RB_Node<T>, Less>::search_interval_intersection; // DELETE ME

    private:
        void setup_sentinel(const shared_rb_node<T>&);
        void insert_fixup(shared_rb_node<T>);
        void extract_fixup(shared_rb_node<T>);
    };

    T8Less
    void
    self::setup_sentinel(const shared_rb_node<T>& node) {
        req(node != nullptr);
        req(node == NIL);
        node->parent = node->left = node->right = nullptr;
        node->color = BLACK;
    }

    T8Less
    self::rbtree() : basic_btree<T, RB_Node<T>, Less>::basic_btree() {
        setup_sentinel(NIL);
    }
    T8Less
    self::~rbtree() {
        setup_sentinel(NIL);
    }

    T8Less
    T
    self::median(const T& keyA, const T& keyB) const {
        T sum{};
        unsigned nelems = 0;

        T lower_end, heigher_end;
        shared_rb_node<T> intersection, node;

        lower_end = std::min(keyA, keyB);
        heigher_end = std::max(keyA, keyB);
        intersection = this->search_interval_intersection(lower_end, heigher_end);

        if (intersection == this->NIL) {
            return sum;
        }
        /* Add intersection only once. */
        sum    += intersection->data;
        nelems += 1;

        if (this->not_equal(lower_end, intersection->data)) {
            node = this->static_search_map(lower_end, intersection->left, [&](const shared_rb_node<T>& elem) {
                    if (this->greater(elem->data, lower_end)) {
                        sum += elem->data;
                        nelems += 1;

                        sum += elem->right->sum;
                        nelems += elem->right->size;
                    }
                });

            if (node != this->NIL) {
                sum += node->data;
                nelems += 1;

                sum += node->right->sum;
                nelems += node->right->size;
            }
        }

        std::cout << "Left branch sum: " << sum << '\n';
        std::cout << "Nr. on left branch: " << nelems << '\n';

        if (this->equal(heigher_end, intersection->data)) {
            return sum / nelems; // done, nothing to the right.
        }

        node = this->static_search_map(heigher_end, intersection->right, [&](const shared_rb_node<T>& elem) {
                if (this->less(elem->data, heigher_end)) {
                    sum += elem->data;
                    nelems += 1;

                    sum += elem->left->sum;
                    nelems += elem->left->size;
                }
            });

        if (node != this->NIL) {
            sum += node->data;
            nelems += 1;

            sum += node->left->sum;
            nelems += node->left->size;
        }

        std::cout << "Sum final: " << sum << '\n';
        std::cout << "Nr. final: " << nelems << '\n';

        return sum / nelems;
    }

    T8Less
    void
    self::construct(T* begin, T* end) {
        req(begin != nullptr);
        req(end != nullptr);
        req(begin <= end);
        for (T* itr = begin; itr != end; ++itr) {
            insert(*itr);
        }
    }

    T8Less
    T&
    self::insert_unique(const T& val) {
        shared_rb_node<T> new_node{nullptr}, mark_node{root}, mark_node_parent{NIL};
        req(root != nullptr);

        while (mark_node != NIL && this->not_equal(mark_node->data, val)) {
            // I don't like this; updating metadata should be more visible & implicit.
            mark_node->size += 1;
            mark_node->sum += val;

            mark_node_parent = mark_node;
            mark_node = this->less(val, mark_node->data) ? mark_node->left : mark_node->right;
        }
        /* Don't insert key twice. */
        if (mark_node != NIL) {
            upwards_branch_metadata_refresh(mark_node_parent); // aka Undo changes.
            return mark_node->data;
        }

        new_node = std::make_shared< RB_Node<T> >(NIL, NIL, val);
        /* link parent <-> =new_node= */
        new_node->parent = mark_node_parent;
        if (mark_node_parent == NIL) {
            root = new_node;
        } else if (this->less(new_node->data, new_node->parent->data)) {
            new_node->parent->left = new_node;
        } else {
            new_node->parent->right = new_node;
        }
        /*hmm */
        upwards_branch_metadata_refresh(mark_node_parent);

        insert_fixup(new_node);
        return new_node->data;
    }


    T8Less
    T&
    self::insert(const T& val) {
        shared_rb_node<T> new_node, mark_node, mark_node_parent;
        new_node = std::make_shared< RB_Node<T> >(NIL, NIL, val);

        mark_node = root;
        mark_node_parent = NIL;
        while (mark_node != NIL) {
            mark_node->size += 1;
            mark_node->sum += val;

            mark_node_parent = mark_node;
            mark_node = this->less(new_node->data, mark_node->data) ? mark_node->left : mark_node->right;
        }
        /* link parent <-> =new_node= */
        new_node->parent = mark_node_parent;
        if (mark_node_parent == NIL) {
            root = new_node;
        } else if (this->less(new_node->data, new_node->parent->data)) {
            new_node->parent->left = new_node;
        } else {
            new_node->parent->right = new_node;
        }
        insert_fixup(new_node);
        return new_node->data;
    }

    T8Less
    void
    self::insert_fixup(shared_rb_node<T> inserted) {
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

    T8Less
    shared_rb_node<T>
    self::extract(const T& data) {
        return extract(search(data));
    }

    T8Less
    shared_rb_node<T>
    self::extract(shared_rb_node<T> extracted) {
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

    T8Less
    void
    self::extract_fixup(shared_rb_node<T> uplifted) {
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

    T8Less
    void
    self::rotate_metadata_fix(const shared_rb_node<T>& downlifted,
                              const shared_rb_node<T>& uplifted)
    {
        /* Order matters here because downlifted has outdated meta
         * and is a now a child of uplifted. So downlifted.update_meta first!!
         */
        downlifted->update_metadata();
        uplifted->update_metadata();
    }

    T8Less
    void
    self::upwards_branch_metadata_refresh(shared_rb_node<T> node) {
        /* 'til root refresh metadata. */
        for (; node != NIL; node = node->parent) {
            req(node != nullptr);
            node->update_metadata();
        }
    }

    T8Less
    unsigned
    self::size() const {
        req(root != nullptr, "[Debug]");
        return root->size;
    }
}

#undef self
#undef T8Less
#endif
