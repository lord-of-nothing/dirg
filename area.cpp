#include "area.h"
#include <QDebug>
#include <QDebug>
#include <QPainter>
#include <QVector2D>
#include <QPointF>
#include "geometry.h"

Area::Area(QWidget *parent)
    : QWidget{parent}
{
    connect(Mediator::instance(), &Mediator::bufferConnect, this, &Area::onBufferConnectReceived);
    // qWarning() << "constr";
}

void Area::paintEvent(QPaintEvent* event) {
    // qWarning() << "paint";
    QPainter painter(this);
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush(Qt::black, Qt::SolidPattern);
    QPen pen(Qt::black);

    // Настройка кисти для рисования точек
    painter.setBrush(Qt::black);
    painter.setPen(Qt::black);
    for (auto& [id, polygon] : all_polygons) {
        std::vector<QUuid>& vertices = polygon.get_vertices();
        qWarning() << vertices.size();
        for (int i = 0; i < vertices.size(); ++i) {
            Vertex& curV = all_vertices[vertices[i]];
            Vertex& nextV = all_vertices[vertices[(i + 1) % vertices.size()]];

            painter.drawEllipse(QPointF(curV.get_x(), curV.get_y()), 3.0, 3.0);
            painter.drawLine(QPointF(curV.get_x(), curV.get_y()), QPointF(nextV.get_x(), nextV.get_y()));
        }
    }
    if (bufferData == nullptr || bufferData->size() == 0) {
        return;
    }

    // brush.setColor(Qt::red);
    painter.setBrush(Qt::red);
    painter.setPen(Qt::red);

    // for (const QVector2D &point : *bufferData) {
    for (int i = 0; i < bufferData->size(); ++i) {
        QVector2D point = bufferData[0][i];
        QVector2D nextPoint;
        if (i < bufferData->size() - 1) {
            nextPoint = bufferData[0][i + 1];
        } else {
            nextPoint = bufferData[0][0];
        }
        painter.drawEllipse(QPointF(point.x(), point.y()), 3.0, 3.0);
        painter.drawLine(QPointF(point.x(), point.y()), QPointF(nextPoint.x(), nextPoint.y()));
    }
    painter.end();
}

void Area::onBufferConnectReceived(QVector<QVector2D>* data) {
    if (bufferData == nullptr) {
        bufferData = data;
    }
    repaint();
    // qWarning() << "signal";
}
