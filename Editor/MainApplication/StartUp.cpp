#include "StartUp.h"
#include <QStyle>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include "GeneralPlainTextDialog.h"
#include <QTextStream>
#include "ProjectData.h"
#include "mainwindow.h"

StartUp::StartUp(QWidget *parent, Project* project) :
    QDialog(parent)
{
    ui.setupUi(this);
    m_project = project;
    setWindowTitle(tr("Startup code settings"));

    ui.pbHelp->setIcon(style()->standardIcon(QStyle::SP_DialogHelpButton));
    if(!m_project || m_project->isNewProject())
    {
        ui.cbIRQFunctions->setChecked(true);
        ui.leUndSize->setText("0x00000008");
        ui.leFiqSize->setText("0x00000008");
        ui.leSvcSize->setText("0x00000008");
        ui.leAbrtSize->setText("0x00000008");
        ui.leIrqSize->setText("0x00000080");
        ui.leUsrSize->setText("0x00000400");
    }
    else
    {
        ui.cbIRQFunctions->setChecked(m_project->useIrqFunctions());
        ui.leUndSize->setText(m_project->undSize());
        ui.leFiqSize->setText(m_project->fiqSize());
        ui.leSvcSize->setText(m_project->svcSize());
        ui.leAbrtSize->setText(m_project->abrtSize());
        ui.leIrqSize->setText(m_project->irqSize());
        ui.leUsrSize->setText(m_project->usrSize());
    }

    QRegExp hexAddressRegExp("0{1,1}x{1,1}[0-9a-fA-F]{8,8}");
    QRegExpValidator* hexAddressValidator = new QRegExpValidator(hexAddressRegExp);

    ui.leUndSize->setValidator(hexAddressValidator);
    ui.leFiqSize->setValidator(hexAddressValidator);
    ui.leSvcSize->setValidator(hexAddressValidator);
    ui.leAbrtSize->setValidator(hexAddressValidator);
    ui.leIrqSize->setValidator(hexAddressValidator);
    ui.leUsrSize->setValidator(hexAddressValidator);

    QFile start1(":/startup/start1.S");
    if(!start1.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open start1.S file";
    }
    else
    {
        startUpCode1 = start1.readAll();
        startUpCode1 += "\n\n";
        //qDebug() << startUpCode1;
        start1.close();
    }

    QFile start3(":/startup/start3.S");
    if(!start3.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open start3.S file";
    }
    else
    {
        startUpCode3 = start3.readAll();
        startUpCode3 += "\n\n";
        //qDebug() << startUpCode3;
        start3.close();
    }

    QFile start5(":/startup/start5.S");
    if(!start5.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open start5.S file";
    }
    else
    {
        startUpCode5 = start5.readAll();
        startUpCode5 += "\n\n";
        //qDebug() << startUpCode5;
        start5.close();
    }

    connect(ui.pbNext,SIGNAL(clicked()),this,SLOT(nextSlot()));
    connect(ui.pbApply,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui.pbPreview,SIGNAL(clicked()),this,SLOT(previewSlot()));
}

void StartUp::prepareOtherString()
{
    startUpCode2 += ".set UND_STACK  ," + ui.leUndSize->text() + "\n";
    startUpCode2 += ".set IRQ_STACK  ," + ui.leIrqSize->text() + "\n";
    startUpCode2 += ".set FIQ_STACK  ," + ui.leFiqSize->text() + "\n";
    startUpCode2 += ".set SVC_STACK  ," + ui.leSvcSize->text() + "\n";
    startUpCode2 += ".set ABRT_STACK  ," + ui.leAbrtSize->text() + "\n";
    startUpCode2 += ".set USR_STACK  ," + ui.leUsrSize->text() + "\n";

    startUpCode4 = "        bl    main   \n\n";
}

void StartUp::updateProjectSetting()
{

    m_project->setUndSize(ui.leUndSize->text());
    m_project->setIrqSize(ui.leIrqSize->text());
    m_project->setFiqSize(ui.leFiqSize->text());
    m_project->setSvcSize(ui.leSvcSize->text());
    m_project->setAbrtSize(ui.leAbrtSize->text());
    m_project->setUsrSize(ui.leUsrSize->text());
    m_project->addIRQFunctions(ui.cbIRQFunctions->isChecked());
    m_project->updateProjectFile();
}

void StartUp::applyChanges()
{
    if(!m_project)
        return;

    updateProjectSetting();

    QString srcDirName;
#ifdef Q_OS_UNIX
    srcDirName = m_project->projectPath() + "/src" ;
#else
    srcDirName = m_project->projectPath() + "\\src" ;
#endif

    QDir srcDir(srcDirName);
    if(!srcDir.exists())
    {
        if(!QDir::current().mkdir(srcDirName))
        {
            QMessageBox::warning(this,tr("Failed to generate startup.S file"),
                                 tr("Failed to create src directory\n"
                                    "please check permissions"));
        }
    }
#ifdef Q_OS_UNIX
            QFile startUpFile(srcDirName +"/startup.S");
#else
            QFile startUpFile(srcDirName +"\\startup.S");
#endif

    if(!startUpFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::warning(this,tr("Failed to open STartup.S file"),
                             tr("Failed to open startup file in read write mode\n"
                                "please check permissions"));
    }
    else
    {
        QTextStream startUpStream(&startUpFile);
        prepareOtherString();
        startUpStream << startUpCode1 << startUpCode2 << startUpCode3 + startUpCode4;

        if(ui.cbIRQFunctions->isChecked())
            startUpStream << startUpCode5;

        startUpFile.close();
    }
    this->close();
}

void StartUp::previewSlot()
{
    GeneralPlainTextDialog startUpPreviewDialog(this);

    prepareOtherString();

    QString str = startUpCode1 + startUpCode2 + startUpCode3 + startUpCode4;
    if(ui.cbIRQFunctions->isChecked())
        str += startUpCode5;
    startUpPreviewDialog.setText(str);
    startUpPreviewDialog.setMinimumWidth(700);
    startUpPreviewDialog.exec();
}

/*
 * IF linker does not exist(new Project) create it else don't modify the existing file
 */

void StartUp::nextSlot()
{
    if(!m_project)
        return;

    updateProjectSetting();
    QString srcDirName;

#ifdef Q_OS_UNIX
    srcDirName = m_project->projectPath() + "/src" ;
#else
    srcDirName = m_project->projectPath() + "\\src" ;
#endif

#ifdef Q_OS_UNIX
            QFile startUpFile(srcDirName +"/startup.S");
#else
            QFile startUpFile(srcDirName +"\\startup.S");
#endif

    if(startUpFile.exists())
        close();
    else
        applyChanges();
}
