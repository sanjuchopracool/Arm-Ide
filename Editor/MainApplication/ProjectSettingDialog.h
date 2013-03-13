#ifndef PROJECTSETTINGDIALOG_H
#define PROJECTSETTINGDIALOG_H

#include <QDialog>
#include "ui_ProjectSettingDialog.h"

class ProjectSettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProjectSettingDialog(QWidget *parent = 0);
    
signals:
    
public slots:
    void next();

private:
    Ui::ProjectDialogSetting ui;
    
};

#endif // PROJECTSETTINGDIALOG_H
