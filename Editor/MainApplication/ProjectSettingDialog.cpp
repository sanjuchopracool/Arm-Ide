#include "ProjectSettingDialog.h"

ProjectSettingDialog::ProjectSettingDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.pbNext,SIGNAL(clicked()),this,SLOT(next()));
}

void ProjectSettingDialog::next()
{
    close();
}
