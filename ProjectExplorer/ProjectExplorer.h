#ifndef PROJECTEXPLORER_H
#define PROJECTEXPLORER_H

#include <QTreeView>


enum FileType {
    PROJECTFILE,
    HEADERFILE,
    CFILE,
    ASSSEMBLYFILE,
    LINKERFILE,
    OTHERFILE,
    FOLDER
};


struct ProjectExplorerData;
class ProjectExplorer : public QTreeView
{
    Q_OBJECT
public:
    ProjectExplorer(QWidget *parent = 0);
    FileType fileTypeFromSuffix(const QString& fileName);

signals:

public slots:

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    ProjectExplorerData* d;

};

#endif // PROJECTEXPLORER_H
