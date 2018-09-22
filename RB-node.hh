#ifndef __rb_node_hh
#define __rb_node_hh

#include <ostream>
#include <memory>
#include <cstring>

#ifdef self
#undef self
#endif
#define self RB_Node<T>

#ifdef T8
#undef T8
#endif
#define T8 template <typename T>

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
        using shared_node = shared_rb_node<T>;
        /* Construct */
        RB_Node();
        ~RB_Node();
        // parent [, data]
        RB_Node(const shared_node&, const T&);
        RB_Node(const shared_node&);
        // <, > [, data]
        RB_Node(const shared_node&, const shared_node&, const T&);
        RB_Node(const shared_node&, const shared_node&);

        /* Access */
        inline T& operator()();
        inline const T& operator()() const;

        /* Print */
        friend std::ostream& operator<< <>(std::ostream&, const RB_Node<T>&);
        friend std::ostream& operator<< <>(std::ostream&, const shared_node&);
        inline std::ostream& print_data(std::ostream&, const char *) const;

        /* Helpers for augumenting. */
        inline void update_size();
        inline void update_sum();
        inline void update_metadata();

        T data;
        shared_node left, right, parent;
        RB_color color;
        /* Extra */
        unsigned size;
        T sum;
    private:
        /* Used in `print_data` to avoid printing garbage values. */
        bool init;
    };

    T8
    self::RB_Node()
        :
        data{},
        left{},
        right{},
        parent{},
        color{ RED },
        size{},
        sum{},
        init{ false }
    { }

    T8
    self::~RB_Node()
    {
        parent = left = right = nullptr;
    }

    T8
    self::RB_Node(const shared_rb_node<T>& p, const T& val)
        :
        data{ val },
        left{},
        right{},
        parent{ p },
        color{ RED },
        size{ 1 },
        sum{ val },
        init{ true }
    { }

    T8
    self::RB_Node(const shared_rb_node<T>& p)
        :
        data{},
        left{},
        right{},
        parent{ p },
        color{ RED },
        size{},
        sum{},
        init{ false }
    { }


    T8
    self::RB_Node(const shared_rb_node<T>& l, const shared_rb_node<T>& r, const T& val)
        :
        data{ val },
        left{ l },
        right{ r },
        parent{},
        color{ RED },
        size{ 1 },
        sum{ val },
        init{ true }
    {
        if (l != nullptr && l->init == true) {
            ++size;
            sum += l->data;
        }
        if (r != nullptr && r->init == true) {
            ++size;
            sum += r->data;
        }
    }

    T8
    self::RB_Node(const shared_rb_node<T>& l, const shared_rb_node<T>& r)
        :
        data{},
        left{ l },
        right{ r },
        parent{},
        color{ RED },
        size{},
        sum{},
        init{ false }
    { }

    template<typename T>
    std::ostream&
    operator<<(std::ostream& out, const RB_Node<T>& n)
    {
        return n.print_data(out, "Node");
    }
    template<typename T>
    std::ostream&
    operator<<(std::ostream& out, const shared_rb_node<T>& n)
    {
        return (n != nullptr) ? out << *n : out << "Node: (null)\n";
    }

    T8
    std::ostream&
    self::print_data(std::ostream& out, const char *) const
    {
        return
            (!init)          ? out << "(uninitialized)" : out << data,
            (color == BLACK) ? out << "::B"             : out << "::R";
    }

    T8
    T&
    self::operator() ()
    {
        return data;
    }

    T8
    const T&
    self::operator() () const
    {
        return data;
    }

    /* Helpers for augumenting. */
    T8
    void
    self::update_size() {
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

    T8
    void
    self::update_sum() {
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

    T8
    void
    self::update_metadata() {
        update_size();
        update_sum();
    }
}

#undef self
#undef T8
#endif
