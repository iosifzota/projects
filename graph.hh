#ifndef __graph_hh
#define __graph_hh

#include <vector>
#include <ostream>
#include <istream>
#include <functional>
#include <unordered_map>
#include <string>

#include "req.hh"
#include "utils.hh"
#include "generic.hh"

#ifdef self
#undef self
#endif
#define self Graph<Node, Edge, Key, Hash, KeyEqual>

#ifdef Node8Edge8Key8Hash8KeyEqual
#undef Node8Edge8Key8Hash8KeyEqual
#endif
#define Node8Edge8Key8Hash8KeyEqual template <typename Node, typename Edge, typename Key, typename Hash, typename KeyEqual>

template <typename Node,
          typename Edge     = unsigned,
          typename Key      = char,
          typename Hash     = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>>
class Graph;

Node8Edge8Key8Hash8KeyEqual
std::ostream& operator << (std::ostream&, const Graph<Node, Edge, Key, Hash, KeyEqual>&);

Node8Edge8Key8Hash8KeyEqual
class Graph
{
    using Key_Node_map = std::unordered_map<Key, Node, Hash, KeyEqual>;
    using Key_Edge_map = std::unordered_map<Key, Edge, Hash, KeyEqual>;

    /*
        Too manny maps; map of Key => Node should be Key => Node, Key(edge).
        Keys will be stored once not 3 times; edges will be the up and coming SLOTS data structure.
        Items will be accessed with 2 ints: one for the slot_id, the other for the "time" stamp,
        so that if *item* at slot x was deleted and then another *item* was allotted that x slot,
        there will be no mistaking between the previous *item* and the current one.
        Got it? :))
        ZERO "time" stamp will indicate garbage.
        maybe slot_id should be also double folded: persistent slot_id, and effective slot_id to enable
        shrinking of the underling container whenever load ratio is significantly low.

        Problem:
        id props: identify a slot and item; knowing if slot position has changed, assert the new slot position O(1).  
    */

    Key_Node_map nodes;
    std::unordered_map<Key, Key_Edge_map, Hash, KeyEqual> edges;

public:
    Graph(){}
    /*
     * What i learned: declare one parameter contructors explicit to
     * prevent the use of the constructor for implict conversions.
     */
    explicit Graph(const std::unordered_map<Key, Key_Edge_map, Hash, KeyEqual>& input)
        : edges{input} {}
    void read(std::istream& nodes_in, std::istream& edges_in);

    /* Helpers for read. */
    void read_nodes(std::istream&);
    void read_edges(std::istream&);

    void print_nodes(std::ostream&) const;
    void print_edges(std::ostream&) const;

    typename Key_Node_map::const_iterator
    keys_begin() {
        return nodes.begin();
    }

    typename Key_Node_map::const_iterator
    keys_end() {
        return nodes.end();
    }

    bool keys_empty() {
        return nodes.empty();
    }

    typename std::unordered_map<Key, Key_Edge_map, Hash, KeyEqual>::const_iterator
    edges_begin() {
        return edges.begin();
    }

    typename std::unordered_map<Key, Key_Edge_map, Hash, KeyEqual>::const_iterator
    edges_end() {
        return edges.end();
    }

    bool edges_empty() {
        return edges.empty();
    }


    Key_Edge_map& neighbors(const Key& key) {
        req(nodes.find(key) != nodes.end(), "Undefined key.");
        return edges[key];
    }

    Edge& meta(const Key&, const Key&);
    inline Node& operator [] (const Key&);

    /* Check whether key is the map. */
    inline bool has(const Key& key) const;
    friend std::ostream& operator << <>(std::ostream&, const Graph<Node, Edge, Key, Hash, KeyEqual>&);
};

Node8Edge8Key8Hash8KeyEqual
Edge&
self::meta(const Key& begin_key, const Key& end_key)
{
    req(nodes.find(begin_key) != nodes.end(), "Edge has unknown beginning.");
    req(nodes.find(end_key) != nodes.end(), "Edge has unknown ending.");
    return edges[begin_key][end_key];
}

Node8Edge8Key8Hash8KeyEqual
std::ostream& operator << (std::ostream& out, const Graph<Node, Edge, Key, Hash, KeyEqual>& graph)
{
    graph.print_nodes(out);
    graph.print_edges(out);
    return out;
}

Node8Edge8Key8Hash8KeyEqual
void self::print_nodes(std::ostream& out) const
{
    for (const auto& node : nodes) {
        out << node.first << ": " << node.second << '\n';
    }
}

Node8Edge8Key8Hash8KeyEqual
void self::print_edges(std::ostream& out) const
{
    /* for each edge. */
    for (const auto& begin : edges) {
        out << begin.first << ":\n\t"; // print the beginning
        for (const auto& end : begin.second) {
            out << end.first << '(' << end.second << "), ";  // end and edge metadata.
        }
        out << '\n';
    }
}


Node8Edge8Key8Hash8KeyEqual
void self::read(std::istream& nodes_in, std::istream& edges_in)
{
    read_nodes(nodes_in);
    read_edges(edges_in);
}

Node8Edge8Key8Hash8KeyEqual
void self::read_nodes(std::istream& in)
{
    Key node_key;
    /* Read graph nodes. */
    while (in >> node_key) {
        req(in >> nodes[node_key], "Not enough input.");
    }
}


Node8Edge8Key8Hash8KeyEqual
void self::read_edges(std::istream& in)
{
    Key begin_key, end_key;
    /* Read graph nodes. */
    while (in >> begin_key) {
        req(nodes.find(begin_key) != nodes.end(), "Edge has unknown beginning.");

        req(in >> end_key, "Not enough input.");
        req(nodes.find(end_key) != nodes.end(), "Edge has unknown ending.");

        /* No need to define edges both ways. */
        req(edges[begin_key].find(end_key) == edges[begin_key].end(), "Redefining edge.");

        req(in >> edges[begin_key][end_key], "Not enough input.");
        edges[end_key][begin_key] = edges[begin_key][end_key];
    }
}

Node8Edge8Key8Hash8KeyEqual
bool self::has(const Key& key) const {
    return nodes.find(key) != nodes.end();
}

Node8Edge8Key8Hash8KeyEqual
Node& self::operator [] (const Key& node_key)
{
    return nodes[node_key];
}

#undef self
#undef Node8Edge8Key8Hash8KeyEqual
#endif