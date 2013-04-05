#include "LinkerConfigDialog.h"
#include "ui_LinkerConfigDialog.h"
#include <QIntValidator>
#include <limits>
#include <QRegExp>
#include <QStyle>
#include <QPixmap>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include "GeneralPlainTextDialog.h"
#include "mainwindow.h"

#include <QDebug>

struct LinkerData
{
    LinkerData() :project(0) {}
    Project* project;
};

LinkerConfigDialog::LinkerConfigDialog(QWidget *parent, Project* project) :
    QDialog(parent),
    ui(new Ui::LinkerConfigDialog)
{
    d = new LinkerData;
    d->project = project;
    QIntValidator* intValidator = new QIntValidator;
    intValidator->setRange(0,std::numeric_limits<int>::max());
    ui->setupUi(this);

    ui->leRomSize->setValidator(intValidator);
    ui->leRamSize->setValidator(intValidator);

    QRegExp hexAddressRegExp("0{1,1}x{1,1}[0-9a-fA-F]{8,8}");
    QRegExpValidator* hexAddressValidator = new QRegExpValidator(hexAddressRegExp);

    ui->leRamOrigin->setValidator(hexAddressValidator);
    ui->leRomOrigin->setValidator(hexAddressValidator);

    if(!d->project || d->project->isNewProject())
    {
        //set variable in ui
        ui->leRamOrigin->setText("0x40000000");
        ui->leRomOrigin->setText("0x00000000");
        ui->leRamSize->setText(QString::number(32));
        ui->leRomSize->setText(QString::number(512));
        ui->cbDebug->setChecked(false);
        ui->cbExtraCode->setChecked(false);
        ui->teExtraCode->setEnabled(false);
    }
    else
    {
        //set variable in ui
        ui->leRamOrigin->setText(d->project->ramAddress());
        ui->leRomOrigin->setText(d->project->romAddress());
        ui->leRamSize->setText(d->project->ramSize());
        ui->leRomSize->setText(d->project->romSize());
        ui->cbDebug->setChecked(d->project->debugFlag());
        ui->cbExtraCode->setChecked(d->project->extraCodeFlag());
        ui->teExtraCode->setEnabled(ui->cbExtraCode->isChecked());
        ui->teExtraCode->setPlainText(d->project->extraCode());
    }

    //set tooltips
    ui->leRamOrigin->setToolTip("must be in hexadecimal format\n example : 0xF1234000");
    ui->leRomOrigin->setToolTip("must be in hexadecimal format\n example : 0xF1234000");
    ui->cbDebug->setToolTip("Check it if u want to Debug");
    ui->cbExtraCode->setToolTip("click it to add extra code in linker section");

    //icons related things
    ui->pbHelp->setIcon(this->style()->standardIcon(QStyle::SP_DialogHelpButton));
    connect(ui->cbExtraCode,SIGNAL(toggled(bool)),ui->teExtraCode,SLOT(setEnabled(bool)));
    connect(ui->pbApply,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->pbPreview,SIGNAL(clicked()),this,SLOT(previewLinkerFile()));
    connect(ui->pbNext,SIGNAL(clicked()),this,SLOT(nextSlot()));

    setWindowTitle(tr("Linker Setting"));
}

LinkerConfigDialog::~LinkerConfigDialog()
{
    delete d;
    delete ui;
}

void LinkerConfigDialog::generateLinkerFile()
{
    if(!d->project)
        return;

        QString linkerDirName;
#ifdef Q_OS_UNIX
        linkerDirName = d->project->projectPath() + "/linker" ;
#else
        linkerDirName = d->project->projectPath() + "\\linker" ;
#endif

    QDir linkerDir(linkerDirName);
    if(!linkerDir.exists())
    {
        if(!QDir::current().mkdir(linkerDirName))
        {
            QMessageBox::warning(this,tr("Failed to generate Linker directory"),
                                 tr("Failed to create linker directory\n"
                                    "please check permissions"));
        }
    }
#ifdef Q_OS_UNIX
            QFile linkerFile(linkerDirName +"/ROM.ld");
#else
            QFile linkerFile(linkerDirName +"\\ROM.ld"); ;
#endif
    if(!linkerFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QMessageBox::warning(this,tr("Failed to open Linker file"),
                             tr("Failed to open linker file in read write mode\n"
                                "please check permissions"));
    }
    else
    {
        QTextStream linkerStream(&linkerFile);
        linkerText(linkerStream);
        linkerFile.close();
    }
}

void LinkerConfigDialog::linkerText(QTextStream &linkerStream)
{
    linkerStream << "MEMORY {    /* memory map */\n";
    linkerStream <<"    ROM (rx)  : ORIGIN = " << ui->leRomOrigin->text();
    linkerStream <<", LENGTH = " << ui->leRomSize->text() << "k\n";
    linkerStream <<"    RAM (rwx)  : ORIGIN = " << ui->leRamOrigin->text();
    linkerStream <<", LENGTH = " << ui->leRamSize->text() << "k\n";
    linkerStream << "}\n";
    linkerStream << "SECTIONS {\n";
    linkerStream << "    . = " << ui->leRomOrigin->text() << ";\n";
    linkerStream << "    .text : {\n "
                 << "        * (vectors);\n"
                 << "         * (.text);\n"
                 << "         . = ALIGN(4);\n"
                 << "     }\n";

    linkerStream << "    .rodata : {\n"
                 << "        * (.rodata);\n"
                 << "        *(.rodata*)      /* all .rodata* sections (constants, strings, etc.)    */\n"
                 << "        *(.glue_7)       /* all .glue_7 sections  (no idea what these are)      */\n"
                 << "        *(.glue_7t)\n"
                 << "        *(.eh_frame) \n"
                 << "        . = ALIGN(4); \n"
                 << "    }\n"
                 << "    flash_sdata = .;\n";
    linkerStream << "    . = " << ui->leRamOrigin->text() << ";\n";
    linkerStream << "        ram_sdata = .;\n"
                 << "   .data : AT (flash_sdata) {\n"
                 << "         * (.data);\n"
                 << "         . = ALIGN(4);\n"
                 << "   }\n"
                 << "   ram_edata = .;\n"
                 << "   data_size = ram_edata - ram_sdata;\n"
                 << "   sbss = .;\n"
                 << "   .bss : {\n"
                 << "        * (.bss);\n"
                 << "        . = ALIGN(4);\n"
                 << "   }\n"
                 << "   ebss = .;\n"
                 << "   bss_size = ebss - sbss;\n"
                 << "       _end = . ;\n"
                 << "  __end = . ;\n"
                 << "    PROVIDE(end = .);\n";
    if(ui->cbExtraCode->isChecked())
        linkerStream << ui->teExtraCode->toPlainText();

    linkerStream << "\n}\n";
}

void LinkerConfigDialog::updateProjectSetting()
{

    d->project->setRamAddress(ui->leRamOrigin->text());
    d->project->setRomAddress(ui->leRomOrigin->text());
    d->project->setRamSize(ui->leRamSize->text());
    d->project->setRomSize(ui->leRomSize->text());
    d->project->addDebugCode(ui->cbDebug->isChecked());
    d->project->addExtraCode(ui->cbExtraCode->isChecked());
    d->project->setExtraCode(ui->teExtraCode->toPlainText());
}

void LinkerConfigDialog::applyChanges()
{
    if(!d->project)
        return;

    updateProjectSetting();
    generateLinkerFile();
    this->close();
}

void LinkerConfigDialog::previewLinkerFile()
{
    GeneralPlainTextDialog generalText(this);
    QString str;
    QTextStream stream(&str);
    linkerText(stream);
    generalText.setText(str);
    generalText.exec();
}

/*
 * IF linker does not exist(new Project) create it else don't modify the existing file
 */

void LinkerConfigDialog::nextSlot()
{
    if(!d->project)
        return;

    updateProjectSetting();

    QString linkerDirName;
#ifdef Q_OS_UNIX
    linkerDirName = d->project->projectPath() + "/linker" ;
#else
    linkerDirName = d->project->projectPath() + "\\linker" ;
#endif

#ifdef Q_OS_UNIX
            QFile linkerFile(linkerDirName +"/ROM.ld");
#else
            QFile linkerFile(linkerDirName +"\\ROM.ld");
#endif
    if(linkerFile.exists())
        close();
    else
        applyChanges();
}
