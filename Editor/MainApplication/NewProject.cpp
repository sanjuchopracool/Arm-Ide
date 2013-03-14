#include "NewProject.h"
#include "ui_NewProject.h"
#include <QFileDialog>
#include <ProjectData.h>
#include <QMessageBox>
#include <QDebug>
#include "ProjectSettingDialog.h"
#include "LinkerConfigDialog.h"
#include "StartUp.h"

NewProject::NewProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProject)
{
    ui->setupUi(this);
    ui->pbNext->setEnabled(false);
    setWindowTitle(tr("New Project"));

    QString directory = defaultSoftwareSetting()->defaultProjectDirectory;
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
                                                                 QDir::homePath());
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
        defaultSoftwareSetting()->defaultProjectDirectory = ui->leProjectDirectory->text();
}

void NewProject::next()
{
    QString actualProjectDirectory = QString(ui->leProjectDirectory->text() + "/" + ui->leProjectName->text()).trimmed();

    if(QDir(actualProjectDirectory).exists())
    {
        QMessageBox::StandardButton answer = QMessageBox::warning(this,tr("Project directory already exist"),
                                           tr("%1 folder already exist.\n").arg(ui->leProjectName->text()) +
                                           tr("Do you still want to use same directory."),
                                           QMessageBox::Yes|QMessageBox::No);
        if(answer == QMessageBox::Yes)
        {
            ProjectData::instance().projectName = ui->leProjectName->text();
            ProjectData::instance().fullProjectPath = ui->leProjectDirectory->text() + "/" + ui->leProjectName->text();

            qDebug() <<  &ProjectData::instance() << ProjectData::instance().projectName;
            qDebug() <<  &ProjectData::instance() << ProjectData::instance().fullProjectPath;

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
        ProjectData::instance().projectName = ui->leProjectName->text();
        ProjectData::instance().fullProjectPath = ui->leProjectDirectory->text() + "/" + ui->leProjectName->text();
        close();
        createProject();
    }
}

void NewProject::createProject()
{
    ProjectSettingDialog projectSettingDialog(this);
    projectSettingDialog.exec();

    LinkerConfigDialog linkerSettingDialog(this);
    linkerSettingDialog.exec();

    StartUp startUpSettingDialog(this);
    startUpSettingDialog.exec();
}
