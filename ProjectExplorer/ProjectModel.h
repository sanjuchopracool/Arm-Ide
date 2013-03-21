#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H
#include <QStandardItemModel>
#include "../Editor/MainApplication/ProjectData.h"
#include "ProjectExplorer.h"
struct ProjectModelData;

class ProjectModel : public QStandardItemModel
{
    Q_OBJECT
public:
    ProjectModel(QObject *parent = 0);

    void addProject(Project* project);
    void removeProject(Project* project);

    QStandardItem* modelItemForProject(Project* project);
    QList<QStandardItem*> modelItemsForStringList(const QStringList& list);

    
signals:
    
public slots:
    void updateModel();
private:
    ProjectModelData* d;
    
};

#endif // PROJECTMODEL_H
