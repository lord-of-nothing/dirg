#include "editor.h"
#include "ui_editor.h"
#include <QLineEdit>
#include <QPushButton>
#include <QStackedLayout>
#include <QDoubleSpinBox>
#include <QVector>
#include <QComboBox>

#include "area.h"

QVector<QString> materials{"1", "2", "3"};

Editor::Editor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Editor)
{
    ui->setupUi(this);
    vtable = ui->vertexTable;
    etable = ui->edgeTable;

    vtable->setColumnCount(5);
    vtable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    etable->setColumnCount(2);
    etable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setupNew();
   }

void Editor::setupNew() {
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

}

void Editor::addVertex() {
    // vertex table
    int row = vtable->rowCount() - 1;

    QString name = static_cast<QLineEdit*>(vtable->cellWidget(row, 0))->text();
    if (name.isEmpty()) {
        return;
    }

    QLineEdit* nameEdit = new QLineEdit(this);
    nameEdit->insert(name);
    nameEdit->setEnabled(false);

    QDoubleSpinBox* xEdit = new QDoubleSpinBox(this);
    xEdit->setRange(minCoord, maxCoord);
    xEdit->setDecimals(precision);
    xEdit->setValue(static_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 1))->value());
    xEdit->setEnabled(false);

    QDoubleSpinBox* yEdit = new QDoubleSpinBox(this);
    yEdit->setRange(minCoord, maxCoord);
    yEdit->setDecimals(precision);
    yEdit->setValue(static_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 2))->value());
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
    etable->setCellWidget(row, 0, edgeNameEdit);
    QComboBox* materialCombo = new QComboBox(this);
    materialCombo->addItems(materials);
    etable->setCellWidget(row, 1, materialCombo);

    // buffer
    buffer.append(QVector2D(xEdit->value(), yEdit->value()));
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
    emit Mediator::instance()->bufferConnect(&buffer);
    // qWarning() << "sent";
}

Editor::~Editor()
{
    delete ui;
}
