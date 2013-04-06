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

    m_project->setNewProject(false);
    m_returnCode = 0;
}

int NewProject::returnCode()
{
    return m_returnCode;
}

void NewProject::generateProjectFile()
{
    if(!m_project)
        return;

    m_project->updateProjectFile();
}
