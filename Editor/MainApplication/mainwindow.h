#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:

    void checkExternalTools();
    void createActions();
    void createMenus();
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
};

#endif // MAINWINDOW_H
