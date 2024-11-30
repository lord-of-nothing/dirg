#include "edge.h"
#include "vertex.h"

QUuid Edge::gen_uuid() {
	QUuid uuid_str = uuid_str = QUuid::createUuid();
	while (all_edges.find(uuid_str) != all_edges.end()) {
			uuid_str = QUuid::createUuid();
	}
	return uuid_str;
}

Edge::Edge(const QUuid &start, const QUuid &finish, const QString &name, int property) : 
	property_(property), 
	coords_({start, finish}), 
	name_(name) {
		id_ = gen_uuid();
		all_edges[id_] = *this;
		all_vertices[start].add_edge(id_);
		all_vertices[finish].add_edge(id_);
}


