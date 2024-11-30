#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <map>
#include <QUuid>
#include <QString>
#include <QVector>
#include <QPair>
#include <QHash>

#include "vertex.h"

inline QUuid shite;

class Edge {
public:
    Edge() = default;
    Edge(QUuid start, QUuid finish, QString name, int property);

    QUuid generator_id_edge();

    QUuid get_id();

    QString get_name();

    QVector<QUuid> &get_polygons();

    QPair<QUuid, QUuid> get_coords();

    void add_polygon(QUuid polygon_id);

    void remove_polygon(QUuid polygon_id);

    int get_property();

private:
    int property;
    QUuid id;
    QPair<QUuid, QUuid> coords{shite, shite};
    QString name;
    QVector<QUuid> polygons;
};

inline QHash<QUuid, Edge> all_edges;

class Polygon {
public:
    Polygon() = default;
    Polygon(QVector<QUuid> &vertices, QVector<QUuid> &edges, QString name, int material,
            int existingNumber = -1, QUuid existingId = QUuid());

    QUuid generator_id_polygon();

    QUuid get_id();

    QString get_name();

    QVector<QUuid> &get_vertices();

    QVector<QUuid> &get_edges();

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
    QString name;
    QVector<QUuid> edges;
    QVector<QUuid> vertices;
    int material;
};

inline QHash<QUuid, Polygon> all_polygons;

bool check_new_point(double first_x, double first_y, double second_x, double second_y, double third_x, double third_y);
bool check_convex(double first_x, double first_y, double second_x, double second_y, double third_x, double third_y);
double area(double first_x, double first_y, double second_x, double second_y, double third_x, double third_y);
bool intersect(double a, double b, double c, double d);
bool check_intersect(double first_x, double first_y, double second_x, double second_y, double third_x, double third_y, double fourth_x, double fourth_y);
bool point_in_polygon(double x, double y, QUuid polygon_id);
QVector<QUuid> find_polygons_by_point(double x, double y);
double cross(QPair<double, double> first_vector, QPair<double, double> second_vector);
bool isConvex(QVector<QPair<double, double>> vertices);
bool isNotIntersecting(QVector<QPair<double, double>> vertices);
bool checkPolygon(QVector<QPair<double, double>> vertices);

#endif // GEOMETRY_H
