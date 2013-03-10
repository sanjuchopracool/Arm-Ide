#ifndef TOOLCHAINHELPDIALOG_H
#define TOOLCHAINHELPDIALOG_H

#include <QDialog>
#include "ui_ToolChainHelpDialog.h"
class ToolChainHelpDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ToolChainHelpDialog(QDialog *parent = 0);
    
signals:
    
public slots:
    
private:
    Ui::ToolChainHelpDialog ui;
};

#endif // TOOLCHAINHELPDIALOG_H
