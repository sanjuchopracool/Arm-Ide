#include "ToolChainHelpDialog.h"

ToolChainHelpDialog::ToolChainHelpDialog(QDialog *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    ui.textEdit->setReadOnly(true);
    setWindowTitle(tr("How to select toolchain."));
}
