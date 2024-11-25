#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
// #include <QApplication>
// #include <QTableWidget>
// #include <QPushButton>
// #include <QVBoxLayout>

// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);

//     QTableWidget table(3, 2); // Create a 3x2 table
//     table.setHorizontalHeaderLabels({"Column 1", "Column 2"});

//     // Add a button to the first cell in the second column and hide it
//     QPushButton *button = new QPushButton("Hidden Button");
//     table.setCellWidget(0, 1, button); // Add button to cell (0, 1)
//     button->hide(); // Immediately hide the button

//     table.hide();

//     return app.exec();
// }
