#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QShowEvent>
#include <QTableWidget>
#include <QPushButton>

namespace Ui {
class Editor;
}

class Editor : public QWidget
{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = nullptr);
    void setupNew();
    ~Editor();

protected:
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void updateTableSize();
    void addVertex();
    void saveVertex(int row);
    void finishEditVertex(int row);
    void editVertex(int row);
    void clearNew();
    Ui::Editor *ui;
    QTableWidget* vtable;
    QPushButton* cancelBtn = nullptr;; // костыль
    QTableWidget* etable;
    const double minCoord = 0.0;
    const double maxCoord = 1000.0;
    const int precision = 3;
};

#endif // EDITOR_H
