#ifndef PROJECTDATA_H
#define PROJECTDATA_H
#include <QString>
#include <QStringList>
#include <QObject>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QObject>

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

struct ProjectData;

class Project :public QObject
{
    Q_OBJECT
public:
    Project();
    void setFullProjectPath(const QString& projectPath);
    QString projectPath() const;

    void setProjectName(const QString& projectName);
    QString projectName();

    void setTooChainPrefix(const QString& toolChainPrefix);
    QString toolChainPrefix() const;

    void setArmCore(const QString& armCPU);
    QString armCore() const;

    void setCompilerCFlags(const QString& compilerFlags);
    QString compilerCFlags() const;

    void setAssemblerFlags(const QString& assemblerFlags);
    QString assemblerFlags() const;

    void setMakeBin(const bool& bin);
    bool makeBin() const;

    void setMakeHex(const bool& hex);
    bool makeHex() const;

    void setRomAddress(const QString& romAddress);
    QString romAddress() const;

    void setRamAddress(const QString& ramAddress);
    QString ramAddress() const;

    void setRomSize(const QString& ROMSize);
    QString romSize() const;

    void setRamSize(const QString& RAMSize);
    QString ramSize() const;

    void addExtraCode(const bool& extraCodeFlag);
    bool extraCodeFlag() const;

    void addDebugCode(const bool& debugFlag);
    bool debugFlag() const;

    void setExtraCode(const QString& extraCode);
    QString extraCode() const;

    void setUndSize(const QString& undSize);
    QString undSize() const;

    void setIrqSize(const QString& irqSize);
    QString irqSize() const;

    void setFiqSize(const QString& fiqSize);
    QString fiqSize() const;

    void setSvcSize(const QString& svcSize);
    QString svcSize() const;

    void setAbrtSize(const QString& abrtSize);
    QString abrtSize() const;

    void setUsrSize(const QString& usrSize);
    QString usrSize() const;

    void addIRQFunctions(bool irqFunctionFlag);
    bool useIrqFunctions() const;

    //files related functions
    const QStringList sources() const;
    void addSourceFile(const QString& fileName);
    void addSourceFiles(const QStringList& fileList);
    void removeSourceFile(const QString& fileName);

    const QStringList headers() const;
    void addHeaderFile(const QString& fileName);
    void addHeaderFiles(const QStringList& fileList);
    void removeHeaderFile(const QString& fileName);


    //project related file
    void save(QDomDocument& doc);
    void load(QDomDocument& doc, QDomElement projectE);
    void updateProjectFile();

    void setNewProject(bool isNewProject);
    bool isNewProject() const;

private:
    ProjectData* d;

};

#endif // Project_H
