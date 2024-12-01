#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QHash>
#include <QPair>
#include <QString>
#include <QUuid>
#include <QVector>
#include <map>

#include "vertex.h"
#include "edge.h"
#include "polygon.h"

bool check_new_point(double first_x, double first_y, double second_x,
					 double second_y, double third_x, double third_y);
bool check_convex(double first_x, double first_y, double second_x,
				  double second_y, double third_x, double third_y);
double area(double first_x, double first_y, double second_x, double second_y,
			double third_x, double third_y);
bool intersect(double a, double b, double c, double d);
bool check_intersect(double first_x, double first_y, double second_x,
					 double second_y, double third_x, double third_y,
					 double fourth_x, double fourth_y);
bool point_in_polygon(double x, double y, QUuid polygon_id);
QVector<QUuid> find_polygons_by_point(double x, double y);
double cross(QPair<double, double> first_vector,
			 QPair<double, double> second_vector);
bool isConvex(QVector<QPair<double, double>> vertices);
bool isNotIntersecting(QVector<QPair<double, double>> vertices);
bool checkPolygon(QVector<QPair<double, double>> vertices);

#endif
