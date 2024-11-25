#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>
#include <vector>
#include <map>
#include <QUuid>

class Vertex {
public:
    Vertex() = default;
    Vertex(double x, double y, std::string name);

    QUuid generator_id_vertex();

    double get_x();

    double get_y();

    QUuid get_id();

    std::string get_name();

    std::vector<QUuid> &get_edges();

    std::vector<QUuid> &get_polygons();

    void add_edge(QUuid edge_id);

    void remove_edge(QUuid edge_id);

    void add_polygon(QUuid polygon_id);

    void remove_polygon(QUuid polygon_id);

    // void delete_vertex();

private:
    double x;
    double y;
    QUuid id;
    std::string name;
    std::vector<QUuid> polygons;
    std::vector<QUuid> edges;
};

inline std::map<QUuid, Vertex> all_vertices;
inline QUuid shite;

class Edge {
public:
    Edge() = default;
    Edge(QUuid start, QUuid finish, std::string name, int property);

    QUuid generator_id_edge();

    QUuid get_id();

    std::string get_name();

    std::vector<QUuid> &get_polygons();

    std::pair<QUuid, QUuid> get_coords();

    void add_polygon(QUuid polygon_id);

    void remove_polygon(QUuid polygon_id);

    int get_property();

private:
    int property;
    QUuid id;
    std::pair<QUuid, QUuid> coords{shite, shite};
    std::string name;
    std::vector<QUuid> polygons;
};

inline std::map<QUuid, Edge> all_edges;

class Polygon {
public:
    Polygon() = default;
    Polygon(std::vector<QUuid> &vertices, std::vector<QUuid> &edges, std::string name, int material,
            int existingNumber = -1, QUuid existingId = QUuid());

    QUuid generator_id_polygon();

    QUuid get_id();

    std::string get_name();

    std::vector<QUuid> &get_vertices();

    std::vector<QUuid> &get_edges();

    QUuid next_vertex(QUuid current_vertex);

    QUuid next_edge(QUuid current_edge);

    QUuid previous_edge(QUuid current_edge);

    QUuid previous_vertex(QUuid current_vertex);

    void delete_polygon();

    static int get_polygons_total();
    int get_number();

private:
    inline static int total_polygon_number = 0;
    int cur_polygon_number;
    QUuid id;
    std::string name;
    std::vector<QUuid> edges;
    std::vector<QUuid> vertices;
    int material;
};

inline std::map<QUuid, Polygon> all_polygons;

bool check_new_point(double first_x, double first_y, double second_x, double second_y, double third_x, double third_y);
bool check_convex(double first_x, double first_y, double second_x, double second_y, double third_x, double third_y);
double area(double first_x, double first_y, double second_x, double second_y, double third_x, double third_y);
bool intersect(double a, double b, double c, double d);
bool check_intersect(double first_x, double first_y, double second_x, double second_y, double third_x, double third_y, double fourth_x, double fourth_y);
bool point_in_polygon(double x, double y, QUuid polygon_id);
std::vector<QUuid> find_polygons_by_point(double x, double y);

#endif // GEOMETRY_H
