// #include "MainWindow.h"
#include <iostream>
// #include <strings.h>
#include <string>
#include <uuid/uuid.h>
#include "geometry.h"

std::string Edge::generator_id_edge() {
    // uuid_t uuid;
    // char uuid_str[37];
    // uuid_generate(uuid);
    // uuid_unparse(uuid, uuid_str);
    std::string uuid_str;
    uuid_str = std::to_string(rand());
    while (all_edges.find(std::string(uuid_str)) != all_edges.end()) {
        // uuid_generate(uuid);
        // uuid_unparse(uuid, uuid_str);
        uuid_str = std::to_string(rand());
    }
    return std::string(uuid_str);
}

std::string Vertex::generator_id_vertex() {
    // uuid_t uuid;
    // char uuid_str[37];
    // uuid_generate(uuid);
    // uuid_unparse(uuid, uuid_str);
    std::string uuid_str;
    uuid_str = std::to_string(rand());
    while (all_vertices.find(std::string(uuid_str)) != all_vertices.end()) {
        // uuid_generate(uuid);
        // uuid_unparse(uuid, uuid_str);
        uuid_str = std::to_string(rand());
    }
    return std::string(uuid_str);
}

std::string Polygon::generator_id_polygon() {
    // uuid_t uuid;
    // char uuid_str[37];
    // uuid_generate(uuid);
    // uuid_unparse(uuid, uuid_str);
    std::string uuid_str;
    uuid_str = std::to_string(rand());
    while (all_polygons.find(std::string(uuid_str)) != all_polygons.end()) {
        // uuid_generate(uuid);
        // uuid_unparse(uuid, uuid_str);
        uuid_str = std::to_string(rand());

    }
    return std::string(uuid_str);
}


Vertex::Vertex(double x, double y, std::string &name) : x(x), y(y), name(name) {
    std::string id = generator_id_vertex();
    this->id = id;
    all_vertices[id] = *this;
}


double Vertex::get_x() {
    return x;
}

double Vertex::get_y() {
    return y;
}

std::string Vertex::get_id() {
    return id;
}

std::string Vertex::get_name() {
    return name;
}

std::vector<std::string> &Vertex::get_edges() {
    return edges;
}

std::vector<std::string> &Vertex::get_polygons() {
    return polygons;
}

void Vertex::add_edge(std::string edge_id) {
    edges.push_back(edge_id);
}

void Vertex::add_polygon(std::string polygon_id) {
    polygons.push_back(polygon_id);
}

//эта штука сейчас не нужна, мб потом пригодится
void Vertex::remove_edge(std::string edge_id) {
    for (size_t i = 0; i < edges.size(); i++) {
        if (edges[i] == edge_id) {
            edges.erase(edges.begin() + i);
            break;
        }
    }
}

//эта штука сейчас не нужна, мб потом пригодится
void Vertex::remove_polygon(std::string polygon_id) {
    for (size_t i = 0; i < polygons.size(); i++) {
        if (polygons[i] == polygon_id) {
            polygons.erase(polygons.begin() + i);
            break;
        }
    }
}

// void Vertex::delete_vertex() {
//     for (int i = 0; i < polygons.size(); ++i) {
//         int index = 0;
//         int count_vertices = all_polygons[polygons[i]].get_vertices().size();
//         for (int j = 0; j < count_vertices; j++) {
//             if (id == all_polygons[polygons[i]].get_vertices()[j]) {
//                 index = j;
//                 all_polygons[polygons[i]].get_vertices().erase(all_polygons[polygons[i]].get_vertices().begin() + j);
//                 break;
//             }
//         }
//         if (index > 0 && index <= count_vertices - 1) {
//             Edge new_edge = Edge(all_edges[all_polygons[polygons[i]].get_edges()[index - 1]].get_coords().first,
//                                  all_edges[all_polygons[polygons[i]].get_edges()[index]].get_coords().second, 0);
//             all_edges[new_edge.get_id()] = new_edge;
//             all_polygons[polygons[i]].get_edges()[index - 1] = new_edge.get_id();
//             all_polygons[polygons[i]].get_edges().erase(all_polygons[polygons[i]].get_edges().begin() + index);
//         } else if (index == 0) {
//             Edge new_edge = Edge(
//                 all_edges[all_polygons[polygons[i]].get_edges()[count_vertices - 1]].get_coords().first,
//                 all_edges[all_polygons[polygons[i]].get_edges()[0]].get_coords().second, 0);
//             all_edges[new_edge.get_id()] = new_edge;
//             all_polygons[polygons[i]].get_edges()[0] = new_edge.get_id();
//             all_polygons[polygons[i]].get_edges().pop_back();
//         }
//     }
//     all_vertices.erase(id);
// }

Edge::Edge(Vertex &start, Vertex &finish, std::string &name, int property) : property(property), coords({start, finish}), name(name) {
    std::string id = generator_id_edge();
    this->id = id;
    all_edges[id] = *this;
    all_vertices[start.get_id()].add_edge(id);
    all_vertices[finish.get_id()].add_edge(id);
}

std::string Edge::get_id() {
    return id;
}

std::string Edge::get_name() {
    return name;
}

std::vector<std::string> &Edge::get_polygons() {
    return polygons;
}

std::pair<Vertex &, Vertex &> Edge::get_coords() {
    return coords;
}

void Edge::add_polygon(std::string polygon_id) {
    polygons.push_back(polygon_id);
}

//эта штука сейчас не нужна, мб потом пригодится
void Edge::remove_polygon(std::string polygon_id) {
    for (size_t i = 0; i < polygons.size(); i++) {
        if (polygons[i] == polygon_id) {
            polygons.erase(polygons.begin() + i);
            break;
        }
    }
}

Polygon::Polygon(std::vector<std::string> &vertices, std::vector<std::string> &edges, std::string &name,
                 int material) : name(name), edges(edges), vertices(vertices), material(material) {
    std::string id = generator_id_polygon();
    this->id = id;
    all_polygons[id] = *this;

    for (int i = 0; i < edges.size(); ++i) {
        all_edges[edges[i]].add_polygon(id);
    }

    for (int i = 0; i < vertices.size(); ++i) {
        all_vertices[vertices[i]].add_polygon(id);
    }

    polygon_number++;
}

std::string Polygon::get_id() {
    return id;
}

std::string Polygon::get_name() {
    return name;
}

int Polygon::get_polygon_number() {
    return polygon_number;
}


std::vector<std::string> &Polygon::get_vertices() {
    return vertices;
}

std::vector<std::string> &Polygon::get_edges() {
    return edges;
}

std::string Polygon::next_edge(std::string current_edge) {
    int index = 0;
    for (int i = 0; i < edges.size(); ++i) {
        if (current_edge == edges[i]) {
            index = i;
        }
    }
    return edges[(index + 1) % edges.size()];

}

std::string Polygon::next_vertex(std::string current_vertex) {
    int index = 0;
    for (int i = 0; i < vertices.size(); ++i) {
        if (current_vertex == vertices[i]) {
            index = i;
        }
    }
    return vertices[(index + 1) % vertices.size()];

}

std::string Polygon::previous_edge(std::string current_edge) {
    int index = 0;
    for (int i = 0; i < edges.size(); ++i) {
        if (current_edge == edges[i]) {
            index = i;
        }
    }

    return edges[(index - 1 + edges.size()) % edges.size()];

}

std::string Polygon::previous_vertex(std::string current_vertex) {
    int index = 0;
    for (int i = 0; i < vertices.size(); ++i) {
        if (current_vertex == vertices[i]) {
            index = i;
        }
    }

    return vertices[(index - 1 + vertices.size()) % vertices.size()];

}

void Polygon::delete_polygon() {
    std::string polygon_id = id;
    for (int i = 0; i < vertices.size(); ++i) {
        // all_vertices[vertices[i]].remove_polygon(polygon_id);
        all_vertices.erase(vertices[i]);
    }

    for (int i = 0; i < edges.size(); ++i) {
        // all_edges[edges[i]].remove_polygon(polygon_id);
        all_edges.erase(edges[i]);
    }
    all_polygons.erase(polygon_id);
}
