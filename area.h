#ifndef AREA_H
#define AREA_H

#include <QWidget>
#include "geometry.h"
#include <QMouseEvent>

class Mediator : public QObject {
    Q_OBJECT
public:
    static Mediator* instance() {
        static Mediator *mediatorInstance = new Mediator();
        return mediatorInstance;
    }

signals:
    void bufferConnect(QVector<QVector2D>* data, Polygon* editedP);
    void polygonSelect(Polygon* polygon);
    void polygonAdd(Polygon* polygon);
private:
    Mediator() {}
};


class Area : public QWidget
{
    Q_OBJECT
public:
    explicit Area(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private slots:
    void onBufferConnectReceived(QVector<QVector2D>* data, Polygon* editedP);
private:
    QVector<QVector2D>* bufferData = nullptr;
    Polygon* edited;
};

#endif // AREA_H
