#ifndef PROJECTDATA_H
#define PROJECTDATA_H
#include <QString>
#include <QStringList>
#include <QObject>
#include <QDir>

/*
 * We will create a xml file which will contain all the setting
 * toolchain prefix,compiler flags linker flags and names ofall the files.
 * we have to create a custom view for project dock Widget
 * All the c and assembly files will go into the src folder
 * All header files will go into include folder
 * linker file will be in linker folder
 * Makefile will be in project folder
 * for release the obj will be in release/obj folder and binaries in debug folder
 * for debug mode the obj files will be in release.obj folder and binaries in release
 */

class ProjectData
{
public:
    static ProjectData& instance(){
            static ProjectData theInstance;
            return theInstance;
        }

    QString projectName;
    QString fullProjectPath;
    QStringList sources;
    QStringList headers;
    QString linkerFile;

    QString toolChainPrefix;
    QStringList cFlags;
    QStringList assemblerFlags;

};

struct softwareDefaults
{
    softwareDefaults() : defaultProjectDirectory(QDir::homePath()){}
    QString defaultProjectDirectory;
};

static softwareDefaults  defaultSofwareSetting;
//static ProjectData projectSetting;

static softwareDefaults* defaultSoftwareSetting()
{
    return &defaultSofwareSetting;
}

//static ProjectData* ProjectSetting()
//{
//    return &projectSetting;
//}

#endif // PROJECTDATA_H
