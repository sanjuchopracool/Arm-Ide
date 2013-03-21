#include "ProjectModel.h"
struct ProjectModelData
{
    QList<Project*> projects;
};

ProjectModel::ProjectModel(QObject *parent) :
    QStandardItemModel(parent)
{
    d = new ProjectModelData;
    Project* project = new Project();
    project->setProjectName("project1.chops");
    addProject(project);
    project = new Project();
    project->setProjectName("project2.chops");
    addProject(project);
}

void ProjectModel::addProject(Project* project)
{
    if(!project)
        return;

    if(d->projects.contains(project))
        return;

    appendRow(modelItemForProject(project));

}

void ProjectModel::removeProject(Project* project)
{
    if(!project)
        return;

    if(!d->projects.contains(project))
        return;
}

QStandardItem* ProjectModel::modelItemForProject(Project *project)
{
    if(!project)
        return new QStandardItem();

    QStandardItem* projectItem = new QStandardItem(QIcon(":/images/project.png"),project->projectName());
    QStandardItem* headers = new QStandardItem(QIcon(":/images/folder.png"),"Headers");
    headers->appendRows(modelItemsForStringList(project->headers()));
    QStandardItem* sources = new QStandardItem(QIcon(":/images/folder.png"),"Sources");
    sources->appendRows(modelItemsForStringList(project->sources()));
    QStandardItem* linker = new QStandardItem(QIcon(":/images/folder.png"),"Linker");
    linker->appendRow(new QStandardItem(QIcon(":/images/text.png"),"Rom.ld"));

    projectItem->appendRow(headers);
    projectItem->appendRow(sources);
    projectItem->appendRow(linker);

    return projectItem;
}

QList<QStandardItem *> ProjectModel::modelItemsForStringList(const QStringList& list)
{
    QList<QStandardItem*> itemList;
    Q_FOREACH(QString item,list)
    {
        QStandardItem* modelItem = new QStandardItem(item);
        if (item.endsWith(".h"))
            modelItem->setIcon(QIcon(":/images/header.png"));
        else if(item.endsWith(".c"))
            modelItem->setIcon(QIcon(":/images/source.png"));
        else if(item.endsWith(".S") || item.endsWith(".s"))
            modelItem->setIcon(QIcon(":/images/assembly.png"));
        itemList << modelItem;
    }
    return itemList;
}


void ProjectModel::updateModel()
{
}