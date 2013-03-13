#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QDialog>

namespace Ui {
class NewProject;
}

class NewProject : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewProject(QWidget *parent = 0);
    ~NewProject();
    
private:
    Ui::NewProject *ui;

public slots:
    void browse();
    void projectText();
    void setDefaultchecked(bool val);
    void next();
    void createProject();
};

#endif // NEWPROJECT_H
