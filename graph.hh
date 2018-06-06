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


generic(N, Edge_meta = unsigned, Key = char, Hash = std::hash<Key>, Pred = std::equal_to<Key>)
class Graph;

generic(N, Edge_meta, Key, Hash, Pred)
std::ostream& operator << (std::ostream&, const Graph<N, Edge_meta, Key, Hash, Pred>&);

generic(N, Edge_meta, Key, Hash, Pred)
class Graph
{
private:
        /* Aliases. */
        typedef std::unordered_map<Key, N, Hash, Pred> Key_N_map;
        typedef std::unordered_map<Key, Edge_meta, Hash, Pred> Key_Edge_meta_map;

        Key_N_map nodes;
        std::unordered_map<Key, Key_Edge_meta_map, Hash, Pred> edges;

public:
        Graph(){}

        /*
         * What i learned: declare one parameter contructors explicit to
         * prevent the use of the constructor for implict conversions.
         */
        explicit Graph(const std::unordered_map<Key, Key_Edge_meta_map, Hash, Pred>& input)
                :edges{input} {}

        void read(std::istream& nodes_in, std::istream& edges_in);

        /* Helpers for read. */
        void read_nodes(std::istream&);
        void read_edges(std::istream&);

        void print_nodes(std::ostream&) const;
        void print_edges(std::ostream&) const;

        typename Key_N_map::const_iterator keys_begin() {
                return nodes.begin();
        }

        typename Key_N_map::const_iterator keys_end() {
                return nodes.end();
        }

        bool keys_empty() {
                return nodes.empty();
        }

        typename std::unordered_map<Key, Key_Edge_meta_map, Hash, Pred>::const_iterator edges_begin() {
                return edges.begin();
        }

        typename std::unordered_map<Key, Key_Edge_meta_map, Hash, Pred>::const_iterator edges_end() {
                return edges.end();
        }

        bool edges_empty() {
                return edges.empty();
        }


        Key_Edge_meta_map& neighbors(const Key& key) {
		req(nodes.find(key) != nodes.end(), "Undefined key.");
                return edges[key];
        }

	Edge_meta& meta(const Key&, const Key&);
        inline N& operator [] (const Key&);

        /* Check whether key is the map. */
        inline bool has(const Key& key) const;

        friend std::ostream& operator << <N, Edge_meta, Key, Hash, Pred>(std::ostream&, const Graph<N, Edge_meta, Key, Hash, Pred>&);
};

generic(N, Edge_meta, Key, Hash, Pred)
Edge_meta& Graph<N, Edge_meta, Key, Hash, Pred>::meta(const Key& begin_key, const Key& end_key)
{
	req(nodes.find(begin_key) != nodes.end(), "Edge has unknown beginning.");
	req(nodes.find(end_key) != nodes.end(), "Edge has unknown ending.");

	return edges[begin_key][end_key];
}

generic(N, Edge_meta, Key, Hash, Pred)
std::ostream& operator << (std::ostream& out, const Graph<N, Edge_meta, Key, Hash, Pred>& graph)
{
        graph.print_nodes(out);
        graph.print_edges(out);
        return out;
}

generic(N, Edge_meta, Key, Hash, Pred)
void Graph<N, Edge_meta, Key, Hash, Pred>::print_nodes(std::ostream& out) const
{
        for (const auto& node : nodes) {
                out << node.first << ": " << node.second << '\n';
        }
}

generic(N, Edge_meta, Key, Hash, Pred)
void Graph<N, Edge_meta, Key, Hash, Pred>::print_edges(std::ostream& out) const
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


generic(N, Edge_meta, Key, Hash, Pred)
void Graph<N, Edge_meta, Key, Hash, Pred>::read(std::istream& nodes_in, std::istream& edges_in)
{
        read_nodes(nodes_in);
        read_edges(edges_in);
}

generic(N, Edge_meta, Key, Hash, Pred)
void Graph<N, Edge_meta, Key, Hash, Pred>::read_nodes(std::istream& in)
{
        Key node_key;

        /* Read graph nodes. */
        while (in >> node_key) {
                req(in >> nodes[node_key], "Not enough input.");
        }
}


generic(N, Edge_meta, Key, Hash, Pred)
void Graph<N, Edge_meta, Key, Hash, Pred>::read_edges(std::istream& in)
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

generic(N, Edge_meta, Key, Hash, Pred)
bool Graph<N, Edge_meta, Key, Hash, Pred>::has(const Key& key) const {
        return nodes.find(key) != nodes.end();
}

generic(N, Edge_meta, Key, Hash, Pred)
N& Graph<N, Edge_meta, Key, Hash, Pred>::operator [] (const Key& node_key)
{
        return nodes[node_key];
}

#endif
