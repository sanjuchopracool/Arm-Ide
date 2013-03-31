#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QDialog>
#include <ProjectData.h>

namespace Ui {
class NewProject;
}

class NewProject : public QDialog
{
    Q_OBJECT
    
public:
    NewProject(QWidget *parent = 0,Project* project = 0);
    ~NewProject();
    
private:
    Ui::NewProject *ui;
    int m_returnCode;
    Project* m_project;

public slots:
    void browse();
    void projectText();
    void setDefaultchecked(bool val);
    void next();
    void createProject();
    int returnCode();
    void generateProjectFile();
};

#endif // NEWPROJECT_H
