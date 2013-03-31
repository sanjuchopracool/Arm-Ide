#include "NewProject.h"
#include "ui_NewProject.h"
#include <QFileDialog>
#include <ProjectData.h>
#include <QMessageBox>
#include <QDebug>
#include "ProjectSettingDialog.h"
#include "LinkerConfigDialog.h"
#include "StartUp.h"
#include "SoftwareDefaults.h"
#include "mainwindow.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QTextStream>


NewProject::NewProject(QWidget *parent, Project* project) :
    QDialog(parent),
    ui(new Ui::NewProject)
{
    ui->setupUi(this);
    m_project = project;
    ui->pbNext->setEnabled(false);
    m_returnCode = -1;
    setWindowTitle(tr("New Project"));

    QString directory = SoftwareDefaults::instance().defaultProjectDir();
    if(directory.isEmpty())
        directory = QDir::homePath();

    ui->leProjectDirectory->setText(directory);
    connect(ui->pbCancel,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->pbBrowse,SIGNAL(clicked()),this,SLOT(browse()));
    connect(ui->leProjectName,SIGNAL(textChanged(QString)),this,SLOT(projectText()));
    connect(ui->checkBox,SIGNAL(toggled(bool)),this,SLOT(setDefaultchecked(bool)));
    connect(ui->pbNext,SIGNAL(clicked()),this,SLOT(next()));
}

NewProject::~NewProject()
{
    delete ui;
}

void NewProject::browse()
{
    QString browsedDirectory = QFileDialog::getExistingDirectory(this,tr("Select Project Directory"),
                                                                 ui->leProjectDirectory->text());
    if(!browsedDirectory.isNull())
        ui->leProjectDirectory->setText(browsedDirectory);
}

void NewProject::projectText()
{
    if(ui->leProjectName->text().isEmpty())
        ui->pbNext->setEnabled(false);
    else
        ui->pbNext->setEnabled(true);

    if(QDir(QString(ui->leProjectDirectory->text() + "/" + ui->leProjectName->text()).trimmed()).exists())
    {
        ui->leProjectName->setStyleSheet("color : red");
    }
    else
        ui->leProjectName->setStyleSheet("color : black");
}

void NewProject::setDefaultchecked(bool val)
{
    if(val)
    {
        SoftwareDefaults::instance().setDefaultProjectDir(ui->leProjectDirectory->text());
        SoftwareDefaults::instance().modifiedSettings();
    }
}

void NewProject::next()
{
    if(!m_project)
        return;

    QString actualProjectDirectory = QString(ui->leProjectDirectory->text() + "/" + ui->leProjectName->text()).trimmed();

    if(QDir(actualProjectDirectory).exists())
    {
        QMessageBox::StandardButton answer = QMessageBox::warning(this,tr("Project directory already exist"),
                                                                  tr("%1 folder already exist.\n").arg(ui->leProjectName->text()) +
                                                                  tr("Do you still want to use same directory."),
                                                                  QMessageBox::Yes|QMessageBox::No);
        if(answer == QMessageBox::Yes)
        {
            m_project->setProjectName(ui->leProjectName->text());
#ifdef Q_OS_UNIX
            m_project->setFullProjectPath(ui->leProjectDirectory->text() + "/" + ui->leProjectName->text());
#else
            m_project->setFullProjectPath(ui->leProjectDirectory->text() + "\\" + ui->leProjectName->text());
#endif
            close();
            createProject();
        }
    }

    else if(!QDir(ui->leProjectDirectory->text()).mkdir(ui->leProjectName->text()))
    {
        QMessageBox::warning(this,tr("Failed to generate project directory"),
                             tr("Failed to create project directory\n"
                                "please check permissions"));
        close();
    }
    else
    {
        m_project->setProjectName(ui->leProjectName->text());
#ifdef Q_OS_UNIX
        m_project->setFullProjectPath(ui->leProjectDirectory->text() + "/" + ui->leProjectName->text());
#else
        m_project->setFullProjectPath(ui->leProjectDirectory->text() + "\\" + ui->leProjectName->text());
#endif
        close();
        createProject();
    }
}

void NewProject::createProject()
{
    ProjectSettingDialog projectSettingDialog(this->parentWidget(), m_project);
    projectSettingDialog.exec();

    LinkerConfigDialog linkerSettingDialog(this->parentWidget(), m_project);
    linkerSettingDialog.exec();

    StartUp startUpSettingDialog(this->parentWidget(), m_project);
    startUpSettingDialog.exec();

    generateProjectFile();
    m_returnCode = 0;
}

int NewProject::returnCode()
{
    return m_returnCode;
}

void NewProject::generateProjectFile()
{
    QString projectFileName = m_project->projectPath();

#ifdef Q_OS_UNIX
    projectFileName += "/" + m_project->projectName() + ".chops";
#else
    projectFileName += "\\" + m_project->projectName() + ".chops";
#endif

    QFile projectXmlFile(projectFileName);
    if(!projectXmlFile.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
        QMessageBox::warning(0,tr("Failed to save Project!"),
                             tr("Failed to save xml for project.\n Unable to create %.chops file.").arg(m_project->projectName()));
        return;
    }
    else
    {
        QDomDocument doc("Chops v1.0");
        QDomElement projectE = doc.createElement("project");
        doc.appendChild(projectE);

        QDomElement projectNameE = doc.createElement("projectName");
        projectNameE.appendChild(doc.createTextNode(m_project->projectName()));
        projectE.appendChild(projectNameE);

        QDomElement projectPathE = doc.createElement("projectPath");
        projectPathE.appendChild(doc.createTextNode(m_project->projectPath()));
        projectE.appendChild(projectPathE);

        QDomElement toolChainE = doc.createElement("toolChain");
        toolChainE.appendChild(doc.createTextNode(m_project->toolChainPrefix()));
        projectE.appendChild(toolChainE);

        QDomElement armCoreE = doc.createElement("armCore");
        armCoreE.appendChild(doc.createTextNode(m_project->armCore()));
        projectE.appendChild(armCoreE);

        QDomElement cFlagsE = doc.createElement("cFlags");
        cFlagsE.appendChild(doc.createTextNode(m_project->compilerCFlags()));
        projectE.appendChild(cFlagsE);

        QDomElement assemblerFlagsE = doc.createElement("assemblerFlags");
        assemblerFlagsE.appendChild(doc.createTextNode(m_project->assemblerFlags()));
        projectE.appendChild(assemblerFlagsE);

        QDomElement makeBinE = doc.createElement("makeBin");
        makeBinE.appendChild(doc.createTextNode(QString::number((int)m_project->makeBin())));
        projectE.appendChild(makeBinE);

        QDomElement makeHexE = doc.createElement("makeHex");
        makeHexE.appendChild(doc.createTextNode(QString::number((int)m_project->makeHex())));
        projectE.appendChild(makeHexE);

        QDomElement romAddressE = doc.createElement("romAddress");
        romAddressE.appendChild(doc.createTextNode(m_project->romAddress()));
        projectE.appendChild(romAddressE);

        QDomElement ramAddressE = doc.createElement("ramAddress");
        ramAddressE.appendChild(doc.createTextNode(m_project->ramAddress()));
        projectE.appendChild(ramAddressE);

        QDomElement romSizeE = doc.createElement("romSize");
        romSizeE.appendChild(doc.createTextNode(m_project->romSize()));
        projectE.appendChild(romSizeE);

        QDomElement ramSizeE = doc.createElement("ramSize");
        ramSizeE.appendChild(doc.createTextNode(m_project->ramSize()));
        projectE.appendChild(ramSizeE);

        QDomElement extraCodeE = doc.createElement("extraCodeFlag");
        extraCodeE.appendChild(doc.createTextNode(QString::number((int)m_project->extraCodeFlag())));
        projectE.appendChild(extraCodeE);

        QDomElement debugFlagE = doc.createElement("debugFlag");
        debugFlagE.appendChild(doc.createTextNode(QString::number((int)m_project->debugFlag())));
        projectE.appendChild(debugFlagE);

        QDomElement extraCodeTextE = doc.createElement("extraCode");
        extraCodeTextE.appendChild(doc.createTextNode(m_project->extraCode()));
        projectE.appendChild(extraCodeTextE);

        QDomElement undSizeE = doc.createElement("undSize");
        undSizeE.appendChild(doc.createTextNode(m_project->undSize()));
        projectE.appendChild(undSizeE);

        QDomElement irqSizeE = doc.createElement("irqSize");
        irqSizeE.appendChild(doc.createTextNode(m_project->irqSize()));
        projectE.appendChild(irqSizeE);

        QDomElement fiqSizeE = doc.createElement("fiqSize");
        fiqSizeE.appendChild(doc.createTextNode(m_project->fiqSize()));
        projectE.appendChild(fiqSizeE);

        QDomElement svcSizeE = doc.createElement("svcSize");
        svcSizeE.appendChild(doc.createTextNode(m_project->svcSize()));
        projectE.appendChild(svcSizeE);

        QDomElement abrtSizeE = doc.createElement("abrtSize");
        abrtSizeE.appendChild(doc.createTextNode(m_project->abrtSize()));
        projectE.appendChild(abrtSizeE);

        QDomElement usrSizeE = doc.createElement("usrSize");
        usrSizeE.appendChild(doc.createTextNode(m_project->usrSize()));
        projectE.appendChild(usrSizeE);

        QDomElement irqFunctionE = doc.createElement("UseIRQFunction");
        irqFunctionE.appendChild(doc.createTextNode(QString::number((int)m_project->useIrqFunctions())));
        projectE.appendChild(irqFunctionE);

        QDomElement sourcesE = doc.createElement("sources");
        sourcesE.appendChild(doc.createTextNode(m_project->sources().join(" ")));
        projectE.appendChild(sourcesE);

        QDomElement headersE = doc.createElement("headers");
        headersE.appendChild(doc.createTextNode(m_project->headers().join(" ")));
        projectE.appendChild(headersE);

        QTextStream xmlStream(&projectXmlFile);
        xmlStream << doc.toString();

        projectXmlFile.close();
    }

}
