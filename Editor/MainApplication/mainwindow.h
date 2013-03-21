#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>

#include "ProjectExplorer.h"
#include "ProjectModel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void newProject();
    void changeToolChain();
    void changeLinkerSetting();
    void changeStartUpCode();
    void changeCompilerSetting();

private:

    void checkExternalTools();
    void createActions();
    void createMenus();
    void createDocks();
    ////////////////////////////////////////////////

    QMenu* m_fileMenu;
    QMenu* m_projectMenu;
    QMenu* m_settingMenu;

    QAction* m_newProjectAction;
    QAction* m_openProjectAction;

    QAction* m_saveFileAction;
    QAction* m_saveFileAsAction;

    QAction* m_saveAllAction;
    QAction* m_closeAllAction;

    QAction* m_closeProjectAction;
    QAction* m_recentProjects;

    QAction* m_quitAction;


    //project Actions
    QAction* m_linkerSettingAction;
    QAction* m_startUpCodeAction;
    QAction* m_compilerSetting;
    //setting Actions
    QAction* m_toolChainAction;


    /*
     *toolChain prefix for arm-elf-gcc it will be
     *arm-elf- we have to add other extension
     *like for compile arm-elf-gcc
     *for assembler arm-elf-as
     *for objcopy  arm-elf-objcoy
     */
    QString m_toolChainPrefix;
    QDockWidget* m_projectExplorerDock;
    ProjectModel* m_model;
    ProjectExplorer* m_explorer;
};

#endif // MAINWINDOW_H
