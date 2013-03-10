#include "FirstTimeSetupDialog.h"
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTextEdit>
#include <QStyle>
#include "ToolChainHelpDialog.h"

FirstTimeSetupDialog::FirstTimeSetupDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    ui.leToolChainName->setText("arm-elf");
    ui.leMakeName->setText("make");
    ui.pbHelp->setIcon(style()->standardIcon(QStyle::SP_DialogHelpButton));
    connect(ui.pbSkip,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui.pbApply,SIGNAL(clicked()),this,SLOT(apply()));
    connect(ui.pbHelp,SIGNAL(clicked()),this,SLOT(help()));
}

void FirstTimeSetupDialog::apply()
{
    QString toolChainConfigFile("ToolChain.xml");
    QFile toolChain(toolChainConfigFile);
    if(!toolChain.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::warning(this,tr("ToolChain configuration failed"),
                             tr("Unable to open %1 in write mode.\n"
                                "Check for user permissions.").arg(toolChainConfigFile));
    }
    else
    {
        if(ui.leToolChainName->text().isEmpty() || ui.leMakeName->text().isEmpty())
        {
            QMessageBox::warning(this,tr("ToolChain configuration failed"),
                                 tr("Please enter names for compiler and make."));
            toolChain.close();
            return;
        }
        QDomDocument document("ToolChain");
        QDomElement tools = document.createElement("tools");
        document.appendChild(tools);
        QDomElement toolChainE = document.createElement("Compiler");
        toolChainE.setAttribute("name",ui.leToolChainName->text());
        tools.appendChild(toolChainE);
        QDomElement makeE = document.createElement("Make");
        makeE.setAttribute("name",ui.leMakeName->text());
        tools.appendChild(makeE);
        QTextStream xmlStream(&toolChain);
        xmlStream << document.toString();
        toolChain.close();
    }
    close();
}

void FirstTimeSetupDialog::help()
{
    ToolChainHelpDialog helpDialog(this);
    helpDialog.exec();
}

