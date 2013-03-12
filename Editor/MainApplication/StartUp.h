#ifndef STARTUP_H
#define STARTUP_H

#include <QDialog>
#include "ui_StartUp.h"
#include <QRegExp>

class StartUp : public QDialog
{
    Q_OBJECT
public:
    explicit StartUp(QWidget *parent = 0);
    void prepareOtherString();
    
signals:
    
public slots:
    void applyChanges();
    void previewSlot();

private:
    Ui::StartUp ui;
    QString startUpCode1;
    QString startUpCode2;
    QString startUpCode3;
    QString startUpCode4;
    QString startUpCode5;
    
};

#endif // STARTUP_H
