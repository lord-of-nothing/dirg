#include "geometry.h"
#include <QRandomGenerator>
#include <QUuid>

QUuid Polygon::generator_id_polygon() {
    QUuid uuid_str;
    uuid_str = QUuid::createUuid();
    while (all_polygons.find(uuid_str) != all_polygons.end()) {
        uuid_str = QUuid::createUuid();
    }
    return uuid_str;
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
    //     double old_first_x = all_vertices[pair.second.coords().first].x();
    //     double old_first_y = all_vertices[pair.second.coords().first].y();
    //     double old_second_x = all_vertices[pair.second.coords().second].x();
    //     double old_second_y = all_vertices[pair.second.coords().second].y();
    //     if (check_intersect(old_first_x, old_first_y, old_second_x, old_second_y, second_x, second_y, third_x, third_y) == false) {
    //         second_check = false;
    //     }
    // }

    for (auto pair = all_edges.begin(); pair != all_edges.end(); ++pair) {
        double old_first_x = all_vertices[pair.value().coords().first].x();
        double old_first_y = all_vertices[pair.value().coords().first].y();
        double old_second_x = all_vertices[pair.value().coords().second].x();
        double old_second_y = all_vertices[pair.value().coords().second].y();
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
        if (check_intersect(x, y, vector_x, vector_y, all_vertices[all_edges[edge].coords().first].x(), all_vertices[all_edges[edge].coords().first].y(), all_vertices[all_edges[edge].coords().second].x(), all_vertices[all_edges[edge].coords().second].y())) {
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
        //     double old_first_x = all_vertices[pair.second.coords().first].x();
        //     double old_first_y = all_vertices[pair.second.coords().first].y();
        //     double old_second_x = all_vertices[pair.second.coords().second].x();
        //     double old_second_y = all_vertices[pair.second.coords().second].y();
        //     if (check_intersect(old_first_x, old_first_y, old_second_x, old_second_y, vertices[i].first, vertices[i].second, vertices[(i + 1) % vertices.size()].first, vertices[(i + 1) % vertices.size()].second)) {
        //         return false;
        //     }
        // }

        for (auto pair = all_edges.begin(); pair != all_edges.end(); ++pair) {
            double old_first_x = all_vertices[pair.value().coords().first].x();
            double old_first_y = all_vertices[pair.value().coords().first].y();
            double old_second_x = all_vertices[pair.value().coords().second].x();
            double old_second_y = all_vertices[pair.value().coords().second].y();
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

