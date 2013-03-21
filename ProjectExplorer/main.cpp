#include <QApplication>
#include "ProjectExplorer.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include "ProjectModel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProjectExplorer explorer;
    ProjectModel model;
    explorer.setModel(&model);
    explorer.show();
    return a.exec();
}
