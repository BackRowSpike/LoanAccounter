#include <QApplication>
#include "tablesortfilterwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TableSortFilterWindow mainwindow;
    mainwindow.showMaximized();
    return a.exec();
}
