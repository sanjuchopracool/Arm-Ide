#ifndef STARTUP_H
#define STARTUP_H

#include <QDialog>
#include "ui_StartUp.h"
#include <QRegExp>
#include "ProjectData.h"

class StartUp : public QDialog
{
    Q_OBJECT
public:
    explicit StartUp(QWidget *parent = 0, Project *project = 0);
    void prepareOtherString();
    void updateProjectSetting();
    
signals:
    
public slots:
    void applyChanges();
    void previewSlot();
    void nextSlot();

private:
    Ui::StartUp ui;
    Project* m_project;
    QString startUpCode1;
    QString startUpCode2;
    QString startUpCode3;
    QString startUpCode4;
    QString startUpCode5;
    
};

#endif // STARTUP_H
