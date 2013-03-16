#include "FirstTimeSetupDialog.h"
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTextEdit>
#include <QStyle>
#include "ToolChainHelpDialog.h"
#include "SoftwareDefaults.h"

FirstTimeSetupDialog::FirstTimeSetupDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    this->setWindowTitle(tr("Select default toolChain setting"));
    QString toolChainPrefix = SoftwareDefaults::instance().toolChainPrefix();
    if(toolChainPrefix.isEmpty())
        toolChainPrefix = "arm-elf";

    ui.leToolChainName->setText(toolChainPrefix);

    ui.leMakeName->setText("make");
    ui.pbHelp->setIcon(style()->standardIcon(QStyle::SP_DialogHelpButton));
    connect(ui.pbSkip,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui.pbApply,SIGNAL(clicked()),this,SLOT(apply()));
    connect(ui.pbHelp,SIGNAL(clicked()),this,SLOT(help()));
}

void FirstTimeSetupDialog::apply()
{
    SoftwareDefaults::instance().setToolChainPrefix(ui.leToolChainName->text());
    SoftwareDefaults::instance().modifiedSettings();
    this->close();
}

void FirstTimeSetupDialog::help()
{
    ToolChainHelpDialog helpDialog(this);
    helpDialog.exec();
}

