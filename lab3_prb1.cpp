#include "utils.hh"
#include "req.hh"

#include<cstdlib>
#include<fstream>
#include<iostream>
#include<list>
#include<memory>
#include<queue>
#include<vector>


class Graph {
private:
    std::vector< std::list<unsigned> > nodes;

public:
    /* Extract path from vector returned by Graph::path. */
    static std::list<unsigned> extract_path(const std::vector< std::pair<int, int> >&);

    /* Bidirectional search for path given the b&e edges. If it exists, return it. */
    std::vector< std::pair<int, int> > path(unsigned, unsigned);

    /* Turn graph from directed to undirected. */
    void undirect();

    /* Search for dipath given the b&e edges. If it exists, return it. */
    std::vector<int> dipath(unsigned, unsigned);

    /* I/O */
    friend std::istream& operator >> (std::istream&, Graph&);
    friend std::ostream& operator << (std::ostream&, const Graph&);
};

std::istream& operator >> (std::istream&, Graph&);
std::ostream& operator << (std::ostream&, const Graph&);

/* Test function for Graph::dipath. */
void test_dipath(const char *file_path);

/* Test function for Graph::path. */
void test_path(const char *file_path);

#ifdef _WIN32
    #define input_file "31.txt"
#else
    #define input_file "../input/31.txt"
#endif

int main()
{
    std::cout << "Find dipath.\n";
    test_dipath(input_file);

    nl;

    std::cout << "Find path.\n";
    test_path(input_file);
    return 0;
}


void test_path(const char *file_path)
{
    Graph g;
    unsigned i, j;
    std::vector< std::pair<int, int> > tched;

    std::ifstream in(file_path);
    req(in, "No input file!");

    /* Read graph. */
    in >> g;

    /* Transform graph. */
    g.undirect();

    /* Begin & End edges. */
    std::cout << "Begin: ";
    std::cin >> i;
    std::cout << "End: ";
    std::cin >> j;

    nl;

    /* Search graph. */
    tched = g.path(i, j);

    if (!tched.size()) {
        std::cout << "No path.\n";
    }
    else {
        std::list<unsigned> path = Graph::extract_path(tched);

        std::cout << "Path: ";
        for (const auto& node : path) {
            std::cout << node << ' ';
        }
        nl;
    }

    in.close();
}


/* Search upwards, or downwards. (downwards, first, second); (upwards, inv.). */
#define search_way(direction, walk_side, check_side)                    \
    if (!direction.empty()) {                                           \
        node = direction.front();                                       \
        direction.pop();                                                \
                                                                        \
        /* Copy the list of neighbors. */                               \
        neighbors = nodes[node];                                        \
                                                                        \
        /* Iterate over neighbors adding them to =fringe=.*/            \
        while (!neighbors.empty()) {                                    \
            neighbor = neighbors.front();                               \
            neighbors.pop_front();                                      \
                                                                        \
            req(neighbor < nodes.size());                               \
                                                                        \
            /* Control. */                                              \
            if (tched[neighbor].walk_side == -1) {                      \
                tched[neighbor].walk_side = node;                       \
                                                                        \
                /* Check if searches intersected. */                    \
                if (tched[neighbor].check_side != -1) {                 \
                    tched.push_back(                                    \
                            std::make_pair(neighbor, neighbor)          \
                            );                                          \
                    return tched;                                       \
                }                                                       \
                                                                        \
                direction.push(neighbor);                               \
            }                                                           \
        }                                                               \
    }

std::vector< std::pair<int, int> > Graph::path(unsigned i, unsigned j)
{
    req(i < nodes.size());
    req(j < nodes.size());

    std::vector< std::pair<int, int> > tched(nodes.size(), std::make_pair(-1, -1));

    /* Local variables. */
    unsigned node, neighbor;
    std::list<unsigned> neighbors;
    std::queue<unsigned> downward, upward;

    /* Push & touch i. */
    downward.push(i);
    tched[i].first = i;

    /* Push & touch j. */
    upward.push(j);
    tched[j].second = j;

    if (i == j) {
        tched[i].second = i;
        tched[j].first = j;

        tched.push_back(std::make_pair(i, i));
        return tched;
    }

    /* Search graph. */
    while (!downward.empty() || !upward.empty()) {
        search_way(downward, first, second);
        search_way(upward, second, first);
    }

    std::vector< std::pair<int, int> > empty;

    return empty;
}


std::list<unsigned> Graph::extract_path(const std::vector< std::pair<int, int> >& tched)
{
    std::list<unsigned> extracted_path;

    /* Local variables */
    unsigned intersection_index;
    intersection_index = tched[tched.size() - 1].first;

    /* Push point of intersection index. */
    extracted_path.push_front( intersection_index );

    /* Go from intersection to the left. */
    for (
        int index = intersection_index;
        (req(index >= 0), 1) && tched[index].first != index;
        index = tched[index].first
        )
    {
        extracted_path.push_front(tched[index].first);
    }

    /* Go from intersection to the right. */
    for (
        int index = intersection_index;
        (req(index >= 0), 1) && tched[index].second != index;
        index = tched[index].second
        )
    {
        extracted_path.push_back(tched[index].second);
    }

    return extracted_path;
}


void test_dipath(const char *file_path)
{
    Graph g;
    unsigned i, j;

    std::ifstream in(file_path);

    req(in, "No input file!");

    /* Read and print graph. */
    in >> g;

    /* Begin & End edges. */
    std::cout << "Begin: ";
    std::cin >> i;
    std::cout << "End: ";
    std::cin >> j;

    /* Get dipath if it exists. */
    std::vector<int> res = g.dipath(i, j);

    if (!res.size()) {
        std::cout << "No dipath.\n";
    }
    else {
        std::cout << "There's a path.\n";
    }

    map<int>(res, [&](int& node) {
            if (node != -1) {
                std::cout << node << ' ';
            }
            else {
                std::cout << "- ";
            }
            });
    nl;

    in.close();
}


std::vector<int> Graph::dipath(unsigned i, unsigned j)
{
    req(i < nodes.size());
    req(j < nodes.size());

    std::vector<int> tched(nodes.size(), -1);

    /* Local variables. */
    unsigned node, neighbor;
    std::list<unsigned> neighbors;
    std::queue<unsigned> fringe;

    /* Push & touch i. */
    fringe.push(i);
    tched[i] = i;

    if (i == j) {
        return tched;
    }

    while (!fringe.empty()) {
        node = fringe.front();
        fringe.pop();

        /* Copy the list of neighbors. */
        neighbors = nodes[node];

        /* Iterate over neighbors adding them to =fringe=.*/
        while (!neighbors.empty()) {
            neighbor = neighbors.front();
            neighbors.pop_front();

            req(neighbor < nodes.size());

            /* Touch & add to =fringe= the neighbor. */
            if (tched[neighbor] == -1) {
                tched[neighbor] = node;
                fringe.push(neighbor);
            }

            if (neighbor == j) {
                return tched;
            }
        }
    }

    std::vector<int> empty;

    return empty;
}

void Graph::undirect()
{
    /* Local variables. */
    unsigned neighbor;
    std::list<unsigned> neighbors;
    std::vector< std::list<unsigned> > aux;

    aux = nodes;
    for (unsigned node_index = 0; node_index < aux.size(); ++node_index) {
        /* Copy the list of neighbors. */
        neighbors = aux[node_index];

        /* Iterate over neighbors adding the opposite edge to the aux.*/
        while (!neighbors.empty()) {
            /* Get neighbor */
            neighbor = neighbors.front();
            neighbors.pop_front();

            req(neighbor < aux.size());

            /* Add opposite edge. */
            nodes[neighbor].push_front(node_index);
        }
    }
}


std::istream& operator >> (std::istream& in, Graph& g)
{
    int nedges;
    unsigned edge;

    nedges = 0;
    while (in >> nedges) {

        req(nedges >= 0);

        std::list<unsigned> l;
        while (nedges-- && in >> edge) {
            l.push_back(edge);
        }

        g.nodes.push_back(l);
    }

    return in;
}


std::ostream& operator << (std::ostream& out, const Graph& g)
{
    for (auto l : g.nodes) {
        if (l.empty()) {
            out << "()";
        }

        while (!l.empty()) {
            out << l.front() << ' ';
            l.pop_front();
        }
        out << '\n';
    }
    nl;

    return out;
}

