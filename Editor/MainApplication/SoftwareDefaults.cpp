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
    SoftwareDefaultsData() : defaultProjectDir(QDir::homePath()), toolChainPrefix("arm-elf") {}
    QString defaultProjectDir;   //where we will create new Project
    QString toolChainPrefix;
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

void SoftwareDefaults::setToolChainPrefix(const QString& prefix)
{
    d->toolChainPrefix = prefix;
}

QString SoftwareDefaults::toolChainPrefix() const
{
    return d->toolChainPrefix;
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

    QDomDocument setting("Settings");
    QDomElement softwareDefaultsE = setting.createElement("SoftwareDefaults");

    QDomElement defaultProjectDirE = setting.createElement("defaultProjectDir");
    defaultProjectDirE.setAttribute("val", d->defaultProjectDir);
    softwareDefaultsE.appendChild(defaultProjectDirE);

    QDomElement toolChainPrefixE = setting.createElement("toolChainPrefix");
    toolChainPrefixE.setAttribute("val", d->toolChainPrefix);
    softwareDefaultsE.appendChild(toolChainPrefixE);

    setting.appendChild(softwareDefaultsE);
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
        QDomDocument setting("Settings");
        QFile settingFile(settingFileName);
        if(!setting.setContent(&settingFile))
        {
            QMessageBox::warning(0,tr("Failed to load settings!"),
                                 tr("Failed to load settings for software.\n") + tr("Failed to parse."));
            return;
        }

        QDomElement SoftwareDefaultsE = setting.firstChildElement("SoftwareDefaults");

        QDomElement defaultProjectDirE = SoftwareDefaultsE.firstChildElement("defaultProjectDir");
        d->defaultProjectDir = defaultProjectDirE.attribute("val");


        QDomElement toolChainPrefixE = SoftwareDefaultsE.firstChildElement("toolChainPrefix");
        d->toolChainPrefix = toolChainPrefixE.attribute("val");

    }
}

void SoftwareDefaults::modifiedSettings()
{
    save();
}
