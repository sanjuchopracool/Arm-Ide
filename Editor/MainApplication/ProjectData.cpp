#include "ProjectData.h"

struct ProjectData
{
    QString projectName;
    QString fullProjectPath;
    QStringList sources;
    QStringList headers;

    //compiler related things
    QString toolChainPrefix;
    QString armCPU;
    QString cFlags;
    QString asFlags;
    bool makeBin;
    bool makeHex;

    //linker related thing
    QString romOrigin;
    QString ramOrigin;
    QString ramSize;
    QString romSize;
    bool haveExtraCode;
    bool debug;
    QString extraCode;

    //startup related thing
    QString undSize;
    QString irqSize;
    QString fiqSize;
    QString svcSize;
    QString abrtSize;
    QString usrSize;
    bool useIRQFunctions;
};


Project &Project::instance()
{
    static Project theInstance;
    return theInstance;
}

Project::Project()
{
    d = new ProjectData();
}

void Project::setFullProjectPath(const QString &projectPath)
{
    d->fullProjectPath = projectPath;
}

QString Project::projectPath() const
{
    return d->fullProjectPath;
}

void Project::setProjectName(const QString &projectName)
{
    d->projectName = projectName;
}

QString Project::projectName()
{
    return d->projectName;
}

void Project::setTooChainPrefix(const QString &toolChainPrefix)
{
    d->toolChainPrefix = toolChainPrefix;
}

QString Project::toolChainPrefix() const
{
    return d->toolChainPrefix;
}

void Project::setArmCore(const QString &armCPU)
{
    d->armCPU = armCPU;
}

QString Project::armCore() const
{
    return d->armCPU;
}

void Project::setCompilerCFlags(const QString& compilerFlags)
{
    d->cFlags = compilerFlags;
}

QString Project::compilerCFlags() const
{
    return d->cFlags;
}

void Project::setAssemblerFlags(const QString& assemblerFlags)
{
    d->asFlags = assemblerFlags;
}

QString Project::assemblerFlags() const
{
    return d->asFlags;
}

void Project::setMakeBin(const bool &bin)
{
    d->makeBin = bin;
}

bool Project::makeBin() const
{
    return d->makeBin;
}

void Project::setMakeHex(const bool &hex)
{
    d->makeHex = hex;
}

bool Project::makeHex() const
{
    return d->makeHex;
}

void Project::setRomAddress(const QString &romAddress)
{
    d->romOrigin = romAddress;
}

QString Project::romAddress() const
{
    return d->romOrigin;
}

void Project::setRamAddress(const QString &ramAddress)
{
    d->ramOrigin = ramAddress;
}

QString Project::ramAddress() const
{
    return d->ramOrigin;
}

void Project::setRomSize(const QString &romSize)
{
    d->romSize = romSize;
}

QString Project::romSize() const
{
    return d->romSize;
}

void Project::setRamSize(const QString &ramSize)
{
    d->ramSize = ramSize;
}

QString Project::ramSize() const
{
    return d->ramSize;
}

void Project::addExtraCode(const bool &extraCodeFlag)
{
    d->haveExtraCode = extraCodeFlag;
}

bool Project::extraCodeFlag() const
{
    return d->haveExtraCode;
}

void Project::addDebugCode(const bool &debugFlag)
{
    d->debug = debugFlag;
}

bool Project::debugFlag() const
{
    return d->debug;
}

void Project::setExtraCode(const QString& extraCode)
{
    d->extraCode = extraCode;
}

QString Project::extraCode() const
{
    return d->extraCode;
}

void Project::setUndSize(const QString& undSize)
{
    d->undSize = undSize;
}

QString Project::undSize() const
{
    return d->undSize;
}

void Project::setIrqSize(const QString& irqSize)
{
    d->irqSize = irqSize;
}

QString Project::irqSize() const
{
    return d->irqSize;
}

void Project::setFiqSize(const QString& fiqSize)
{
    d->fiqSize = fiqSize;
}

QString Project::fiqSize() const
{
    return d->fiqSize;
}

void Project::setSvcSize(const QString& svcSize)
{
    d->svcSize = svcSize;
}

QString Project::svcSize() const
{
    return d->svcSize;
}

void Project::setAbrtSize(const QString& abrtSize)
{
    d->abrtSize = abrtSize;
}

QString Project::abrtSize() const
{
    return d->abrtSize;
}

void Project::setUsrSize(const QString& usrSize)
{
    d->usrSize = usrSize;
}

QString Project::usrSize() const
{
    return d->usrSize;
}
