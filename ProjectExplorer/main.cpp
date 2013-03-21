#include <QApplication>
#include "ProjectExplorer.h"
#include <QStandardItem>
#include <QStandardItemModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProjectExplorer explorer;
    QStandardItemModel model;
    QStandardItem* project = new QStandardItem("Project1.chops");
    project->setEditable(false);
    QStandardItem* headers = new QStandardItem("Headers");
    headers->setEditable(false);
    QStandardItem* header1 = new QStandardItem("Header1.h");
    header1->setEditable(false);
    QStandardItem* header2 = new QStandardItem("Header2.h");
    header2->setEditable(false);
    headers->appendRow(header1);
    headers->appendRow(header2);
    QStandardItem* sources = new QStandardItem("Sources");
    sources->setEditable(false);
    QStandardItem* source1 = new QStandardItem("source1.c");
    source1->setEditable(false);
    QStandardItem* source2 = new QStandardItem("source2.c");
    source2->setEditable(false);
    QStandardItem* startup = new QStandardItem("startup.S");
    startup->setEditable(false);
    sources->appendRow(source1);
    sources->appendRow(source2);
    sources->appendRow(startup);
    QStandardItem* linker = new QStandardItem("Linker");
    linker->setEditable(false);
    QStandardItem* rom = new QStandardItem("ROM.ld");
    rom->setEditable(false);
    linker->appendRow(rom);

    QStandardItem* others = new QStandardItem("Others");
    others->setEditable(false);
    QStandardItem* other1 = new QStandardItem("other1.txt");
    other1->setEditable(false);
    QStandardItem* other2 = new QStandardItem("other2.xml");
    other2->setEditable(false);
    others->appendRow(other1);
    others->appendRow(other2);

    project->appendRow(headers);
    project->appendRow(sources);
    project->appendRow(linker);
    project->appendRow(others);

    model.setItem(0,project);

    explorer.setModel(&model);
    explorer.show();
    return a.exec();
}
