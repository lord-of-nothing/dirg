#include "geometry.h"

#include <QRandomGenerator>
#include <QUuid>

// bool check_convex(double first_x, double first_y, double second_x,
// 				  double second_y, double third_x, double third_y) {
// 	QPair<double, double> first_vector(second_x - first_x, second_y - first_y);
// 	QPair<double, double> second_vector(third_x - second_x, third_y - second_y);
// 	double mult = first_vector.first * second_vector.second -
// 				  second_vector.first * first_vector.second;
// 	return mult <= 0;
// }

double area(QPair<double, double> first_point, QPair<double, double> second_point,
			QPair<double, double> third_point) {
	double first_x = first_point.first;
	double first_y = first_point.second;

	double second_x = second_point.first;
	double second_y = second_point.second;

	double third_x = third_point.first;
	double third_y = third_point.second;

	return (second_x - first_x) * (third_y - first_y) -
		   (second_y - first_y) * (third_x - first_x);
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

bool check_intersect(QPair<double, double> first_point,
					 QPair<double, double> second_point,
					 QPair<double, double> third_point,
					 QPair<double, double> fourth_point) {
	double first_x = first_point.first;
	double first_y = first_point.second;

	double second_x = second_point.first;
	double second_y = second_point.second;

	double third_x = third_point.first;
	double third_y = third_point.second;

	double fourth_x = fourth_point.first;
	double fourth_y = fourth_point.second;
	return intersect(first_x, second_x, third_x, fourth_x) &&
		   intersect(first_y, second_y, third_y, fourth_y) &&
		   area(first_point, second_point, third_point) *
				   area(first_point, second_point, fourth_point) <= 0 &&
		   area(third_point, fourth_point, first_point) *
				   area(third_point, fourth_point, second_point) <= 0;
}

// bool check_new_point(double first_x, double first_y, double second_x,
// 					 double second_y, double third_x, double third_y) {
// 	bool first_check =
// 		check_convex(first_x, first_y, second_x, second_y, third_x, third_y);
// 	bool second_check = true;

// 	for (auto pair = all_edges.begin(); pair != all_edges.end(); ++pair) {
// 		double old_first_x = all_vertices[pair.value().coords().first].x();
// 		double old_first_y = all_vertices[pair.value().coords().first].y();
// 		double old_second_x = all_vertices[pair.value().coords().second].x();
// 		double old_second_y = all_vertices[pair.value().coords().second].y();
// 		if (check_intersect(old_first_x, old_first_y, old_second_x,
// 							old_second_y, second_x, second_y, third_x,
// 							third_y) == false) {
// 			second_check = false;
// 		}
// 	}

// 	return first_check && second_check;
// }

bool point_in_polygon(QPair<double, double> point, QUuid polygon_id) {
	double vector_x = 40000 + QRandomGenerator::global()->bounded(0, 2000);
	double vector_y = 40000 + QRandomGenerator::global()->bounded(0, 2000);
	int count_intersection = 0;
	for (auto &edge : all_polygons[polygon_id].edges) {
		if (check_intersect(
				point, {vector_x, vector_y},
				{all_vertices[all_edges[edge].coords().first].x(),
				 all_vertices[all_edges[edge].coords().first].y()},
				{all_vertices[all_edges[edge].coords().second].x(),
				 all_vertices[all_edges[edge].coords().second].y()})) {
			count_intersection++;
		}
	}
	return count_intersection % 2 == 1;
}

QVector<QUuid> find_polygons_by_point(QPair<double, double> point) {
	QVector<QUuid> polygons;

	for (auto polygon = all_polygons.begin(); polygon != all_polygons.end();
		 ++polygon) {
		if (point_in_polygon(point, polygon.key())) {
			polygons.append(polygon.key());
		}
	}
	return polygons;
}

double cross(QPair<double, double> first_vector,
			 QPair<double, double> second_vector) {
	return first_vector.first * second_vector.second -
		   second_vector.first * first_vector.second;
}

bool checkConvex(QVector<QPair<double, double>> vertices) {
	double init = cross({vertices[1].first - vertices[0].first,
						 vertices[1].second - vertices[0].second},
						{vertices[2].first - vertices[1].first,
						 vertices[2].second - vertices[1].second});
	for (int i = 1; i <= vertices.size(); i++) {
		double current_cross = cross(
			{vertices[i % vertices.size()].first - vertices[i - 1].first,
			 vertices[i % vertices.size()].second - vertices[i - 1].second},
			{vertices[(i + 1) % vertices.size()].first - vertices[i % vertices.size()].first,
			 vertices[(i + 1) % vertices.size()].second - vertices[i % vertices.size() ].second});
		if (init * current_cross <= 0) {
			return false;
		}
	}
	return true;
}

bool checkNotIntersecting(QVector<QPair<double, double>> vertices) {
	for (int i = 0; i < vertices.size(); i++) {
		for (auto pair = all_edges.begin(); pair != all_edges.end(); ++pair) {
			double old_first_x = all_vertices[pair.value().coords().first].x();
			double old_first_y = all_vertices[pair.value().coords().first].y();
			double old_second_x =
				all_vertices[pair.value().coords().second].x();
			double old_second_y =
				all_vertices[pair.value().coords().second].y();
			if (check_intersect({old_first_x, old_first_y}, {old_second_x, old_second_y},
								{vertices[i].first, vertices[i].second},
								{vertices[(i + 1) % vertices.size()].first, vertices[(i + 1) % vertices.size()].second})) {
				return false;
			}
		}
	}
	return true;
}

bool checkUniqueName(const QVector<QString>& names) {
	QSet<QString> temp;
	for (int i = 0; i < names.size(); ++i){
		temp.insert(names[i]);
	}
	if (temp.size() != names.size()) {
		return false;
	}
	for (int i = 0; i < names.size(); ++i) {
		if (all_names.contains(names[i])) {
			return false;
		}
	}
	return true;
}

bool checkNestingInOnceLayer(QVector<QPair<double, double>> vertices, int layer) {
	for (auto pair = all_polygons.begin(); pair != all_polygons.end(); ++pair) {
		if (pair.value().layer() == layer) {
			for (auto vertex : vertices) {
				if (point_in_polygon(vertex, pair.key())) {
					return false;
				}
			}
		}
	}
	return true;
}

// bool checkPolygon(QVector<QPair<double, double>> vertices) {
// 	return checkConvex(vertices) && checkNotIntersecting(vertices);
// }
