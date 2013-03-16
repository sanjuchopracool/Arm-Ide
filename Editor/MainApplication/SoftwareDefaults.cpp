#include "SoftwareDefaults.h"
#include <QMessageBox>
#include <QDomDocument>
#include <QDomElement>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

struct SoftwareDefaultsData
{
    QString defaultProjectDir;   //where we will create new Project
};

SoftwareDefaults::SoftwareDefaults(QObject *parent) :
    QObject(parent)
{
    d = new SoftwareDefaultsData;
}

SoftwareDefaults &SoftwareDefaults::instance()
{
    static SoftwareDefaults theInstance;
    return theInstance;
}

void SoftwareDefaults::setDefaultProjectDir(const QString& projectDir)
{
    d->defaultProjectDir = projectDir;
}

QString SoftwareDefaults::defaultProjectDir() const
{
    return d->defaultProjectDir;
}

/*
 * check whether the software directory exist
 * if does not exist create the directory
 */

void SoftwareDefaults::save()
{
    QString projectSettingDir;
    projectSettingDir = QDir::homePath();

#ifdef Q_OS_UNIX
    projectSettingDir += "/.Arm-Ide";
#else
    projectSettingDir += "\.Arm-Ide";
#endif

    if(!QDir(projectSettingDir).exists())
    {
        if(!QDir(QDir::homePath()).mkdir(".Arm-Ide"))
        {
            QMessageBox::warning(0,tr("Failed to save settings!"),
                                 tr("Failed to create directory to save software settings."));
            return;
        }
    }

#ifdef Q_OS_UNIX
    QFile settingFile(projectSettingDir + "/setting.xml");
#else
    QFile settingFile(projectSettingDir + "\setting.xml");
#endif

    if(!settingFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(0,tr("Failed to save settings!"),
                             tr("Failed to save settings for software.\n Unable to create setting.xml file."));
        return;
    }

    QDomDocument setting("SoftwareDefaults");
    QDomElement defaultProjectDirE = setting.createElement("defaultProjectDir");
    QDomText defaultProjectDir = setting.createTextNode(d->defaultProjectDir);
    defaultProjectDirE.appendChild(defaultProjectDir);
    setting.appendChild(defaultProjectDirE);
    QTextStream settingStream(&settingFile);
    settingStream << setting.toString();
    settingFile.close();
}

void SoftwareDefaults::load()
{
    QString settingFileName;
#ifdef Q_OS_UNIX
    settingFileName = QDir::homePath() + "/.Arm-Ide/setting.xml";
#else
    settingFileName += QDir::homePath() +  "\.Arm-Ide\setting.xml";
#endif

    if(!QFile(settingFileName).exists())
    {
        QMessageBox::warning(0,tr("Failed to load settings!"),
                             tr("Failed to load settings for software.\n setting file does not exist."));
    }
    else
    {
        QDomDocument setting("SoftwareDefaults");
        QFile settingFile(settingFileName);
        if(!setting.setContent(&settingFile))
        {
            QMessageBox::warning(0,tr("Failed to load settings!"),
                                 tr("Failed to load settings for software.\n Failed to parse."));
            return;
        }

        QDomElement defaultProjectDirE = setting.firstChildElement("defaultProjectDir");
        d->defaultProjectDir = defaultProjectDirE.text();
    }
}

void SoftwareDefaults::modifiedSettings()
{
    save();
}
