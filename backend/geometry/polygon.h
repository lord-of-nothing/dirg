#ifndef POLYGON_H
#define POLYGON_H

#include <QUuid>
#include <QString>
#include <QVector>
#include <QPair>
#include <QHash>

class Polygon {
public:
	Polygon() = default;
	Polygon(const QVector<QUuid> &v, const QVector<QUuid> &e, const QString &name, const int material,
					const int existingNumber = -1, const QUuid &existingId = QUuid());

	static QUuid gen_uuid();
	inline auto id() const { return id_; }
	inline auto &name() const { return name_; }
	inline auto number() const { return cur_polygon_number_; }

	auto &next_vertex(const QUuid &current_vertex) const;
	auto &prev_vertex(const QUuid &current_vertex) const;

	auto &next_edge(const QUuid &current_edge) const;
	auto &prev_edge(const QUuid &current_edge) const;

	void delete_polygon();

	static inline int get_polygons_total() {
    return total_polygon_number;
	}

	QVector<QUuid> edges;
	QVector<QUuid> vertices;
private:
	static inline int total_polygon_number = 0;
	int cur_polygon_number_;
	QUuid id_;
	QString name_;
	int material_;
};

inline QHash<QUuid, Polygon> all_polygons;

#endif