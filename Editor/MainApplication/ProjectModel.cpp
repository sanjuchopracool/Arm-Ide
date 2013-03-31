#include "ProjectModel.h"
#include <QMap>
#include <QDebug>

struct ProjectModelData
{
    QList<Project*> projects;
    QMap <Project*,QStandardItem*> projectsMap;
};

ProjectModel::ProjectModel(QObject *parent) :
    QStandardItemModel(parent)
{
    d = new ProjectModelData;
//    Project* project = new Project();
//    project->setProjectName("project1.chops");
//    addProject(project);
//    addFileToProject(project,"Chops.c");
//    addFileToProject(project,"Chops.S");
//    addFileToProject(project,"Chops.txt");
//    addFileToProject(project,"Chops.h");
//    removeFileFromProject(project,"Chops.c");
////    removeFileFromProject(project,"Chops.txt");
//    project = new Project();
//    project->setProjectName("project2.chops");
//    addProject(project);
//    addFileToProject(project,"Chops.txt");
}

//ProjectModel &ProjectModel::instance()
//{
//    static ProjectModel theInstance;
//    return theInstance;
//}

void ProjectModel::addProject(Project* project)
{
    if(!project)
        return;

    if(d->projects.contains(project))
        return;

    d->projects.append(project);
    QStandardItem* projectItem = modelItemForProject(project);
    d->projectsMap.insert(project,projectItem);
    appendRow(projectItem);

}

void ProjectModel::removeProject(Project* project)
{
    if(!project)
        return;

    if(!d->projects.contains(project))
        return;

    d->projects.removeAt(d->projects.indexOf(project));
    QStandardItem* projectItem = d->projectsMap.value(project);
    removeRow(projectItem->row(),projectItem->index().parent());
    d->projectsMap.remove(project);

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

void ProjectModel::addFileToProject(Project *project, const QString &fileName)
{
    if(!project)
        return;

    if(fileName.isEmpty())
        return;

    FileType type;
    QStandardItem* newFileItem;
    QStandardItem* projectItem = d->projectsMap.value(project);
    if(fileName.endsWith(".c"))
    {
        type = CFILE;
        newFileItem = new QStandardItem(QIcon(":/images/source.png"),fileName);
    }
    else if (fileName.endsWith(".h"))
    {
        type = HEADERFILE;
        newFileItem = new QStandardItem(QIcon(":/images/header.png"),fileName);
    }
    else if(fileName.endsWith(".S") || fileName.endsWith(".s"))
    {
        type = ASSSEMBLYFILE;
        newFileItem = new QStandardItem(QIcon(":/images/assembly.png"),fileName);
    }
    else
    {
        type = OTHERFILE;
        newFileItem = new QStandardItem(QIcon(":/images/text.png"),fileName);
    }

    QStandardItem* folder;

    switch (type)
    {
    case CFILE:
    case ASSSEMBLYFILE:
        folder = projectItem->child(1);
        break;
    case HEADERFILE:
        folder = projectItem->child(0);
        break;
    case OTHERFILE:
        folder = projectItem->child(3);
        if(!folder)
        {
            folder = new QStandardItem(QIcon(":/images/folder.png"),"Others");
            projectItem->appendRow(folder);
            folder = projectItem->child(3);
        }
        break;
    default:
        break;
    }

    folder->appendRow(newFileItem);
}

void ProjectModel::removeFileFromProject(Project *project, const QString &fileName)
{
    if(!project)
        return;

    if(fileName.isEmpty())
        return;

    FileType type;
    QStandardItem* projectItem = d->projectsMap.value(project);
    if(fileName.endsWith(".c"))
        type = CFILE;
    else if (fileName.endsWith(".h"))
        type = HEADERFILE;
    else if(fileName.endsWith(".S") || fileName.endsWith(".s"))
        type = ASSSEMBLYFILE;
    else
        type = OTHERFILE;

    QStandardItem* folder;

    switch (type)
    {
    case CFILE:
    case ASSSEMBLYFILE:
        folder = projectItem->child(1);
        break;
    case HEADERFILE:
        folder = projectItem->child(0);
        break;
    case OTHERFILE:
        folder = projectItem->child(3);
        break;
    default:
        break;
    }

    int childrenCount = folder->rowCount();
    for(int i = 0; i < childrenCount; i++)
        if(folder->child(i)->data(Qt::DisplayRole).toString() == fileName)
        {
            folder->removeRow(i);
            break;
        }

    if(!folder->rowCount())
        removeRow(folder->row(), folder->parent()->index());
}


void ProjectModel::updateModel()
{
}
