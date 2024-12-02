#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "editor.h"

#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	ui->editorDock->setWidget(new Editor(ui->editorDock)); // ?? -- а что не так?
	connect(ui->editorDock, &QDockWidget::visibilityChanged, this, [this]() {
		if (!ui->editorDock->isVisible()) {
			emit Mediator::instance() -> onEditorReset();
		}
	});
	ui->editorDock->close();

	ui->tree->setHeaderHidden(true);
	connect(ui->newPolygonBtn, &QPushButton::released, this,
			&MainWindow::newPolygon);
	// connect(ui->tree, &QTreeWidget::itemClicked, this, onIte);
	connect(ui->tree, &QTreeWidget::itemDoubleClicked,
			[this](QTreeWidgetItem *item, [[maybe_unused]] int column) {
				QUuid itemId = item->data(0, Qt::UserRole).value<QUuid>();
				if (all_polygons.contains(itemId)) {
					selectPolygon(itemId);
					return;
				}
				QUuid polygonId = item->parent()->parent()->data(0, Qt::UserRole).value<QUuid>();
				selectPolygon(polygonId);
				if (all_vertices.contains(itemId)) {
					// selectVertex(id);
					emit Mediator::instance()->onVertexSelect(&all_vertices[itemId]);
				} else {
					// selectEdge(id);
					emit Mediator::instance()->onEdgeSelect(&all_edges[itemId]);
				}
			});
	connect(Mediator::instance(), &Mediator::onPolygonSave, this,
			[this](Polygon *polygon, bool isNew) {
				if (!isNew) {
					removePolygon(polygon->id());
				}
				addPolygon(polygon);
			});
	ui->tree->setSortingEnabled(true);
	ui->tree->sortByColumn(0, Qt::AscendingOrder);

	setWindowTitle("Grid Editor");
	setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

	QColor gray(229, 228, 226);
	QPalette pal = QPalette();
	pal.setColor(QPalette::Window, gray);

	ui->editorDock->setAutoFillBackground(true);
	ui->editorDock->setPalette(pal);
	ui->treeDock->setAutoFillBackground(true);
	ui->treeDock->setPalette(pal);

	setStyleSheet("QMainWindow::separator{ width: 0px; height: 0px; }");
	// setDockOptions(QMainWindow::GroupedDockWidgets);

	// Exit by Ctrl+Q
	auto actionClose = new QAction();
	actionClose->setShortcut(QKeySequence::Quit);
	addAction(actionClose);
	QObject::connect(actionClose, &QAction::triggered, this,
					 &QCoreApplication::quit);
}

void MainWindow::newPolygon() {
	emit Mediator::instance() -> onEditorReset();
	ui->editorDock->show();
}

void MainWindow::addPolygon(Polygon *polygon) {
	QTreeWidgetItem *polyItem = new QTreeWidgetItem(ui->tree);
	polyItem->setText(0, polygon->name());
	polyItem->setData(0, Qt::UserRole, QVariant::fromValue(polygon->id()));

	QTreeWidgetItem *vertexFolder = new QTreeWidgetItem(polyItem);
	vertexFolder->setText(0, "Vertices");
	vertexFolder->setData(0, Qt::UserRole, QVariant::fromValue(polygon->id()));
	QTreeWidgetItem *edgeFolder = new QTreeWidgetItem(polyItem);
	edgeFolder->setText(0, "Edges");
	edgeFolder->setData(0, Qt::UserRole, QVariant::fromValue(polygon->id()));

	for (auto &vertex : polygon->vertices) {
		QTreeWidgetItem *v = new QTreeWidgetItem(vertexFolder);
		v->setText(0, all_vertices[vertex].name());
		v->setData(0, Qt::UserRole, QVariant::fromValue(vertex));
	}
	for (auto &edge : polygon->edges) {
		QTreeWidgetItem *e = new QTreeWidgetItem(edgeFolder);
		e->setText(0, all_edges[edge].name());
		e->setData(0, Qt::UserRole, QVariant::fromValue(edge));
	}
}

void MainWindow::removePolygon(QUuid id) {
	QVariant target = QVariant::fromValue(id);
	int count = ui->tree->topLevelItemCount();
	// Проходим по всем дочерним элементам первого уровня
	for (int i = 0; i < count; ++i) {
		QTreeWidgetItem *item =
			ui->tree->topLevelItem(i); // Убираем первый элемент
		if (item->data(0, Qt::UserRole) == target) {
			delete item; // Удаляем поддерево
			return;		 // После удаления выходим
		}
	}
}

void MainWindow::selectPolygon(QUuid id) {
	if (id.isNull()) {
		return;
	}
	Polygon *polygon = &all_polygons[id];
	ui->editorDock->show();
	emit Mediator::instance() -> onPolygonSelect(polygon);
}

MainWindow::~MainWindow() { delete ui; }
