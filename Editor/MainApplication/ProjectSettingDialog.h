#ifndef PROJECTSETTINGDIALOG_H
#define PROJECTSETTINGDIALOG_H

#include <QDialog>
#include "ui_ProjectSettingDialog.h"
#include "ProjectData.h"

class ProjectSettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProjectSettingDialog(QWidget *parent = 0, Project* project = 0);
    void updateProjectSetting();
    
signals:
    
public slots:
    void next();
    void apply();

private:
    Ui::ProjectDialogSetting ui;
    Project* m_project;
    
};

#endif // PROJECTSETTINGDIALOG_H
