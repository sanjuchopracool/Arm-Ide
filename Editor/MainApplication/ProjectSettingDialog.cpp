#include "ProjectSettingDialog.h"
#include "SoftwareDefaults.h"

ProjectSettingDialog::ProjectSettingDialog(QWidget *parent, Project *project) :
    QDialog(parent)
{
    ui.setupUi(this);
    m_project = project;
    connect(ui.pbNext,SIGNAL(clicked()),this,SLOT(next()));
    connect(ui.pbApply,SIGNAL(clicked()),this,SLOT(apply()));
    setWindowTitle(tr("Choose compiler setting"));

    //-mcpu=$(ARM_CPU)
    if(!m_project || m_project->projectName().isEmpty())
    {
        ui.leToolChainPrefix->setText(SoftwareDefaults::instance().toolChainPrefix());
        ui.leCFlags->setText("-mthumb-interwork -ffunction-sections -Os -Wall -nostdlib");
        ui.leAssemblerFlags->setText("-mthumb-interwork -mfpu=softfpa -Wall");
        ui.cbBin->setChecked(true);
        ui.cbHex->setChecked(true);
    }
    else
    {
        ui.leToolChainPrefix->setText(m_project->toolChainPrefix());
        ui.leCFlags->setText(m_project->compilerCFlags());
        ui.leAssemblerFlags->setText(m_project->assemblerFlags());
        ui.cbBin->setChecked(m_project->makeBin());
        ui.cbHex->setChecked(m_project->makeHex());
    }
}

void ProjectSettingDialog::updateProjectSetting()
{

    m_project->setArmCore(ui.leCpu->text());
    m_project->setCompilerCFlags(ui.leCFlags->text());
    m_project->setAssemblerFlags(ui.leAssemblerFlags->text());
    m_project->setTooChainPrefix(ui.leToolChainPrefix->text());
    m_project->setMakeBin(ui.cbBin->isChecked());
    m_project->setMakeHex(ui.cbHex->isChecked());
}

void ProjectSettingDialog::next()
{
    if(!m_project)
        return;
    updateProjectSetting();
    close();
}

void ProjectSettingDialog::apply()
{
    if(!m_project)
        return;

    updateProjectSetting();
    close();
}
