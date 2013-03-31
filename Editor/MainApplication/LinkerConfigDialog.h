#ifndef LINKERCONFIGDIALOG_H
#define LINKERCONFIGDIALOG_H

#include <QDialog>
#include <QTextStream>
#include "ProjectData.h"
struct LinkerData;
namespace Ui {
class LinkerConfigDialog;
}

class LinkerConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    LinkerConfigDialog(QWidget *parent = 0, Project* project = 0);
    ~LinkerConfigDialog();
    void generateLinkerFile();
    void linkerText(QTextStream& linkerStream);
    void updateProjectSetting();
    
private:
    Ui::LinkerConfigDialog *ui;
    LinkerData *d;

private slots:
    void applyChanges();
    void previewLinkerFile();
    void nextSlot();
};

#endif // LINKERCONFIGDIALOG_H
