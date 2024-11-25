#include "editor.h"
#include "ui_editor.h"
#include <QLineEdit>
#include <QPushButton>
#include <QStackedLayout>
#include <QDoubleSpinBox>
#include <QVector>
#include <QComboBox>

#include "area.h"
#include "geometry.h"

QVector<QString> materials{"1", "2", "3"};

Editor::Editor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Editor)
{
    ui->setupUi(this);
    dock = qobject_cast<QDockWidget*>(parentWidget());
    mainWindow = qobject_cast<MainWindow*>(dock->parentWidget());
    vtable = ui->vertexTable;
    etable = ui->edgeTable;

    vtable->setColumnCount(5);
    vtable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    etable->setColumnCount(2);
    etable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->confirmBtn, &QPushButton::released, this, &Editor::savePolygon);
    connect(ui->cancelBtn, &QPushButton::released, this, &Editor::resetEditor);

    // setupNew();
   // }

// void Editor::setupNew() {
    vtable->insertRow(0);
    QLineEdit* nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Name");
    vtable->setCellWidget(0, 0, nameEdit);

    QDoubleSpinBox* xEdit = new QDoubleSpinBox(this);
    xEdit->setRange(minCoord, maxCoord);
    xEdit->setDecimals(precision);
    vtable->setCellWidget(0, 1, xEdit);

    QDoubleSpinBox* yEdit = new QDoubleSpinBox(this);
    yEdit->setRange(minCoord, maxCoord);
    yEdit->setDecimals(precision);
    vtable->setCellWidget(0, 2, yEdit);

    QPushButton* addBtn = new QPushButton("+", this);
    connect(addBtn, &QPushButton::released, this, &Editor::addVertex);
    connect(addBtn, &QPushButton::released, this, &Editor::onBufferConnect);
    vtable->setCellWidget(0, 3, addBtn);
    polygonNumber = Polygon::get_polygons_total();
}

void Editor::addVertexRow(int row, QString vName, double x, double y) {
    // int row = vtable->rowCount() - 1;

    QLineEdit* nameEdit = new QLineEdit(this);
    nameEdit->insert(vName);
    nameEdit->setEnabled(false);

    QDoubleSpinBox* xEdit = new QDoubleSpinBox(this);
    xEdit->setRange(minCoord, maxCoord);
    xEdit->setDecimals(precision);
    xEdit->setValue(x);
    xEdit->setEnabled(false);

    QDoubleSpinBox* yEdit = new QDoubleSpinBox(this);
    yEdit->setRange(minCoord, maxCoord);
    yEdit->setDecimals(precision);
    yEdit->setValue(y);
    yEdit->setEnabled(false);

    vtable->insertRow(row);
    vtable->setCellWidget(row, 0, nameEdit);
    vtable->setCellWidget(row, 1, xEdit);
    vtable->setCellWidget(row, 2, yEdit);

    QPushButton* editBtn = new QPushButton("e", this);
    connect(editBtn, &QPushButton::released, this, [this, row](){editVertex(row);});
    QPushButton* saveBtn = new QPushButton("s", this);
    connect(saveBtn, &QPushButton::released, this, [this, row](){saveVertex(row);});
    connect(saveBtn, &QPushButton::released, this, &Editor::onBufferConnect);

    QWidget* editSaveStackW = new QWidget(this);
    QStackedLayout* editSaveStack = new QStackedLayout(editSaveStackW);
    editSaveStack->addWidget(editBtn);
    editSaveStack->addWidget(saveBtn);
    vtable->setCellWidget(row, 3, editSaveStackW);

    clearNew();

    // edge table
    etable->insertRow(row);
    QLineEdit* edgeNameEdit = new QLineEdit(this);
    edgeNameEdit->setPlaceholderText("Name");
    edgeNameEdit->setText("E" + QString::number(polygonNumber) + "_" + QString::number(row));
    etable->setCellWidget(row, 0, edgeNameEdit);
    QComboBox* materialCombo = new QComboBox(this);
    materialCombo->addItems(materials);
    etable->setCellWidget(row, 1, materialCombo);

    buffer.append(QVector2D(x, y));

}

void Editor::addVertex() {
    int row = vtable->rowCount() - 1;
    // vertex table

    QString name = static_cast<QLineEdit*>(vtable->cellWidget(row, 0))->text();
    if (name.isEmpty()) {
        // return;
        name = "V" + QString::number(polygonNumber) + "_" + QString::number(row);
    }

    double x = static_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 1))->value();
    double y = static_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 2))->value();

    addVertexRow(row, name, x, y);

  }

void Editor::editVertex(int row) {
    for (int i = 0; i < 3; ++i) {
        vtable->cellWidget(row, i)->setEnabled(true);
    }
    cancelBtn = new QPushButton("x", this);
    connect(cancelBtn, &QPushButton::released, this, [this, row](){resetVertex(row);});
    vtable->setCellWidget(row, 4, cancelBtn);

    QStackedLayout* layout = qobject_cast<QStackedLayout*>(vtable->cellWidget(row, 3)->layout());
    layout->setCurrentIndex(1);

    for (int i = 0; i < vtable->rowCount(); ++i) {
        if (i != row) {
            vtable->cellWidget(i, 3)->setEnabled(false);
        }
    }
}

void Editor::saveVertex(int row) {
    finishEditVertex(row);

    double x = qobject_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 1))->value();
    double y = qobject_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 2))->value();

    buffer[row] = QVector2D(x, y);
}

void Editor::resetVertex(int row) {
    finishEditVertex(row);

    QDoubleSpinBox* xEdit = qobject_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 1));
    xEdit->setValue(buffer[row][0]);
    QDoubleSpinBox* yEdit = qobject_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 2));
    yEdit->setValue(buffer[row][1]);
}

void Editor::finishEditVertex(int row) {
    for (int i = 0; i < 3; ++i) {
        vtable->cellWidget(row, i)->setEnabled(false);
    }
    vtable->removeCellWidget(row, 4);
    delete cancelBtn;
    cancelBtn = nullptr;
    QStackedLayout* layout = qobject_cast<QStackedLayout*>(vtable->cellWidget(row, 3)->layout());
    layout->setCurrentIndex(0);
    for (int i = 0; i < vtable->rowCount(); ++i) {
        vtable->cellWidget(i, 3)->setEnabled(true);
    }
}

void Editor::clearNew() {
    int row = vtable->rowCount() - 1;
    static_cast<QLineEdit*>(vtable->cellWidget(row, 0))->setText("");
    static_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 1))->setValue(0);
    static_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 2))->setValue(0);
}

void Editor::updateTableSize() {
    // vertex table
    auto tableWidth = vtable->width();
    for (auto col = 0; col < 3; ++col) {
        vtable->setColumnWidth(col, tableWidth * 0.3);
    }
    for (auto col = 3; col < 6; ++col) {
        vtable->setColumnWidth(col, tableWidth * 0.05);
    }

    // edge table
    tableWidth = etable->width();
    etable->setColumnWidth(0, tableWidth * 0.5);
    etable->setColumnWidth(1, tableWidth * 0.5);
}

void Editor::showEvent(QShowEvent *event) {
    updateTableSize();
}

void Editor::resizeEvent(QResizeEvent *event) {
    updateTableSize();
}

void Editor::onBufferConnect() {
    emit Mediator::instance()->bufferConnect(&buffer, editedPolygon);
    // qWarning() << "sent";
}

void Editor::savePolygon() {
    if (etable->rowCount() < 3) {
        return;
    }
    std::string name = "P" + QString::number(polygonNumber).toStdString();
    QUuid id;
    if (editedPolygon != nullptr) {
        name = editedPolygon->get_name();
        id = editedPolygon->get_id();
        editedPolygon->delete_polygon();
        editedPolygon = nullptr;
    }

    std::vector<QUuid> vertices;
    std::vector<QUuid> edges;

    // создаём точки
    for (int row = 0; row < etable->rowCount(); ++row) {
        std::string vname = qobject_cast<QLineEdit*>(vtable->cellWidget(row, 0))->text().toStdString();
        double x = qobject_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 1))->value();
        double y = qobject_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 2))->value();
        Vertex v(x, y, vname);
        vertices.push_back(v.get_id());
    }

    // создаём рёбра
    for (int row = 0; row < etable->rowCount(); ++row) {
        QString ename = qobject_cast<QLineEdit*>(etable->cellWidget(row, 0))->text();
        if (ename == "") {
            ename = "E" + QString::number(polygonNumber) + "_" + QString::number(row);
        }
        int property = qobject_cast<QComboBox*>(etable->cellWidget(row, 1))->currentText().toInt();
        Edge e(all_vertices[vertices[row]], all_vertices[vertices[(row + 1) % etable->rowCount()]],
                ename.toStdString(), property);
        edges.push_back(e.get_id());
    }

    // где-то здесь должна быть проверка корректности

    // создаём полигон
    Polygon* p = new Polygon(vertices, edges, name, 1, polygonNumber, id);

    // emit Mediator::instance()->polygonAdd(&p);
    if (!id.isNull()) {
        mainWindow->removePolygon(id);
    }
    mainWindow->addPolygon(p);
    resetEditor();
}

void Editor::resetEditor() {
    while (vtable->rowCount() > 1) {
        vtable->removeRow(0);
    }
    while (etable->rowCount() > 0) {
        etable->removeRow(0);
    }
    buffer.clear();
    polygonNumber = Polygon::get_polygons_total();
    clearNew();
    editedPolygon = nullptr;
    dock->close();
}

void Editor::onPolygonSelectReceived(Polygon* polygon) {
    dock->show();
    setupExistingPolygon(polygon);
}

void Editor::setupExistingPolygon(Polygon* polygon) {
    editedPolygon = polygon;
    polygonNumber = editedPolygon->get_number();
    for (auto& vId : polygon->get_vertices()) {
        Vertex& v = all_vertices[vId];
        QString vName = QString::fromStdString(v.get_name());
        double x = v.get_x();
        double y = v.get_y();
        addVertexRow(vtable->rowCount() - 1, vName, x, y);
    }

    for (auto& eId : polygon->get_edges()) {
        Edge& e = all_edges[eId];
        QString eName = QString::fromStdString(e.get_name());
        QString material = QString::number(e.get_property());

        qobject_cast<QLineEdit*>(etable->cellWidget(etable->rowCount() - 1, 0))->setText(eName);
        qobject_cast<QComboBox*>(etable->cellWidget(etable->rowCount() - 1, 1))->setCurrentText(material);
    }
}

Editor::~Editor()
{
    delete ui;
}
