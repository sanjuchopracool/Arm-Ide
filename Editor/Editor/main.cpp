#include <QApplication>
#include "Editor.h"

int main(int argc,char** argv)
{
    QApplication app(argc,argv);
    Editor editor;
    editor.resize(700,500);
    editor.show();
    return app.exec();

}
