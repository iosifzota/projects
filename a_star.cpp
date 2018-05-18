#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <memory>
#include <list>
#include <queue>
#include <unordered_set>

#include "graph.hh"
#include "utils.hh"
#include "req.hh"
#include "priority_queue.hh"

struct Coordinates {
    double lat, lng;
    static double euclidian_distance(const Coordinates&, const Coordinates&);
};
/**/
std::ostream& operator << (std::ostream& out, const Coordinates& place);
std::istream& operator >> (std::istream& in, Coordinates& place);

struct Path {
    std::list<std::string> path;
    unsigned cost;

    Path(const std::list<std::string>& p, unsigned c)
        : path{p}, cost{c} {}
};
std::ostream& operator << (std::ostream& out, const Path& place);


struct City_priority {
    std::string city_name;
    double priority;
};
/* Defining std::greater of City_priority. */
namespace std {
    template<>
        class greater<City_priority>
        {
            public:
                bool operator () (const City_priority& lhs, const City_priority& rhs) {
                    return lhs.priority > rhs.priority;
                }
        };
}

/*
 * Returns a list containing path and the associated cost.
 * If there's no path nullptr is returned.
 */
std::shared_ptr<Path>
a_star_search(Graph<Coordinates, unsigned, std::string>& world_map,
        const std::string& start,
        const std::string& destination);
/*
 * Given the came_from map and the destination name, reconstruct the path. */
std::list<std::string>
    recreate_path(std::unordered_map<std::string, std::string>& visited, const std::string& destination);
/* The heuristic */
double aprox_cost(const Coordinates& lhs, const Coordinates& rhs);

void test_search();

int main()
{
    test_search();

    return 0;
}

#define test_true(start, destination) {                     \
    std::shared_ptr<Path> result;                           \
    req( (result = a_star_search(world_map, start, destination)) != nullptr); \
    std::cout << "From ";                                   \
    print_green(start);                                     \
    std::cout << " to ";                                    \
    print_green(destination);                               \
    std::cout << ":  " << *(result.get()) << " RON.\n";     \
}

#define test_false(start, destination)                              \
    req(a_star_search(world_map, start, destination) == nullptr);   \
    std::cout << "No path from ";                                   \
    print_green(start);                                             \
    std::cout << " to ";                                            \
    print_green(destination);                                       \
    std::cout << ".\n";

/* Initialize map. */
void read_world_map(Graph<Coordinates, unsigned, std::string>&);

void test_search()
{
    print_green("\n[Debug] test_search():\n");

    Graph<Coordinates, unsigned, std::string> world_map;

    read_world_map(world_map);

    test_true("Brasov", "Sofia");
    test_true("Brasov", "Brasov");
    test_true("Copenhaga", "Oslo");
    test_true("Minsk", "Praga");
    test_true("Chisinau", "Moscova");
    test_true("Varsovia", "Helsinki");
    test_true("Berlin", "Helsinki");
    test_true("Copenhaga", "Moscova");
    test_true("Copenhaga", "Chisinau");
    test_true("Londra", "Bagdad");
    test_false("Brasov", "Ierusalim");

    /* */
    print_green("\nExpanded test case:\n");

    std::cout << "(Sofia, Atena) = " << world_map.meta("Sofia", "Atena") << " RON *"; nl;
    std::cout << "(Sofia, Ankara) = " << world_map.meta("Sofia", "Ankara") << " RON *"; nll;

    std::cout << "(Atena, Nicosia) = " << world_map.meta("Atena", "Nicosia") << " RON"; nl;
    std::cout << "<Atena, Nicosia> = " << aprox_cost(world_map["Atena"], world_map["Nicosia"]) << " ron **"; nll;

    std::cout << "(Ankara, Nicosia) = " << world_map.meta("Ankara", "Nicosia") << " RON"; nl;
    std::cout << "<Ankara, Nicosia> = " << aprox_cost(world_map["Ankara"], world_map["Nicosia"]) << " ron **"; nll;

    test_true("Sofia", "Nicosia");
}


/* TODO: Declare world_map as const and add at operator for Graph. */
std::shared_ptr<Path> a_star_search(
        Graph<Coordinates, unsigned, std::string>& world_map,
        const std::string& start,
        const std::string& destination)
{
    priority_queue< City_priority, std::greater<City_priority> > fringe;
    std::unordered_map<std::string, std::string> came_from;
    std::unordered_map<std::string, unsigned> cost_so_far;

    req(world_map.has(start), "The map doesn't have =start= city.");
    req(world_map.has(destination), "The map doesn't have =destination= city.");

    fringe.push({ start, 0 });
    came_from[start] = start;
    cost_so_far[start] = 0;

    std::string current_city;

    while (!fringe.empty()) {

        current_city = fringe.top().city_name;
        fringe.pop();

        if (current_city == destination) {
            return std::make_shared<Path>(recreate_path(came_from, destination), cost_so_far[destination]);
        }

        /* For each neighbor ... */
        for (const auto& neighbor : world_map.neighbors(current_city)) {
            unsigned current_to_neighbor_cost, new_cost;
            std::string neighbor_name;

            /* Aliases. */
            neighbor_name = neighbor.first;
            current_to_neighbor_cost = neighbor.second;

            new_cost = cost_so_far[current_city] + current_to_neighbor_cost;

            if (cost_so_far.find(neighbor_name) == cost_so_far.end()
                    || new_cost < cost_so_far[neighbor_name])
            {
                double priority;

                cost_so_far[neighbor_name] = new_cost;

                priority = new_cost + aprox_cost(world_map[neighbor_name], world_map[destination]);
                fringe.push({ neighbor_name, priority });

                came_from[neighbor_name] = current_city;
            }

        }
    }

    return nullptr;
}

double aprox_cost(const Coordinates& lhs, const Coordinates& rhs)
{
    const double cost_per_degree = 109.435;
    double euclidian_distance;

    euclidian_distance = Coordinates::euclidian_distance(lhs, rhs);

    return euclidian_distance * cost_per_degree;
}

    std::list<std::string>
recreate_path(std::unordered_map<std::string, std::string>& visited, const std::string& destination)
{
    std::list<std::string> path;
    std::string current_city;

    current_city = destination;

    req(visited.find(current_city) != visited.end(), "recreate(): undefined key. [Debug]");

    while (current_city != visited[current_city]) {
        path.push_front(current_city);
        current_city = visited[current_city];
    }

    path.push_front(current_city);

    return path;
}


#define nodes_file_name "43_nodes.txt"
#define edges_file_name "43_edges.txt"
#define input_dir "../input/"

#if  defined (__GNUC__) || defined(__GNUG__)
#define nodes_file input_dir nodes_file_name
#define edges_file input_dir edges_file_name
#else
#define nodes_file nodes_file_name
#define edges_file edges_file_name
#endif

/* Generate input data. */
void generate_costs(Graph<Coordinates, unsigned, std::string>& world_map, const char *output_file);


void read_world_map(Graph<Coordinates, unsigned, std::string>& g)
{
    print_green("[Debug] Reading graph ...\n");

    std::cout << nodes_file << '\n'; //
    std::cout << edges_file << '\n';

    std::ifstream nodes_in(nodes_file);
    req(nodes_in, "Nodes file not found.");

    std::ifstream edges_in(edges_file);
    req(edges_in, "Edges file not found.");

    Graph<Coordinates, unsigned, std::string> temp;
    temp.read(nodes_in, edges_in);

#define generated_edges_file "delete_me___generated_edge_costs.txt"

    print_green("Generating edge costs ...\n");
    generate_costs(temp, generated_edges_file);

    /* Open the generated edge costs file. */
    edges_in.close();
    nodes_in.close();

    nodes_in.open(nodes_file);
    edges_in.open(generated_edges_file);

    /*
     * The side effect is here. */
    g.read(nodes_in, edges_in);

    print_green("[Debug] Read graph!\n\n");

    /* Cleanup */
    nodes_in.close();
    edges_in.close();
}

void generate_costs(Graph<Coordinates, unsigned, std::string>& world_map, const char *output_file)
{
    using city_to_neighbors = std::unordered_map< std::string, std::unordered_set<std::string> >;

    city_to_neighbors checked;
    std::queue<std::string> fringe;

    req(output_file != nullptr, "Null output_file str.");
    std::ofstream generated(output_file);
    req(generated, "Could not open output_file");

    req(!world_map.keys_empty(), "The map doesn't any cities.");
    req(!world_map.edges_empty(), "The map doesn't any connections.");

    fringe.push(world_map.keys_begin()->first);

    std::string current_city;

    while (!fringe.empty()) {

        current_city = fringe.front();
        fringe.pop();

        /* For each neighbor ... */
        for (const auto& neighbor : world_map.neighbors(current_city)) {
            /* Alias. */
            std::string neighbor_name = neighbor.first;

            if (
                    checked[current_city].find(neighbor_name) == checked[current_city].end() &&

                    ( checked.find(neighbor_name) == checked.end() ||
                      checked[neighbor_name].find(current_city) == checked[neighbor_name].end() )
               )
            {
#define ceiling(x) ((long long)(x) + 1)

                generated << current_city << ' '
                    << neighbor_name << ' '
                    << ceiling(aprox_cost(world_map[current_city], world_map[neighbor_name])) << '\n';

                checked[current_city].insert(neighbor_name);

                if (checked.find(neighbor_name) == checked.end()) {
                    fringe.push(neighbor_name);
                }
            }
        }
    }

    generated.close();
}


double Coordinates::euclidian_distance(const Coordinates& lhs, const Coordinates& rhs)
{
#define square(x) ((x)*(x))

    double delta_lat, delta_lng;

    delta_lat = abs(lhs.lat - rhs.lat);
    delta_lng = abs(lhs.lng - rhs.lng);

    return sqrt(square(delta_lat) + square(delta_lng));
}

std::ostream& operator << (std::ostream& out, const Coordinates& place)
{
    out << place.lat << ", " << place.lng;
    return out;
}

std::istream& operator >> (std::istream& in, Coordinates& place)
{
    if (in >> place.lat)
        req(in >> place.lng, "A place has both latitude and longitude.");

    return in;
}


/* Function on path. */
std::ostream& operator << (std::ostream& out, const Path& path)
{
    for (const auto& city : path.path) {
        out << city << ' ';
    }

    out << "- " << path.cost;
    return out;
}
