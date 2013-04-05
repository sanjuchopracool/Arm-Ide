#include "ProjectData.h"
#include <QDebug>

struct ProjectData
{
    ProjectData()
    {
        sources << "source1.c" << "source2.c" << "startup.S";
        headers << "header.h" << "header2.h";
        projectName = "project.chops";
        newProject = true;
    }

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

    //mys custom
    bool newProject;
};

Project::Project()
{
    d = new ProjectData;
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

void Project::setRomSize(const QString& ROMSize)
{
    d->romSize = ROMSize;
}

QString Project::romSize() const
{
    return d->romSize;
}

void Project::setRamSize(const QString& RAMSize)
{
    d->ramSize = RAMSize;
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

void Project::addIRQFunctions(bool irqFunctionFlag)
{
    d->useIRQFunctions = irqFunctionFlag;
}

bool Project::useIrqFunctions() const
{
    return d->useIRQFunctions;
}

const QStringList Project::sources() const
{
    return d->sources;
}

void Project::addSourceFile(const QString &fileName)
{
    if(fileName.isEmpty())
        return;

    if(!d->sources.contains(fileName))
        d->sources.append(fileName);

}

void Project::addSourceFiles(const QStringList &fileList)
{
    d->headers = fileList;
}

void Project::removeSourceFile(const QString &fileName)
{
    if(fileName.isEmpty())
        return;

    if(d->sources.contains(fileName))
        d->sources.removeAt(d->sources.indexOf(fileName));
}

const QStringList Project::headers() const
{
    return d->headers;
}

void Project::addHeaderFile(const QString &fileName)
{
    if(fileName.isEmpty())
        return;

    if(!d->headers.contains(fileName))
        d->headers.append(fileName);
}

void Project::addHeaderFiles(const QStringList &fileList)
{
    d->sources = fileList;
}

void Project::removeHeaderFile(const QString &fileName)
{
    if(fileName.isEmpty())
        return;

    if(d->headers.contains(fileName))
        d->headers.removeAt(d->headers.indexOf(fileName));
}

void Project::save(QDomDocument& doc)
{
    QDomElement projectE = doc.createElement("project");
    doc.appendChild(projectE);

    QDomElement projectNameE = doc.createElement("projectName");
    projectNameE.appendChild(doc.createTextNode(projectName()));
    projectE.appendChild(projectNameE);

    QDomElement projectPathE = doc.createElement("projectPath");
    projectPathE.appendChild(doc.createTextNode(projectPath()));
    projectE.appendChild(projectPathE);

    QDomElement toolChainE = doc.createElement("toolChain");
    toolChainE.appendChild(doc.createTextNode(toolChainPrefix()));
    projectE.appendChild(toolChainE);

    QDomElement armCoreE = doc.createElement("armCore");
    armCoreE.appendChild(doc.createTextNode(armCore()));
    projectE.appendChild(armCoreE);

    QDomElement cFlagsE = doc.createElement("cFlags");
    cFlagsE.appendChild(doc.createTextNode(compilerCFlags()));
    projectE.appendChild(cFlagsE);

    QDomElement assemblerFlagsE = doc.createElement("assemblerFlags");
    assemblerFlagsE.appendChild(doc.createTextNode(assemblerFlags()));
    projectE.appendChild(assemblerFlagsE);

    QDomElement makeBinE = doc.createElement("makeBin");
    makeBinE.appendChild(doc.createTextNode(QString::number((int)makeBin())));
    projectE.appendChild(makeBinE);

    QDomElement makeHexE = doc.createElement("makeHex");
    makeHexE.appendChild(doc.createTextNode(QString::number((int)makeHex())));
    projectE.appendChild(makeHexE);

    QDomElement romAddressE = doc.createElement("romAddress");
    romAddressE.appendChild(doc.createTextNode(romAddress()));
    projectE.appendChild(romAddressE);

    QDomElement ramAddressE = doc.createElement("ramAddress");
    ramAddressE.appendChild(doc.createTextNode(ramAddress()));
    projectE.appendChild(ramAddressE);

    QDomElement romSizeE = doc.createElement("romSize");
    romSizeE.appendChild(doc.createTextNode(romSize()));
    projectE.appendChild(romSizeE);

    QDomElement ramSizeE = doc.createElement("ramSize");
    ramSizeE.appendChild(doc.createTextNode(ramSize()));
    projectE.appendChild(ramSizeE);

    QDomElement extraCodeE = doc.createElement("extraCodeFlag");
    extraCodeE.appendChild(doc.createTextNode(QString::number((int)extraCodeFlag())));
    projectE.appendChild(extraCodeE);

    QDomElement debugFlagE = doc.createElement("debugFlag");
    debugFlagE.appendChild(doc.createTextNode(QString::number((int)debugFlag())));
    projectE.appendChild(debugFlagE);

    QDomElement extraCodeTextE = doc.createElement("extraCode");
    extraCodeTextE.appendChild(doc.createTextNode(extraCode()));
    projectE.appendChild(extraCodeTextE);

    QDomElement undSizeE = doc.createElement("undSize");
    undSizeE.appendChild(doc.createTextNode(undSize()));
    projectE.appendChild(undSizeE);

    QDomElement irqSizeE = doc.createElement("irqSize");
    irqSizeE.appendChild(doc.createTextNode(irqSize()));
    projectE.appendChild(irqSizeE);

    QDomElement fiqSizeE = doc.createElement("fiqSize");
    fiqSizeE.appendChild(doc.createTextNode(fiqSize()));
    projectE.appendChild(fiqSizeE);

    QDomElement svcSizeE = doc.createElement("svcSize");
    svcSizeE.appendChild(doc.createTextNode(svcSize()));
    projectE.appendChild(svcSizeE);

    QDomElement abrtSizeE = doc.createElement("abrtSize");
    abrtSizeE.appendChild(doc.createTextNode(abrtSize()));
    projectE.appendChild(abrtSizeE);

    QDomElement usrSizeE = doc.createElement("usrSize");
    usrSizeE.appendChild(doc.createTextNode(usrSize()));
    projectE.appendChild(usrSizeE);

    QDomElement irqFunctionE = doc.createElement("UseIRQFunction");
    irqFunctionE.appendChild(doc.createTextNode(QString::number((int)useIrqFunctions())));
    projectE.appendChild(irqFunctionE);

    QDomElement sourcesE = doc.createElement("sources");
    sourcesE.appendChild(doc.createTextNode(sources().join(" ")));
    projectE.appendChild(sourcesE);

    QDomElement headersE = doc.createElement("headers");
    headersE.appendChild(doc.createTextNode(headers().join(" ")));
    projectE.appendChild(headersE);

    setNewProject(false);
}

void Project::load(QDomDocument &doc, QDomElement projectE)
{
    QDomElement projectNameE = projectE.firstChildElement("projectName");
    setProjectName(projectNameE.text());

    QDomElement projectPathE = projectE.firstChildElement("projectPath");
    setFullProjectPath(projectPathE.text());

    QDomElement toolChainE = projectE.firstChildElement("toolChain");
    setTooChainPrefix(toolChainE.text());

    QDomElement armCoreE = projectE.firstChildElement("armCore");
    setArmCore(armCoreE.text());

    QDomElement cFlagsE = projectE.firstChildElement("cFlags");
    setCompilerCFlags(cFlagsE.text());

    QDomElement assemblerFlagsE = projectE.firstChildElement("assemblerFlags");
    setAssemblerFlags(assemblerFlagsE.text());

    QDomElement makeBinE = projectE.firstChildElement("makeBin");
    setMakeBin(makeBinE.text().toInt() ? true : false);

    QDomElement makeHexE = projectE.firstChildElement("makeHex");
    setMakeHex(makeHexE.text().toInt() ? true : false);

    QDomElement romAddressE = projectE.firstChildElement("romAddress");
    setRomAddress(romAddressE.text());

    QDomElement ramAddressE = projectE.firstChildElement("ramAddress");
    setRamAddress(ramAddressE.text());

    QDomElement romSizeE = projectE.firstChildElement("romSize");
    setRomSize(romSizeE.text());
    qDebug() << romSize();

    QDomElement ramSizeE = projectE.firstChildElement("ramSize");
    setRomSize(ramSizeE.text());
    qDebug() << ramSize();

    QDomElement extraCodeE = projectE.firstChildElement("extraCodeFlag");
    addExtraCode(extraCodeE.text().toInt() ? true : false);

    QDomElement debugFlagE = projectE.firstChildElement("debugFlag");
    addDebugCode(debugFlagE.text().toInt() ? true : false);

    QDomElement extraCodeTextE = projectE.firstChildElement("extraCode");
    setExtraCode(extraCodeTextE.text());

    QDomElement undSizeE = projectE.firstChildElement("undSize");
    setUndSize(undSizeE.text());

    QDomElement irqSizeE = projectE.firstChildElement("irqSize");
    setIrqSize(irqSizeE.text());

    QDomElement fiqSizeE = projectE.firstChildElement("fiqSize");
    setFiqSize(fiqSizeE.text());

    QDomElement svcSizeE = projectE.firstChildElement("svcSize");
    setSvcSize(svcSizeE.text());

    QDomElement abrtSizeE = projectE.firstChildElement("abrtSize");
    setAbrtSize(abrtSizeE.text());

    QDomElement usrSizeE = projectE.firstChildElement("usrSize");
    setUsrSize(usrSizeE.text());

    QDomElement irqFunctionE = projectE.firstChildElement("UseIRQFunction");
    addIRQFunctions(irqFunctionE.text().toInt() ? true : false);

    QDomElement sourcesE = projectE.firstChildElement("sources");
    QString sources = sourcesE.text();
    addSourceFiles(sources.split(" "));

    QDomElement headersE = projectE.firstChildElement("headers");
    QString headers = headersE.text();
    addHeaderFiles(headers.split(" "));

    setNewProject(false);
}

void Project::setNewProject(bool isNewProject)
{
    d->newProject = isNewProject;
}

bool Project::isNewProject() const
{
    return d->newProject;
}
