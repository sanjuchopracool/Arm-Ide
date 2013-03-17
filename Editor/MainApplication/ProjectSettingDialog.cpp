#include "ProjectSettingDialog.h"
#include "SoftwareDefaults.h"

ProjectSettingDialog::ProjectSettingDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.pbNext,SIGNAL(clicked()),this,SLOT(next()));
    ui.leToolChainPrefix->setText(SoftwareDefaults::instance().toolChainPrefix());

    setWindowTitle(tr("Choose compiler setting"));

    //-mcpu=$(ARM_CPU)
    ui.leCFlags->setText("-mthumb-interwork -ffunction-sections -Os -Wall -nostdlib");
    ui.leAssemblerFlags->setText("-mthumb-interwork -mfpu=softfpa -Wall");
    ui.cbBin->setChecked(true);
    ui.cbHex->setChecked(true);
}

void ProjectSettingDialog::next()
{
    close();
}
