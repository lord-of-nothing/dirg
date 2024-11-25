#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->editorDock->setWidget(new Editor(ui->editorDock));
}



MainWindow::~MainWindow()
{
    delete ui;
}
