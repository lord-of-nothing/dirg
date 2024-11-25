#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <string>
#include <vector>
#include <map>
#include <uuid/uuid.h>
class Vertex {
public:
    Vertex() = default;
    Vertex(double x, double y, std::string &name);

    std::string generator_id_vertex();

    double get_x();

    double get_y();

    std::string get_id();

    std::string get_name();

    std::vector<std::string> &get_edges();

    std::vector<std::string> &get_polygons();

    void add_edge(std::string edge_id);

    void remove_edge(std::string edge_id);

    void add_polygon(std::string polygon_id);

    void remove_polygon(std::string polygon_id);

    // void delete_vertex();

private:
    double x;
    double y;
    std::string id;
    std::string name;
    std::vector<std::string> polygons;
    std::vector<std::string> edges;
};

inline std::map<std::string, Vertex> all_vertices;
inline Vertex shite;

class Edge {
public:
    Edge() = default;
    Edge(Vertex &start, Vertex &finish, std::string &name, int property);

    std::string generator_id_edge();

    std::string get_id();

    std::string get_name();

    std::vector<std::string> &get_polygons();

    std::pair<Vertex &, Vertex &> get_coords();

    void add_polygon(std::string polygon_id);

    void remove_polygon(std::string polygon_id);

private:
    int property;
    std::string id;
    std::pair<Vertex &, Vertex &> coords{shite, shite};
    std::string name;
    std::vector<std::string> polygons;
};

inline std::map<std::string, Edge> all_edges;

class Polygon {
public:
    Polygon() = default;
    Polygon(std::vector<std::string> &vertices, std::vector<std::string> &edges, std::string &name, int material);

    std::string generator_id_polygon();

    std::string get_id();

    std::string get_name();

    std::vector<std::string> &get_vertices();

    std::vector<std::string> &get_edges();

    std::string next_vertex(std::string current_vertex);

    std::string next_edge(std::string current_edge);

    std::string previous_edge(std::string current_edge);

    std::string previous_vertex(std::string current_vertex);

    void delete_polygon();

    int get_polygon_number();

private:
    int polygon_number = 0;
    std::string id;
    std::string name;
    std::vector<std::string> edges;
    std::vector<std::string> vertices;
    int material;
};

inline std::map<std::string, Polygon> all_polygons;

#endif //MAINWINDOW_H
