#ifndef GENERALPLAINTEXTDIALOG_H
#define GENERALPLAINTEXTDIALOG_H

#include <QDialog>

namespace Ui {
class GeneralPlainTextDialog;
}

class GeneralPlainTextDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GeneralPlainTextDialog(QWidget *parent = 0);
    ~GeneralPlainTextDialog();
    void setText( const QString& text);
    
private:
    Ui::GeneralPlainTextDialog *ui;
};

#endif // GENERALPLAINTEXTDIALOG_H
