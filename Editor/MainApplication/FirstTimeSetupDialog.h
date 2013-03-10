#ifndef FIRSTTIMESETUPDIALOG_H
#define FIRSTTIMESETUPDIALOG_H

#include <QDialog>
#include "ui_FirstTimeSetupDialog.h"

class FirstTimeSetupDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FirstTimeSetupDialog(QWidget *parent = 0);
    
signals:
    
public slots:
    void apply();
    void help();
private:
    Ui::FirstTimeSetupDialog ui;
};

#endif // FIRSTTIMESETUPDIALOG_H
