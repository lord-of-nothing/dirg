#include "geometry.h"
#include <QRandomGenerator>
#include <QUuid>

QUuid Edge::generator_id_edge() {
    QUuid uuid_str;
    uuid_str = QUuid::createUuid();
    while (all_edges.find(uuid_str) != all_edges.end()) {
        uuid_str = QUuid::createUuid();
    }
    return uuid_str;
}

QUuid Vertex::generator_id_vertex() {
    QUuid uuid_str;
    uuid_str = QUuid::createUuid();
    while (all_vertices.find(uuid_str) != all_vertices.end()) {
        uuid_str = QUuid::createUuid();
    }
    return uuid_str;
}

QUuid Polygon::generator_id_polygon() {
    QUuid uuid_str;
    uuid_str = QUuid::createUuid();
    while (all_polygons.find(uuid_str) != all_polygons.end()) {
        uuid_str = QUuid::createUuid();
    }
    return uuid_str;
}


Vertex::Vertex(double x, double y, QString name) : x(x), y(y), name(name) {
    QUuid id = generator_id_vertex();
    this->id = id;
    all_vertices[id] = *this;
}


double Vertex::get_x() {
    return x;
}

double Vertex::get_y() {
    return y;
}

QUuid Vertex::get_id() {
    return id;
}

QString Vertex::get_name() {
    return name;
}

QVector<QUuid> &Vertex::get_edges() {
    return edges;
}

QVector<QUuid> &Vertex::get_polygons() {
    return polygons;
}

void Vertex::add_edge(QUuid edge_id) {
    edges.append(edge_id);
}

void Vertex::add_polygon(QUuid polygon_id) {
    polygons.append(polygon_id);
}

//эта штука сейчас не нужна, мб потом пригодится
void Vertex::remove_edge(QUuid edge_id) {
    for (size_t i = 0; i < edges.size(); i++) {
        if (edges[i] == edge_id) {
            edges.erase(edges.begin() + i);
            break;
        }
    }
}

//эта штука сейчас не нужна, мб потом пригодится
void Vertex::remove_polygon(QUuid polygon_id) {
    for (size_t i = 0; i < polygons.size(); i++) {
        if (polygons[i] == polygon_id) {
            polygons.erase(polygons.begin() + i);
            break;
        }
    }
}

#if 0
void Vertex::delete_vertex() {
    for (int i = 0; i < polygons.size(); ++i) {
        int index = 0;
        int count_vertices = all_polygons[polygons[i]].get_vertices().size();
        for (int j = 0; j < count_vertices; j++) {
            if (id == all_polygons[polygons[i]].get_vertices()[j]) {
                index = j;
                all_polygons[polygons[i]].get_vertices().erase(all_polygons[polygons[i]].get_vertices().begin() + j);
                break;
            }
        }
        if (index > 0 && index <= count_vertices - 1) {
            Edge new_edge = Edge(all_edges[all_polygons[polygons[i]].get_edges()[index - 1]].get_coords().first,
                                 all_edges[all_polygons[polygons[i]].get_edges()[index]].get_coords().second, 0);
            all_edges[new_edge.get_id()] = new_edge;
            all_polygons[polygons[i]].get_edges()[index - 1] = new_edge.get_id();
            all_polygons[polygons[i]].get_edges().erase(all_polygons[polygons[i]].get_edges().begin() + index);
        } else if (index == 0) {
            Edge new_edge = Edge(
                all_edges[all_polygons[polygons[i]].get_edges()[count_vertices - 1]].get_coords().first,
                all_edges[all_polygons[polygons[i]].get_edges()[0]].get_coords().second, 0);
            all_edges[new_edge.get_id()] = new_edge;
            all_polygons[polygons[i]].get_edges()[0] = new_edge.get_id();
            all_polygons[polygons[i]].get_edges().pop_back();
        }
    }
    all_vertices.erase(id);
}
#endif

Edge::Edge(QUuid start, QUuid finish, QString name, int property) : property(property), coords({start, finish}), name(name) {
    QUuid id = generator_id_edge();
    this->id = id;
    all_edges[id] = *this;
    all_vertices[start].add_edge(id);
    all_vertices[finish].add_edge(id);
}

QUuid Edge::get_id() {
    return id;
}

QString Edge::get_name() {
    return name;
}

QVector<QUuid> &Edge::get_polygons() {
    return polygons;
}

QPair<QUuid, QUuid> Edge::get_coords() {
    return coords;
}

void Edge::add_polygon(QUuid polygon_id) {
    polygons.append(polygon_id);
}

int Edge::get_property() {
    return property;
}

//эта штука сейчас не нужна, мб потом пригодится
void Edge::remove_polygon(QUuid polygon_id) {
    for (size_t i = 0; i < polygons.size(); i++) {
        if (polygons[i] == polygon_id) {
            polygons.erase(polygons.begin() + i);
            break;
        }
    }
}

Polygon::Polygon(QVector<QUuid> &vertices, QVector<QUuid> &edges, QString name,
                 int material, int existingNumber, QUuid existingId) : name(name), edges(edges), vertices(vertices), material(material),
    cur_polygon_number(existingNumber) {
    if (existingId.isNull()){
        id = generator_id_polygon();
    } else {
        id = existingId;
    }
    all_polygons[id] = *this;

    for (int i = 0; i < edges.size(); ++i) {
        all_edges[edges[i]].add_polygon(id);
    }

    for (int i = 0; i < vertices.size(); ++i) {
        all_vertices[vertices[i]].add_polygon(id);
    }
    // this->cur_polygon_number = existingNumber;
    total_polygon_number++;
    // }
}

QUuid Polygon::get_id() {
    return id;
}

QString Polygon::get_name() {
    return name;
}

int Polygon::get_polygons_total() {
    return total_polygon_number;
}

int Polygon::get_number(){
    return cur_polygon_number;
}


QVector<QUuid> &Polygon::get_vertices() {
    return vertices;
}

QVector<QUuid> &Polygon::get_edges() {
    return edges;
}

QUuid Polygon::next_edge(QUuid current_edge) {
    int index = 0;
    for (int i = 0; i < edges.size(); ++i) {
        if (current_edge == edges[i]) {
            index = i;
        }
    }
    return edges[(index + 1) % edges.size()];

}

QUuid Polygon::next_vertex(QUuid current_vertex) {
    int index = 0;
    for (int i = 0; i < vertices.size(); ++i) {
        if (current_vertex == vertices[i]) {
            index = i;
        }
    }
    return vertices[(index + 1) % vertices.size()];

}

QUuid Polygon::previous_edge(QUuid current_edge) {
    int index = 0;
    for (int i = 0; i < edges.size(); ++i) {
        if (current_edge == edges[i]) {
            index = i;
        }
    }

    return edges[(index - 1 + edges.size()) % edges.size()];

}

QUuid Polygon::previous_vertex(QUuid current_vertex) {
    int index = 0;
    for (int i = 0; i < vertices.size(); ++i) {
        if (current_vertex == vertices[i]) {
            index = i;
        }
    }

    return vertices[(index - 1 + vertices.size()) % vertices.size()];

}

void Polygon::delete_polygon() {
    QUuid polygon_id = id;
    for (int i = 0; i < vertices.size(); ++i) {
        // all_vertices[vertices[i]].remove_polygon(polygon_id);
        all_vertices.remove(vertices[i]);
    }

    for (int i = 0; i < edges.size(); ++i) {
        // all_edges[edges[i]].remove_polygon(polygon_id);
        all_edges.remove(edges[i]);
    }
    all_polygons.remove(polygon_id);
}


bool check_convex(double first_x, double first_y, double second_x, double second_y, double third_x, double third_y) {
    QPair<double, double> first_vector(second_x - first_x, second_y - first_y);
    QPair<double, double> second_vector(third_x - second_x, third_y - second_y);
    double mult = first_vector.first * second_vector.second - second_vector.first * first_vector.second;
    return mult <= 0;
}

double area(double first_x, double first_y, double second_x, double second_y, double third_x, double third_y) {
    return (second_x - first_x) * (third_y - first_y) - (second_y - first_y) * (third_x - first_x);
}

bool intersect(double a, double b, double c, double d) {
    if (a > b) {
        std::swap(a, b);
    }
    if (c > d) {
        std::swap(c, d);
    }
    return std::max(a, c) <= std::min(b, d);
}

bool check_intersect(double first_x, double first_y, double second_x, double second_y, double third_x, double third_y, double fourth_x, double fourth_y) {
    return intersect(first_x, second_x, third_x, fourth_x)
           && intersect(first_y, second_y, third_y, fourth_y)
           && area(first_x, first_y, second_x, second_y, third_x, third_y) * area(first_x, first_y, second_x, second_y, fourth_x, fourth_y) <= 0
           && area(third_x, third_y, fourth_x, fourth_y, first_x, first_y) * area(third_x, third_y, fourth_x, fourth_y, second_x, second_y) <= 0;
}


bool check_new_point(double first_x, double first_y, double second_x, double second_y, double third_x, double third_y) {
    bool first_check = check_convex(first_x, first_y, second_x, second_y, third_x, third_y);
    bool second_check = true;

    // for (auto& pair : all_edges) {
    //     double old_first_x = all_vertices[pair.second.get_coords().first].get_x();
    //     double old_first_y = all_vertices[pair.second.get_coords().first].get_y();
    //     double old_second_x = all_vertices[pair.second.get_coords().second].get_x();
    //     double old_second_y = all_vertices[pair.second.get_coords().second].get_y();
    //     if (check_intersect(old_first_x, old_first_y, old_second_x, old_second_y, second_x, second_y, third_x, third_y) == false) {
    //         second_check = false;
    //     }
    // }

    for (auto pair = all_edges.begin(); pair != all_edges.end(); ++pair) {
        double old_first_x = all_vertices[pair.value().get_coords().first].get_x();
        double old_first_y = all_vertices[pair.value().get_coords().first].get_y();
        double old_second_x = all_vertices[pair.value().get_coords().second].get_x();
        double old_second_y = all_vertices[pair.value().get_coords().second].get_y();
        if (check_intersect(old_first_x, old_first_y, old_second_x, old_second_y, second_x, second_y, third_x, third_y) == false) {
            second_check = false;
        }
    }

    return first_check && second_check;
}

bool point_in_polygon(double x, double y, QUuid polygon_id){
    double vector_x = 40000 + QRandomGenerator::global()->bounded(0, 2000);
    double vector_y = 40000 + QRandomGenerator::global()->bounded(0, 2000);
    int count_intersection = 0;
    for (auto& edge : all_polygons[polygon_id].get_edges()) {
        if (check_intersect(x, y, vector_x, vector_y, all_vertices[all_edges[edge].get_coords().first].get_x(), all_vertices[all_edges[edge].get_coords().first].get_y(), all_vertices[all_edges[edge].get_coords().second].get_x(), all_vertices[all_edges[edge].get_coords().second].get_y())) {
            count_intersection++;
        }
    }
    return count_intersection % 2 == 1;
}

QVector<QUuid> find_polygons_by_point(double x, double y) {
    QVector<QUuid> polygons;
    // for (auto& polygon : all_polygons) {
    //     if (point_in_polygon(x, y, polygon.first)) {
    //         polygons.append(polygon.first);
    //     }
    // }

    for (auto polygon =  all_polygons.begin(); polygon !=  all_polygons.end(); ++polygon) {
        if (point_in_polygon(x, y, polygon.key())) {
            polygons.append(polygon.key());
        }
    }
    return polygons;
}

double cross(QPair<double, double> first_vector, QPair<double, double> second_vector) {
    double mult = first_vector.first * second_vector.second - second_vector.first * first_vector.second;
    return mult;
}

bool isConvex(QVector<QPair<double, double>> vertices) {
    double init = cross({vertices[1].first - vertices[0].first, vertices[1].second - vertices[0].second}, {vertices[2].first - vertices[1].first, vertices[2].second - vertices[1].second});
    for (int i = 1; i < vertices.size(); i++) {
        double current_cross = cross({vertices[i].first - vertices[i - 1].first, vertices[i].second - vertices[i - 1].second}, {vertices[(i + 1) % vertices.size()].first - vertices[i].first, vertices[(i + 1) % vertices.size()].second - vertices[i].second});
        if (init * current_cross < 0) {
            return false;
        }
    }
    return true;
}

bool isNotIntersecting(QVector<std::pair<double, double>> vertices) {
    for (int i = 0; i < vertices.size(); i++) {
        // for (auto& pair : all_edges) {
        //     double old_first_x = all_vertices[pair.second.get_coords().first].get_x();
        //     double old_first_y = all_vertices[pair.second.get_coords().first].get_y();
        //     double old_second_x = all_vertices[pair.second.get_coords().second].get_x();
        //     double old_second_y = all_vertices[pair.second.get_coords().second].get_y();
        //     if (check_intersect(old_first_x, old_first_y, old_second_x, old_second_y, vertices[i].first, vertices[i].second, vertices[(i + 1) % vertices.size()].first, vertices[(i + 1) % vertices.size()].second)) {
        //         return false;
        //     }
        // }

        for (auto pair = all_edges.begin(); pair != all_edges.end(); ++pair) {
            double old_first_x = all_vertices[pair.value().get_coords().first].get_x();
            double old_first_y = all_vertices[pair.value().get_coords().first].get_y();
            double old_second_x = all_vertices[pair.value().get_coords().second].get_x();
            double old_second_y = all_vertices[pair.value().get_coords().second].get_y();
            if (check_intersect(old_first_x, old_first_y, old_second_x, old_second_y, vertices[i].first, vertices[i].second, vertices[(i + 1) % vertices.size()].first, vertices[(i + 1) % vertices.size()].second)) {
                return false;
            }
        }
    }
    return true;
}


bool checkPolygon(QVector<QPair<double, double>> vertices) {
    bool firstFlag = isConvex(vertices);
    bool secondFlag = isNotIntersecting(vertices);
    return firstFlag && secondFlag;
}

