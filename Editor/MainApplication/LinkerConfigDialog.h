#ifndef LINKERCONFIGDIALOG_H
#define LINKERCONFIGDIALOG_H

#include <QDialog>
#include <QTextStream>

struct LinkerData;
namespace Ui {
class LinkerConfigDialog;
}

class LinkerConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    LinkerConfigDialog(QWidget *parent = 0);
    ~LinkerConfigDialog();
    void generateLinkerFile();
    void linkerText(QTextStream& linkerStream);
    
private:
    Ui::LinkerConfigDialog *ui;
    LinkerData *d;

private slots:
    void applyChanges();
    void previewLinkerFile();
    void nextSlot();
};

#endif // LINKERCONFIGDIALOG_H
