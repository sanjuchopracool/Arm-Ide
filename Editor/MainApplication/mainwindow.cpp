#include "mainwindow.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QStyle>
#include <QDebug>
#include <QFile>
#include "FirstTimeSetupDialog.h"
#include <QDomDocument>
#include "LinkerConfigDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    showMaximized();

    //check whether configuration for toolchain exist and is valid
    checkExternalTools();
    // create action, menu , toolbars and other things
    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::newProject()
{
    qDebug() << "Create a new Project";
}

void MainWindow::checkExternalTools()
{
    QFile toolChainFile("ToolChain.xml");
    if(!toolChainFile.exists())
    {
        FirstTimeSetupDialog toolChainSelectDialog(this);
        toolChainSelectDialog.exec();
    }
    else
    {
        QDomDocument d("ToolChain");
        if(!toolChainFile.open(QIODevice::ReadOnly))
            return;
        if(!d.setContent(&toolChainFile))
        {
            FirstTimeSetupDialog toolChainSelectDialog(this);
            toolChainSelectDialog.exec();
        }
    }
}

void MainWindow::createActions()
{

    m_newProjectAction = new QAction(style()->standardIcon(QStyle::SP_FileIcon),
                                     QString(tr("&New project")),this);
    m_newProjectAction->setShortcuts(QKeySequence::New);
    m_newProjectAction->setStatusTip("Create a new Project");
    connect(m_newProjectAction,SIGNAL(triggered()),this,SLOT(newProject()));


    //quit Action
    m_quitAction = new QAction(tr("E&xit"),this);
    m_quitAction->setShortcuts(QKeySequence::Quit);
    connect(m_quitAction,SIGNAL(triggered()),qApp,SLOT(quit()));



    //project Actions
    m_linkerSettingAction = new QAction(tr("&Linker Setting"),this);
    connect(m_linkerSettingAction,SIGNAL(triggered()),this,SLOT(changeLinkerSetting()));
    //setting actions
    m_toolChainAction = new QAction(tr("Change &toolchain"),this);
    connect(m_toolChainAction,SIGNAL(triggered()),this,SLOT(changeToolChain()));

}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu("&File");
    m_fileMenu->addAction(m_newProjectAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_quitAction);


    m_projectMenu = menuBar()->addMenu("&Project");
    m_projectMenu->addAction(m_linkerSettingAction);

    m_settingMenu = menuBar()->addMenu("&Setting");
    m_settingMenu->addAction(m_toolChainAction);
}

void MainWindow::changeToolChain()
{
    FirstTimeSetupDialog toolchainDialog(this);
    toolchainDialog.exec();
}

void MainWindow::changeLinkerSetting()
{
    LinkerConfigDialog linkerDialog(this);
    linkerDialog.exec();
}
