#ifndef AREA_H
#define AREA_H

#include <QWidget>

class Mediator : public QObject {
    Q_OBJECT
public:
    static Mediator* instance() {
        static Mediator *mediatorInstance = new Mediator();
        return mediatorInstance;
    }

signals:
    void bufferConnect(QVector<QVector2D>* data);
private:
    Mediator() {}
};


class Area : public QWidget
{
    Q_OBJECT
public:
    explicit Area(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event);
private slots:
    void onBufferConnectReceived(QVector<QVector2D>* data);
private:
    QVector<QVector2D>* bufferData = nullptr;
};

#endif // AREA_H
