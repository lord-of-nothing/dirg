#include "edge.h"
#include "vertex.h"

QUuid Edge::gen_uuid() {
	QUuid uuid_str = QUuid::createUuid();
	while (all_edges.find(uuid_str) != all_edges.end()) {
		uuid_str = QUuid::createUuid();
	}
	return uuid_str;
}

Edge::Edge(const QUuid &start, const QUuid &finish, const QString &name,
		   int property)
	: property_(property), id_(gen_uuid()), name_(name),
	  coords_({start, finish}) {
	all_edges[id_] = *this;
	all_vertices[start].add_edge(id_);
	all_vertices[finish].add_edge(id_);
}
