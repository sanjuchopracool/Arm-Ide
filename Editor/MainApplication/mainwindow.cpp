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



    //setting actions
    m_toolChainAction = new QAction(tr("Change &toolchain"),this);
    connect(m_toolChainAction,SIGNAL(triggered()),this,SLOT(changeToolChain()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(m_newProjectAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_quitAction);


    settingMenu = menuBar()->addMenu("&Setting");
    settingMenu->addAction(m_toolChainAction);
}

void MainWindow::changeToolChain()
{
    FirstTimeSetupDialog toolchainDialog;
    toolchainDialog.exec();
}
