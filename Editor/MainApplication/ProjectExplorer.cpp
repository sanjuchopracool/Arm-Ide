#include "ProjectExplorer.h"
#include <QDebug>
#include <QContextMenuEvent>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>

struct ProjectExplorerData
{
    ProjectExplorerData() {}
    QAction* setAsActiveAction;
    QAction* makeFileAction;
    QAction* buildAction;
    QAction* rebuildAction;
    QAction* cleanAction;
    QAction* addNew;
    QAction* addExistingAction;
    QAction* closeAction;
    QAction* openAction;
    QAction* removeAction;
    QAction* renameAction;
};

ProjectExplorer::ProjectExplorer(QWidget *parent) :
    QTreeView(parent)
{
    //hide headers
    setHeaderHidden(true);
    d = new ProjectExplorerData();
    d->setAsActiveAction = new QAction(tr("Set as active project"),this);
    d->makeFileAction = new QAction(tr("Generate Makefile"),this);
    d->buildAction = new QAction(tr("Build"),this);
    d->rebuildAction = new QAction(tr("Rebuild"),this);
    d->cleanAction = new QAction(tr("Clean"),this);
    d->addNew = new QAction(tr("Add new"),this);
    d->addExistingAction = new QAction(tr("Add existing"),this);
    d->closeAction = new QAction(tr("Close project"),this);
    d->openAction = new QAction(tr("Open"),this);
    d->removeAction = new QAction(tr("Remove"),this);
    d->renameAction = new QAction(tr("Rename"),this);

}

FileType ProjectExplorer::fileTypeFromSuffix(const QString& fileName)
{
    FileType fileType;
    if(fileName.endsWith(".c"))
        fileType = CFILE;
    else if(fileName.endsWith(".h"))
        fileType = HEADERFILE;
    else if (fileName.endsWith(".chops"))
        fileType = PROJECTFILE;
    else if (fileName.endsWith(".S") || fileName.endsWith(".s"))
        fileType = ASSSEMBLYFILE;
    else if (fileName.endsWith(".ld"))
        fileType = LINKERFILE;
    else if( fileName.isEmpty() || fileName == "Headers" || fileName == "Sources" ||
             fileName == "Linker" || fileName == "Others")
        fileType = FOLDER;
    else
        fileType = OTHERFILE;

    return fileType;
}

//ProjectExplorer &ProjectExplorer::instance()
//{
//    static ProjectExplorer theInstance;
//    return theInstance;
//}

void ProjectExplorer::contextMenuEvent(QContextMenuEvent *event)
{
    /*
     * Context menu event
     * If clicked on chops file either project file
     * Menu Items will be
     *
     * set as Active Project
     * Generate makefile
     * Build
     * Rebuild
     * Clean
     * Add new
     * Add existing
     * Close Project
     */

    /*
     * Sources files menu will be
     * Open
     * Remove
     * Rename
     */

    /*
     * Linker and startup file must always be there with same name
     * Menu will be
     * Open
     *
     */

    /*
     * For other file menu will contain
     * Open
     * Remove
     * Rename
     */

    QPoint pos = event->pos();
    QModelIndex index = indexAt(pos);
    QAbstractItemModel* fModel = model();

    FileType fileType;
    QString projectName;

    QString fileName = fModel->data(index,Qt::DisplayRole).toString();

    fileType = fileTypeFromSuffix(fileName);

    if(fileType == FOLDER)
        return;

    if(fileType == PROJECTFILE)
        projectName = fileName;
    else
        projectName = fModel->data(index.parent().parent(),Qt::DisplayRole).toString();

    QMenu fileMenu(this);
    switch (fileType) {
    case PROJECTFILE:
        fileMenu.addAction(d->setAsActiveAction);
        fileMenu.addAction(d->makeFileAction);
        fileMenu.addSeparator();
        fileMenu.addAction(d->buildAction);
        fileMenu.addAction(d->rebuildAction);
        fileMenu.addAction(d->cleanAction);
        fileMenu.addSeparator();
        fileMenu.addAction(d->addNew);
        fileMenu.addAction(d->addExistingAction);
        fileMenu.addSeparator();
        fileMenu.addAction(d->closeAction);
        break;

    case CFILE:
    case ASSSEMBLYFILE:
    case OTHERFILE:
    case HEADERFILE:

        fileMenu.addAction(d->openAction);
        if(fileName == "startup.S")
            break;
        fileMenu.addAction(d->renameAction);
        fileMenu.addAction(d->removeAction);
        break;

    case LINKERFILE:
        fileMenu.addAction(d->openAction);
        break;

    default:
        break;
    }

    fileMenu.exec(QCursor::pos());
    qDebug() << "Project Name" << projectName;
    //    QModelIndex parent = index.parent();
    //    if(parent.isValid())
    //        qDebug() << fModel->data(parent,Qt::DisplayRole).toString();
}

void ProjectExplorer::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    QModelIndex index = indexAt(pos);
    QAbstractItemModel* fModel = model();

    FileType fileType;
    QString projectName;

    QString fileName = fModel->data(index,Qt::DisplayRole).toString();

    fileType = fileTypeFromSuffix(fileName);

    if(fileType == FOLDER)
        return;

    if(fileType == PROJECTFILE)
        projectName = fileName;
    else
        projectName = fModel->data(index.parent().parent(),Qt::DisplayRole).toString();

    qDebug() << "Project Name" << projectName;
}

