#include "area.h"
#include <QDebug>
#include <QDebug>
#include <QPainter>
#include <QVector2D>
#include <QPointF>

Area::Area(QWidget *parent)
    : QWidget{parent}
{
    connect(Mediator::instance(), &Mediator::bufferConnect, this, &Area::onBufferConnectReceived);
    // qWarning() << "constr";
}

void Area::paintEvent(QPaintEvent* event) {
    // qWarning() << "paint";
    if (bufferData == nullptr || bufferData->size() == 0) {
        return;
    }
    QPainter painter(this);
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Настройка кисти для рисования точек
    QBrush brush(Qt::blue, Qt::SolidPattern);
    painter.setBrush(brush);

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
