#ifndef PROJECTEXPLORER_H
#define PROJECTEXPLORER_H

#include <QTreeView>

struct ProjectExplorerData;
class ProjectExplorer : public QTreeView
{
    Q_OBJECT
public:
    ProjectExplorer(QWidget *parent = 0);
    
signals:
    
public slots:

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    ProjectExplorerData* d;
    
};

#endif // PROJECTEXPLORER_H
