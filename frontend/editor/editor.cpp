#include "editor.h"
#include "ui_editor.h"

#include "area.h"
// #include "geometry.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedLayout>
#include <QStringList>
#include <QVector>
#include <QMessageBox>

QVector<QString> properties{"1", "2", "3"};

Editor::Editor(QWidget *parent) : QWidget(parent), ui(new Ui::Editor) {
	// widget setup
	ui->setupUi(this);
	dock = qobject_cast<QDockWidget *>(parentWidget());
	vtable = ui->vertexTable;
	etable = ui->edgeTable;

	// polygon properties setup
	ui->polygonMaterial->addItems(properties);

	// vertexTable setUp
	vtable->setColumnCount(5);
	vtable->setHorizontalHeaderLabels(QStringList()
									  << "Name" << "X" << "Y" << "" << "");
	vtable->horizontalHeader()->setSectionsClickable(false);
	vtable->horizontalHeader()->setSectionsMovable(false);
	vtable->horizontalHeader()->setStyleSheet("QHeaderView::section {"
											  "    border: none;"
											  "}");
	vtable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	vtable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

	// edgeTable setUp
	etable->setColumnCount(4);
	etable->setHorizontalHeaderLabels(QStringList() << "Name" << "Property" << "" << "");
	etable->horizontalHeader()->setSectionsClickable(false);
	etable->horizontalHeader()->setSectionsMovable(false);
	etable->horizontalHeader()->setStyleSheet("QHeaderView::section {"
											  "    border: none;"
											  "}");
	etable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	etable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

	connect(ui->confirmBtn, &QPushButton::released, this, &Editor::savePolygon);
	connect(ui->cancelBtn, &QPushButton::released, this, &Editor::resetEditor);

	setStyleSheet("QDoubleSpinBox::up-button { width: 0; height: 0; }"
				  "QDoubleSpinBox::down-button { width: 0; height: 0; }"
				  "QDoubleSpinBox { border: 1px solid gray; border-radius: "
				  "4px; padding: 2px 4px; }");

	// vertex table empty row setup
	vtable->insertRow(0);
	QLineEdit *nameEdit = new QLineEdit(this);
	nameEdit->setPlaceholderText("V" + QString::number(polygonNumber) + "_" +
								 QString::number(0));
	// nameEdit->setPlaceholderText("Name");
	vtable->setCellWidget(0, 0, nameEdit);

	QDoubleSpinBox *xEdit = new QDoubleSpinBox(this);
	xEdit->setRange(minCoord, maxCoord);
	xEdit->setDecimals(precision);
	vtable->setCellWidget(0, 1, xEdit);

	QDoubleSpinBox *yEdit = new QDoubleSpinBox(this);
	yEdit->setRange(minCoord, maxCoord);
	yEdit->setDecimals(precision);
	vtable->setCellWidget(0, 2, yEdit);

	QPushButton *addBtn = new QPushButton(this);
	connect(addBtn, &QPushButton::released, this, &Editor::addVertex);
	connect(addBtn, &QPushButton::released, this, &Editor::onBufferConnect);
	vtable->setCellWidget(0, 3, addBtn);
	polygonNumber = Polygon::get_polygons_total();
	addBtn->setIcon(addIcon);

	// opening editor on polygon selection
	connect(Mediator::instance(), &Mediator::onPolygonSelect, this,
			&Editor::onPolygonSelectReceived);
	connect(Mediator::instance(), &Mediator::onEditorReset, this,
			&Editor::resetEditor);
	connect(Mediator::instance(), &Mediator::onVertexSelect, this,
			&Editor::onVertexSelectReceived);
	connect(Mediator::instance(), &Mediator::onEdgeSelect, this,
			&Editor::onEdgeSelectReceived);

	// testing
	connect(ui->loadBtn, &QPushButton::released, this, &Editor::load);

	// Add Vertext by mouse click
	// connect(Mediator::instance(), &Mediator::addNewVertex, this,
	// 		&Editor::addVertexByMouse);

	// // Dragging Vertex
	// connect(Mediator::instance(), &Mediator::editVertexMouse, this,
	// 		&Editor::editVertex);
	// connect(Mediator::instance(), &Mediator::editVertexCoordMouse, this,
	// 		&Editor::editVertexCoord);
	// connect(Mediator::instance(), &Mediator::saveVertexMouse, this,
	// 		&Editor::saveVertex);
	// connect(Mediator::instance(), &Mediator::saveVertexMouse, this,
	// 		&Editor::onBufferConnect);
}

// Slot for create two testing polygons
void Editor::load() {
	// testing polygon
	// triangle
	resetEditor();
	addVRow(0, "V" + QString::number(polygonNumber) + "_0", 300.0, 0.0);
	addVRow(1, "V" + QString::number(polygonNumber) + "_1", 700.0, 500.0);
	addVRow(2, "V" + QString::number(polygonNumber) + "_2", 900.0, 200.0);
	savePolygon();
	// square
	// addVRow(0, "V" + QString::number(polygonNumber) + "_0", 100.0, 100.0);
	// addVRow(1, "V" + QString::number(polygonNumber) + "_1", 100.0, 400.0);
	// addVRow(2, "V" + QString::number(polygonNumber) + "_2", 400.0, 400.0);
	// addVRow(3, "V" + QString::number(polygonNumber) + "_3", 400.0, 100.0);
	// savePolygon();
	ui->loadBtn->hide();
}

// Add Row for Vertex and Edge table
void Editor::addVRow(int row, QString vName, double x, double y) {
	// int row = vtable->rowCount() - 1;

	QLineEdit *nameEdit = new QLineEdit(this);
	nameEdit->insert(vName);
	nameEdit->setPlaceholderText(
		"V" + QString::number(polygonNumber) + "_" +
		QString::number(row)); // Зачем нужна эта строчка, перед ней сразу
							   // заполняется текст?
	nameEdit->setEnabled(false);

	QDoubleSpinBox *xEdit = new QDoubleSpinBox(this);
	xEdit->setRange(minCoord, maxCoord);
	xEdit->setDecimals(precision);
	xEdit->setValue(x);
	xEdit->setEnabled(false);

	QDoubleSpinBox *yEdit = new QDoubleSpinBox(this);
	yEdit->setRange(minCoord, maxCoord);
	yEdit->setDecimals(precision);
	yEdit->setValue(y);
	yEdit->setEnabled(false);

	vtable->insertRow(row);
	vtable->setCellWidget(row, 0, nameEdit);
	vtable->setCellWidget(row, 1, xEdit);
	vtable->setCellWidget(row, 2, yEdit);

	QPushButton *editBtn = new QPushButton(this);
	connect(editBtn, &QPushButton::released, this,
			[this, row]() { editVertex(row); });
	editBtn->setIcon(editIcon);
	QPushButton *saveBtn = new QPushButton(this);
	saveBtn->setIcon(saveIcon);
	connect(saveBtn, &QPushButton::released, this,
			[this, row]() { saveVertex(row); });
	connect(saveBtn, &QPushButton::released, this, &Editor::onBufferConnect);

	QWidget *editSaveStackW = new QWidget(this);
	QStackedLayout *editSaveStack = new QStackedLayout(editSaveStackW);
	editSaveStack->addWidget(editBtn);
	editSaveStack->addWidget(saveBtn);
	vtable->setCellWidget(row, 3, editSaveStackW);

	clearNew();

	// edge table
	etable->insertRow(row);
	QLineEdit *edgeNameEdit = new QLineEdit(this);
	QString defaultEdgeName =
		"E" + QString::number(polygonNumber) + "_" + QString::number(row);
	edgeNameEdit->setPlaceholderText(
		defaultEdgeName); // Зачем нужна эта строчка, дальше идет сразу
						  // заполняется текст?
	edgeNameEdit->setText(defaultEdgeName);
	etable->setCellWidget(row, 0, edgeNameEdit);
	edgeNameEdit->setEnabled(false);


	QComboBox *materialCombo = new QComboBox(this);
	materialCombo->addItems(properties);
	etable->setCellWidget(row, 1, materialCombo);
	materialCombo->setEnabled(false);

	QPushButton *edgeEditBtn = new QPushButton(this);
	connect(edgeEditBtn, &QPushButton::released, this,
			[this, row]() { editEdge(row); });
	edgeEditBtn->setIcon(editIcon);
	QPushButton *edgeSaveBtn = new QPushButton(this);
	edgeSaveBtn->setIcon(saveIcon);
	connect(edgeSaveBtn, &QPushButton::released, this,
			[this, row]() { saveEdge(row); });

	QWidget *edgeEditSaveStackW = new QWidget(this);
	QStackedLayout *edgeEditSaveStack = new QStackedLayout(edgeEditSaveStackW);
	edgeEditSaveStack->addWidget(edgeEditBtn);
	edgeEditSaveStack->addWidget(edgeSaveBtn);
	etable->setCellWidget(row, 2, edgeEditSaveStackW);

	buffer.append(QVector2D(x, y));
}

void Editor::editEdge(int row) {
	for (int i = 0; i < 2; ++i) {
		etable->cellWidget(row, i)->setEnabled(true);
	}
	vtable->setEnabled(false);

	QPushButton* cancelBtn = new QPushButton(this);
	cancelBtn->setIcon(resetIcon);
	connect(cancelBtn, &QPushButton::released, this, [this, row]() { resetEdge(row); });
	etable->setCellWidget(row, 3, cancelBtn);

	QStackedLayout *layout =
		qobject_cast<QStackedLayout*>(etable->cellWidget(row, 2)->layout());
	layout->setCurrentIndex(1);

	for (int i = 0; i < etable->rowCount(); ++i) {
		if (i != row) {
			etable->cellWidget(i, 2)->setEnabled(false);
		}
	}

	editedEdgeName = qobject_cast<QLineEdit*>(etable->cellWidget(row, 0))->text();
	editedEdgeProperty = qobject_cast<QComboBox*>(etable->cellWidget(row, 1))->currentText();


	QVector2D first_vertex = buffer[row];
	QVector2D second_vertex = buffer[(row + 1) % buffer.size()];
	QLineF edge_line({first_vertex[0], first_vertex[1]}, {second_vertex[0], second_vertex[1]});
	emit Mediator::instance()->onLineHighlight(edge_line);
}

void Editor::saveEdge(int row) {
	finishEditEdge(row);

	QLineEdit* nameEdit = qobject_cast<QLineEdit*>(etable->cellWidget(row, 0));
	if (nameEdit->text().isEmpty()) {
		nameEdit->setText(nameEdit->placeholderText());
	}
}

void Editor::resetEdge(int row) {
	finishEditEdge(row);

	qobject_cast<QLineEdit*>(etable->cellWidget(row, 0))->setText(editedEdgeName);
	qobject_cast<QComboBox*>(etable->cellWidget(row, 1))->setCurrentText(editedEdgeProperty);
}

void Editor::finishEditEdge(int row) {
	for (int i = 0; i < 2; ++i) {
		etable->cellWidget(row, i)->setEnabled(false);
	}
	vtable->setEnabled(true);

	etable->removeCellWidget(row, 3);
	QStackedLayout *layout =
		qobject_cast<QStackedLayout*>(etable->cellWidget(row, 2)->layout());
	layout->setCurrentIndex(0);
	for (int i = 0; i < etable->rowCount(); ++i) {
		etable->cellWidget(i, 2)->setEnabled(true);
	}


	emit Mediator::instance()->onHighlightReset();
}

// Slot for add Vertex
void Editor::addVertex() {
	int row = vtable->rowCount() - 1;
	// vertex table

	QString name = static_cast<QLineEdit *>(vtable->cellWidget(row, 0))->text();
	if (name.isEmpty()) {
		// return;
		name =
			"V" + QString::number(polygonNumber) + "_" + QString::number(row);
	}

	double x =
		static_cast<QDoubleSpinBox *>(vtable->cellWidget(row, 1))->value();
	double y =
		static_cast<QDoubleSpinBox *>(vtable->cellWidget(row, 2))->value();

	// addVertexRow(row, name, x, y);
	addVRow(row, name, x, y);
}

// Change addMode to editMode
void Editor::editVertex(int row) {
	for (int i = 0; i < 3; ++i) {
		vtable->cellWidget(row, i)->setEnabled(true);
	}
	etable->setEnabled(false);

	QPushButton* cancelBtn = new QPushButton(this);
	cancelBtn->setIcon(resetIcon);
	connect(cancelBtn, &QPushButton::released, this,
			[this, row]() { resetVertex(row); });
	vtable->setCellWidget(row, 4, cancelBtn);

	QStackedLayout *layout =
		qobject_cast<QStackedLayout *>(vtable->cellWidget(row, 3)->layout());
	layout->setCurrentIndex(1);

	for (int i = 0; i < vtable->rowCount(); ++i) {
		if (i != row) {
			vtable->cellWidget(i, 3)->setEnabled(false);
		}
	}

	double x = qobject_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 1))->value();
	double y = qobject_cast<QDoubleSpinBox*>(vtable->cellWidget(row, 2))->value();
	emit Mediator::instance()->onPointHighlight(QPointF(x, y));
}

void Editor::saveVertex(int row) {
	finishEditVertex(row);

	double x =
		qobject_cast<QDoubleSpinBox *>(vtable->cellWidget(row, 1))->value();
	double y =
		qobject_cast<QDoubleSpinBox *>(vtable->cellWidget(row, 2))->value();

	buffer[row] = QVector2D(x, y);
}

void Editor::resetVertex(int row) {
	finishEditVertex(row);

	QDoubleSpinBox *xEdit =
		qobject_cast<QDoubleSpinBox *>(vtable->cellWidget(row, 1));
	xEdit->setValue(buffer[row][0]);
	QDoubleSpinBox *yEdit =
		qobject_cast<QDoubleSpinBox *>(vtable->cellWidget(row, 2));
	yEdit->setValue(buffer[row][1]);
}

void Editor::finishEditVertex(int row) {
	for (int i = 0; i < 3; ++i) {
		vtable->cellWidget(row, i)->setEnabled(false);
	}
	etable->setEnabled(true);

	vtable->removeCellWidget(row, 4);
	QStackedLayout *layout =
		qobject_cast<QStackedLayout *>(vtable->cellWidget(row, 3)->layout());
	layout->setCurrentIndex(0);
	for (int i = 0; i < vtable->rowCount(); ++i) {
		vtable->cellWidget(i, 3)->setEnabled(true);
	}
	emit Mediator::instance()->onHighlightReset();
}

void Editor::clearNew() {
	int row = vtable->rowCount() - 1;
	QLineEdit *nameEdit = static_cast<QLineEdit *>(vtable->cellWidget(row, 0));
	nameEdit->setText("");
	nameEdit->setPlaceholderText("V" + QString::number(polygonNumber) + "_" +
								 QString::number(row));
	static_cast<QDoubleSpinBox *>(vtable->cellWidget(row, 1))->setValue(0);
	static_cast<QDoubleSpinBox *>(vtable->cellWidget(row, 2))->setValue(0);
}

void Editor::updateTableSize() {
	// vertex table
	auto tableWidth = vtable->width();
	for (auto col = 0; col < 3; ++col) {
		vtable->setColumnWidth(col, tableWidth * 0.3);
	}
	for (auto col = 3; col < 6; ++col) {
		vtable->setColumnWidth(col, tableWidth * 0.04);
	}

	// edge table
	tableWidth = etable->width();
	for (auto col = 0; col < 2; ++col) {
		etable->setColumnWidth(col, tableWidth * 0.4);
	}
	for (auto col = 2; col < 4; ++col) {
		etable->setColumnWidth(col, tableWidth * 0.08);
	}
}

void Editor::showEvent([[maybe_unused]] QShowEvent *event) {
	updateTableSize();
}

void Editor::resizeEvent([[maybe_unused]] QResizeEvent *event) {
	updateTableSize();
}

void Editor::onBufferConnect() {
	// emit Mediator::instance()->bufferConnect(&buffer, editedPolygon);
	// qWarning() << "sent";
	emit Mediator::instance() -> onBufferConnect(&buffer, editedPolygon);
}

void Editor::savePolygon() {
	if (etable->rowCount() < 3) {
		QMessageBox::warning(this, "Incorrect input", "Not enough vertices");
		return;
	}

	QVector<QString> vNames;
	QVector<QPair<double, double>> vCoords;
	for (int row = 0; row < vtable->rowCount() - 1; ++row) {
		QString vName = qobject_cast<QLineEdit*>(vtable->cellWidget(row, 0))->text();
		double x =
			qobject_cast<QDoubleSpinBox *>(vtable->cellWidget(row, 1))->value();
		double y =
			qobject_cast<QDoubleSpinBox *>(vtable->cellWidget(row, 2))->value();
		vNames.append(vName);
		vCoords.emplaceBack(x, y);
	}
	// CHECKS
	// уникальность имён и координат вершин внутри полигона
	QSet<QString> vNameCheck;
	QSet<QPair<double, double>> vCoordCheck;
	for (int row = 0; row < vtable->rowCount() - 1; ++row) {
		if (vNameCheck.contains(vNames[row])) {
			QMessageBox::warning(this, "Incorrect input", "Duplicate vertex name within the polygon");
			return;
		}
		if (vCoordCheck.contains(vCoords[row])) {
			QMessageBox::warning(this, "Incorrect input", "Identical vertices within the polygon");
			return;
		}
		vNameCheck.insert(vNames[row]);
		vCoordCheck.insert(vCoords[row]);
	}
	// выпуклость (и впуклость)
	bool convex = checkConvex(vCoords);
	if (!convex) {
		QMessageBox::warning(this, "Incorrect input", "Polygon should be convex");
		return;
	}
	// уникальность имён рёбер
	QVector<QString> eNames;
	for (int row = 0; row < etable->rowCount(); ++row) {
		QString ename =
			qobject_cast<QLineEdit *>(etable->cellWidget(row, 0))->text();
		if (ename == "") {
			ename = "E" + QString::number(polygonNumber) + "_" +
					QString::number(row);
		}
		eNames.append(ename);
	}
	QSet<QString> eNameCheck;
	for (int row = 0; row < etable->rowCount(); ++row) {
		if (eNameCheck.contains(eNames[row])) {
			QMessageBox::warning(this, "Incorrect input", "Duplicate edge name within the polygon");
			return;
		}
		eNameCheck.insert(eNames[row]);
	}

	// имя полигона
	QString name = ui->polygonNameEdit->text();
	if (name.isEmpty()) {
		name = ui->polygonNameEdit->placeholderText();
	}
	// материал полигона
	int material = ui->polygonMaterial->currentText().toInt();

	QUuid id;
	if (editedPolygon != nullptr) {
		// name = editedPolygon->name();
		id = editedPolygon->id();
		editedPolygon->delete_polygon();
		editedPolygon = nullptr;
	}

	QVector<QUuid> vertices;
	QVector<QUuid> edges;

	// создаём точки
	for (int row = 0; row < etable->rowCount(); ++row) {
		Vertex v(vCoords[row].first, vCoords[row].second, vNames[row]);
		vertices.append(v.id());
	}

	// создаём рёбра
	for (int row = 0; row < etable->rowCount(); ++row) {
		int property = qobject_cast<QComboBox *>(etable->cellWidget(row, 1))
						   ->currentText()
						   .toInt();
		Edge e(vertices[row], vertices[(row + 1) % etable->rowCount()], eNames[row],
			   property);
		edges.append(e.id());
	}


	// создаём полигон
	Polygon *p = new Polygon(vertices, edges, name, material, polygonNumber, id);

	// emit Mediator::instance()->polygonAdd(&p);
	// if (!id.isNull()) {
	//     mainWindow->removePolygon(id);
	// }
	// mainWindow->addPolygon(p);
	emit Mediator::instance() -> onPolygonSave(p, id.isNull());
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
	emit Mediator::instance() -> onBufferConnect(&buffer, nullptr);
	emit Mediator::instance()->onHighlightReset();
	emit Mediator::instance()->onEditingExit();
	polygonNumber = Polygon::get_polygons_total();
	ui->polygonNameEdit->setText("");
	ui->polygonNameEdit->setPlaceholderText("P" + QString::number(polygonNumber));
	clearNew();
	editedPolygon = nullptr;
	dock->close();
}

void Editor::onPolygonSelectReceived(Polygon *polygon) {
	if (editedPolygon != nullptr) {
		return;
	}
	resetEditor();
	setupExistingPolygon(polygon);
}

void Editor::setupExistingPolygon(Polygon *polygon) {
	editedPolygon = polygon;
	polygonNumber = editedPolygon->number();
	ui->polygonNameEdit->setPlaceholderText("P" + QString::number(polygonNumber));
	ui->polygonNameEdit->setText(polygon->name());
	ui->polygonMaterial->setCurrentText(QString::number(polygon->material()));
	// emit Mediator::instance()->onBufferConnect(&buffer, polygon);
	for (auto &vId : polygon->vertices) {
		Vertex &v = all_vertices[vId];
		QString vName = v.name();
		double x = v.x();
		double y = v.y();
		// addVertexRow(vtable->rowCount() - 1, vName, x, y);
		addVRow(vtable->rowCount() - 1, vName, x, y);
	}

	// for (auto &eId : polygon->edges) {
	for (int i = 0; i < polygon->edges.size(); ++i) {
		auto eId = polygon->edges[i];
		Edge &e = all_edges[eId];
		QString eName = e.name();
		QString property = QString::number(e.get_property());
		// auto material = e.get_property();

		qobject_cast<QLineEdit *>(etable->cellWidget(i, 0))
			->setText(eName);
		QComboBox* cbox = qobject_cast<QComboBox *>(etable->cellWidget(i, 1));
		int idx = cbox->findText(property);
		cbox->setCurrentIndex(idx);
		dock->show();
		emit Mediator::instance() -> onBufferConnect(&buffer, polygon);
	}
}

void Editor::onVertexSelectReceived(Vertex* vertex) {
	auto vName = vertex->name();
	for (int row = 0; row < vtable->rowCount() - 1; ++row) {
		QLineEdit* nameEdit = qobject_cast<QLineEdit*>(vtable->cellWidget(row, 0));
		if (nameEdit->text() == vName) {
			editVertex(row);
			return;
		}
	}
}

void Editor::onEdgeSelectReceived(Edge* edge) {
	auto eName = edge->name();
	for (int row = 0; row < etable->rowCount(); ++row) {
		QLineEdit* nameEdit = qobject_cast<QLineEdit*>(etable->cellWidget(row, 0));
		if (nameEdit->text() == eName) {
			etable->selectRow(row);
			return;
		}
	}
}

// Add Vertext by mouse click
// void Editor::addVertexByMouse(QPoint *point) {
// 	int row = vtable->rowCount() - 1;
// 	QString name =
// 		"V" + QString::number(polygonNumber) + "_" + QString::number(row);

// 	// For check open dock or not
// 	if (dock->isVisible()) {
// 		addVRow(row, name, point->x(), point->y());
// 		onBufferConnect();
// 	} else {
// 		resetEditor();
// 		dock->show();
// 		addVRow(row, name, point->x(), point->y());
// 	}
// }

// // Change coord for vertex when move by mouse
// void Editor::editVertexCoord(int row, QPoint *new_coord) {
// 	qobject_cast<QDoubleSpinBox *>(vtable->cellWidget(row, 1))
// 		->setValue(new_coord->x());
// 	qobject_cast<QDoubleSpinBox *>(vtable->cellWidget(row, 2))
// 		->setValue(new_coord->y());

// 	buffer[row] = QVector2D(new_coord->x(), new_coord->y());
// 	onBufferConnect();
// }

Editor::~Editor() { delete ui; }
