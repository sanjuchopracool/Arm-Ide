#include "LinkerConfigDialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LinkerConfigDialog w;
    w.show();
    
    return a.exec();
}
