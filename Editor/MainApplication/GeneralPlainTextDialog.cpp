#include "GeneralPlainTextDialog.h"
#include "ui_GeneralPlainTextDialog.h"

GeneralPlainTextDialog::GeneralPlainTextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralPlainTextDialog)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    connect(ui->pbOk,SIGNAL(clicked()),this,SLOT(close()));
}

GeneralPlainTextDialog::~GeneralPlainTextDialog()
{
    delete ui;
}

void GeneralPlainTextDialog::setText(const QString &text)
{
    ui->textEdit->insertPlainText(text);
}
