#ifndef AREA_H
#define AREA_H

#include "geometry.h"
#include <QMouseEvent>
#include <QWidget>

class Mediator : public QObject {
	Q_OBJECT
public:
	static Mediator *instance() {
		static Mediator *mediatorInstance = new Mediator();
		return mediatorInstance;
	}

signals:
#if 0
    void bufferConnect(QVector<QVector2D>* data, Polygon* editedP);
    void polygonSelect(Polygon* polygon);
    void polygonAdd(Polygon* polygon);
#endif
	void onBufferConnect(QVector<QVector2D> *data, Polygon *editedP);
	void onPolygonSelect(Polygon *polygon);
	void onEditorReset();
	void onPolygonSave(Polygon *polygon, bool isNew);

	void addNewVertex(QPoint *point);
	void editVertexMouse(int row);
	void editVertexCoordMouse(int row, QPoint *new_coord);
	void saveVertexMouse(int row);

private:
	Mediator() {}
};

class Area : public QWidget {
	Q_OBJECT
public:
	explicit Area(QWidget *parent = nullptr);

protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
	void onBufferConnectReceived(QVector<QVector2D> *data, Polygon *editedP);

private:
	QVector<QVector2D> *bufferData = nullptr;
	Polygon *edited;

	constexpr static inline int select_radius = 10;
	int draggingVertex =
		-1;			   // Index of dragged vertex (-1 if no vertex is dragged)
	QPoint dragOffset; // Cursor offset relative to the vertex at the start of
					   // dragging
};

#endif // AREA_H
