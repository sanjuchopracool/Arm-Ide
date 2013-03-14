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
#include "ProjectData.h"
#include <QDebug>

struct LinkerData
{
    LinkerData() :romOrigin("0x00000000"),ramOrigin("0x40000000"),
        ramSize(32),romSize(512),debug(false),useExtraSectionCode(false) {}

    QString romOrigin;
    QString ramOrigin;
    QString extraCode;
    int ramSize;
    int romSize;
    bool debug;
    bool useExtraSectionCode;
};

LinkerConfigDialog::LinkerConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LinkerConfigDialog)
{
    d = new LinkerData;
    QIntValidator* intValidator = new QIntValidator;
    intValidator->setRange(0,std::numeric_limits<int>::max());
    ui->setupUi(this);

    ui->leRomSize->setValidator(intValidator);
    ui->leRamSize->setValidator(intValidator);

    QRegExp hexAddressRegExp("0{1,1}x{1,1}[0-9a-fA-F]{8,8}");
    QRegExpValidator* hexAddressValidator = new QRegExpValidator(hexAddressRegExp);

    ui->leRamOrigin->setValidator(hexAddressValidator);
    ui->leRomOrigin->setValidator(hexAddressValidator);

    //set variable in ui
    ui->leRamOrigin->setText(d->ramOrigin);
    ui->leRomOrigin->setText(d->romOrigin);
    ui->leRamSize->setText(QString::number(d->ramSize));
    ui->leRomSize->setText(QString::number(d->romSize));
    ui->cbDebug->setChecked(d->debug);
    ui->cbExtraCode->setChecked(d->useExtraSectionCode);
    ui->teExtraCode->setEnabled(d->useExtraSectionCode);

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
        QString linkerDirName;
#ifdef Q_OS_UNIX
        linkerDirName = ProjectData::instance().fullProjectPath + "/linker" ;
#else
        linkerDirName = ProjectData::instance().fullProjectPath + "\\linker" ;
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

void LinkerConfigDialog::applyChanges()
{
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
    QString linkerDirName;
#ifdef Q_OS_UNIX
    linkerDirName = ProjectData::instance().fullProjectPath + "/linker" ;
#else
    linkerDirName = ProjectData::instance().fullProjectPath + "\\linker" ;
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
