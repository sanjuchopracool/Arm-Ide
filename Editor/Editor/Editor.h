#ifndef EDITOR_H
#define EDITOR_H
#include "EditorConstants.h"
#include "CSyntaxHighLighter.h"
#include <QPlainTextEdit>
class Editor;

/**************************************************************************
 *  left Side Area must have space to show error , line number and code folding
 *  arrow like qtcretor does.
 *      ______________________________________________________
 *     |                |                  |                  |
 *     |debug ,error    | Line number      | Code folding icon|
 *     |warning icon    |                  |                  |
 *     |________________|__________________|__________________|
 *
 *  At present we are not implementing debuggin but we have to show
 *  warning and error icon.Code folding will be done later
 ***************************************************************************/

struct LeftSideAreaData;
class LeftSideArea : public QWidget
{
public:
    LeftSideArea(Editor *editor);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    LeftSideAreaData* d;
};


struct EditorData;

class Editor : public QPlainTextEdit
{
    Q_OBJECT
public:
    Editor(QWidget *parent = 0);
    void leftSideAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();
    
signals:
    
public slots:
    void updateLeftAreaWidth(int);
    void updateLeftSideArea(const QRect& rect, int dy);
protected:
    void resizeEvent(QResizeEvent *e);
private:
    EditorData* d;
    
};

#endif // EDITOR_H
