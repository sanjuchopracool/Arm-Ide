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
#include "StartUp.h"
#include <QFileDialog>
#include "SoftwareDefaults.h"
#include "NewProject.h"
#include "ProjectSettingDialog.h"
#include <QTreeWidget>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    showMaximized();

    //check whether configuration for toolchain exist and is valid
    checkExternalTools();
    // create action, menu , toolbars and other things
    createActions();
    createMenus();
    disableProjectMenu();

    //add docks at present only the projectexplorer
    createDocks();

    setCentralWidget(new QWidget());
    //Read settings
    SoftwareDefaults::instance().load();
}

MainWindow::~MainWindow()
{
    
}

Project *MainWindow::currentProject()
{
    return m_currentProject;
}

MainWindow& MainWindow::instance()
{
    static MainWindow theInstance;
    return theInstance;
}

void MainWindow::newProject()
{
    m_currentProject = new Project();

    NewProject projectDialog(this,m_currentProject);
    projectDialog.exec();
    if(projectDialog.returnCode() == 0)
    {
        m_model->addProject(m_currentProject);
        enableProjectMenu();
    }
    else
    {
        delete m_currentProject;
        m_currentProject = 0 ;
        disableProjectMenu();
    }
}

void MainWindow::openProject()
{
    QString projectFileName = QFileDialog::getOpenFileName(this, tr("Choose existing project"),
                                                           SoftwareDefaults::instance().defaultProjectDir(),
                                                           tr("Arm Project (*.chops)"));

    if(projectFileName.isEmpty())
        return;

    QDomDocument projectXml("chops");
    QFile projectFile(projectFileName);
    if(!projectFile.open(QIODevice::ReadOnly))
        qDebug() << "Failed to open project file in read only mode";

    QString xmlString = projectFile.readAll();

    projectFile.close();

    if(xmlString.isEmpty())
        return;

    if(!projectXml.setContent(xmlString))
        qDebug() << "Unable to parse xml file";

    m_currentProject = new Project();

    QDomElement projectE = projectXml.firstChildElement("project");
    m_currentProject->load(projectXml, projectE);

    m_model->addProject(m_currentProject);
    enableProjectMenu();

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

    m_openProjectAction = new QAction(style()->standardIcon(QStyle::SP_DialogOpenButton),
                                      QString(tr("&Open project")),this);
    m_openProjectAction->setShortcuts(QKeySequence::Open);
    connect(m_openProjectAction, SIGNAL(triggered()), this, SLOT(openProject()));



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

    m_startUpCodeAction = new QAction(tr("StartUp Co&de"),this);
    connect(m_startUpCodeAction,SIGNAL(triggered()),this,SLOT(changeStartUpCode()));

    m_compilerSetting = new QAction(tr("&Compiler setting"),this);
    connect(m_compilerSetting,SIGNAL(triggered()),this,SLOT(changeCompilerSetting()));

}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu("&File");
    m_fileMenu->addAction(m_newProjectAction);
    m_fileMenu->addAction(m_openProjectAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_quitAction);


    m_projectMenu = menuBar()->addMenu("&Project");
    m_projectMenu->addAction(m_linkerSettingAction);
    m_projectMenu->addAction(m_startUpCodeAction);
    m_projectMenu->addAction(m_compilerSetting);

    m_settingMenu = menuBar()->addMenu("&Setting");
    m_settingMenu->addAction(m_toolChainAction);
}

void MainWindow::createDocks()
{
//    setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
//    setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
//    setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
//    setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );
    setDockOptions(QMainWindow::ForceTabbedDocks | QMainWindow::VerticalTabs);

    m_projectExplorerDock = new QDockWidget(tr("Projects"),this);
    m_projectExplorerDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_model = new ProjectModel(this);
    m_explorer = new ProjectExplorer(this);
    m_explorer->setModel(m_model);
    m_projectExplorerDock->setWidget(m_explorer);

//    QDockWidget* dock2 = new QDockWidget("Dock 2" , this);
//    dock2->setWidget(new QWidget());
    addDockWidget(Qt::LeftDockWidgetArea,m_projectExplorerDock);
    //    tabifyDockWidget(m_projectExplorerDock,dock2);
}

void MainWindow::disableProjectMenu()
{
    m_projectMenu->setEnabled(false);
}

void MainWindow::enableProjectMenu()
{
    m_projectMenu->setEnabled(true);
}

void MainWindow::changeToolChain()
{
    FirstTimeSetupDialog toolchainDialog(this);
    toolchainDialog.exec();
}

void MainWindow::changeLinkerSetting()
{
    LinkerConfigDialog linkerDialog(this, m_currentProject);
    linkerDialog.exec();
}

void MainWindow::changeStartUpCode()
{
    StartUp startUpCodeDialog(this, m_currentProject);
    startUpCodeDialog.exec();
}

void MainWindow::changeCompilerSetting()
{
    ProjectSettingDialog compilerSettingDialog(this, m_currentProject);
    compilerSettingDialog.exec();
}
