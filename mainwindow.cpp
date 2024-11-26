#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "editor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->editorDock->setWidget(new Editor(ui->editorDock));
    ui->editorDock->close();
    ui->tree->setHeaderHidden(true);
    connect(ui->newPolygonBtn, &QPushButton::released, this, &MainWindow::newPolygon);
    // connect(ui->tree, &QTreeWidget::itemClicked, this, onIte);
    connect(ui->tree, &QTreeWidget::itemDoubleClicked, [this](QTreeWidgetItem *item, int column) {
        QUuid id = item->data(0, Qt::UserRole).value<QUuid>();
        selectPolygon(id);
    });
    connect(Mediator::instance(), &Mediator::onPolygonSave, this, [this] (Polygon* polygon, bool isNew) {
        if (!isNew) {
            removePolygon(polygon->get_id());
        }
        addPolygon(polygon);
    });
}


void MainWindow::newPolygon() {
    // if (ui->editorDock->isVisible()){
        // emit Mediator::instance()->onEditorReset();
    // }
    ui->editorDock->show();
}

void MainWindow::addPolygon(Polygon* polygon) {
    QTreeWidgetItem* polyItem = new QTreeWidgetItem(ui->tree);
    polyItem->setText(0, QString::fromStdString(polygon->get_name()));
    polyItem->setData(0, Qt::UserRole, QVariant::fromValue(polygon->get_id()));

    QTreeWidgetItem* vertexFolder = new QTreeWidgetItem(polyItem);
    vertexFolder->setText(0, "Vertices");
    QTreeWidgetItem* edgeFolder = new QTreeWidgetItem(polyItem);
    edgeFolder->setText(0, "Edges");

    for (auto& vertex : polygon->get_vertices()) {
        QTreeWidgetItem* v = new QTreeWidgetItem(vertexFolder);
        v->setText(0, QString::fromStdString(all_vertices[vertex].get_name()));
    }
    for (auto& edge : polygon->get_edges()) {
        QTreeWidgetItem* e = new QTreeWidgetItem(edgeFolder);
        e->setText(0, QString::fromStdString(all_edges[edge].get_name()));
    }
}


void MainWindow::removePolygon(QUuid id) {
    QVariant target = QVariant::fromValue(id);
    int count = ui->tree->topLevelItemCount();
    // Проходим по всем дочерним элементам первого уровня
    for (int i = 0; i < count; ++i) {
        QTreeWidgetItem *item = ui->tree->takeTopLevelItem(0);  // Убираем первый элемент
        if (item->data(0, Qt::UserRole) == target) {
            delete item; // Удаляем поддерево
            return; // После удаления выходим
        }
    }
}

void MainWindow::selectPolygon(QUuid id) {
    Polygon* polygon = &all_polygons[id];
     // if (ui->editorDock->isVisible()){
         // emit Mediator::instance()->onEditorReset();
    // }
    ui->editorDock->show();
    emit Mediator::instance()->onPolygonSelect(polygon);
}

MainWindow::~MainWindow()
{
    delete ui;
}
