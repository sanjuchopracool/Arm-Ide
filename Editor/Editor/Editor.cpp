#include "Editor.h"
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QTextBlock>

struct LeftSideAreaData
{
    LeftSideAreaData() : codeEditor(0) {}
    Editor* codeEditor;
};

LeftSideArea::LeftSideArea(Editor *editor): QWidget(editor)
{
    d = new LeftSideAreaData;
    d->codeEditor = editor;
}

QSize LeftSideArea::sizeHint() const
{
    int totalWidth = d->codeEditor->lineNumberAreaWidth();
    totalWidth += DEBUG_ICON_WIDTH + CODE_FOLDING_ICON_WIDTH;
    return QSize(totalWidth,0);
}

void LeftSideArea::paintEvent(QPaintEvent *event)
{
    d->codeEditor->leftSideAreaPaintEvent(event);
}


struct EditorData
{
    EditorData(): leftSideArea(0) {}
    QWidget* leftSideArea;
};

Editor::Editor(QWidget* parent): QPlainTextEdit(parent)
{
    d = new EditorData;
    d->leftSideArea = new LeftSideArea(this);

    //make sure to set NoWrap otherwise bloack will use multiple lines in view area
    setLineWrapMode(QPlainTextEdit::NoWrap);
    //connects all the singnals
    connect(this,SIGNAL(blockCountChanged(int)),this,SLOT(updateLeftAreaWidth(int)));
    connect(this,SIGNAL(updateRequest(QRect,int)),this,SLOT(updateLeftSideArea(QRect,int)));

    //initial setting
    updateLeftAreaWidth(0);
}

void Editor::leftSideAreaPaintEvent(QPaintEvent *event)
{
    /*
     *font size is adjusted in line number
     */
//    QFont font = this->font();
//    font.setPixelSize(20);
//    this->setFont(font);
    QPainter painter(d->leftSideArea);
    painter.fillRect(event->rect(),LINE_NUMBER_BACKGROUND);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    while(block.isValid() && top <= event->rect().bottom())
    {
        if(block.isVisible() && bottom >= event->rect().top())
        {
            QString blockString = QString::number(blockNumber + 1);
            painter.setPen(LINE_NUMBER_COLOR);
            painter.drawText(DEBUG_ICON_WIDTH,top,lineNumberAreaWidth(), fontMetrics().height(), Qt::AlignRight,blockString);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int Editor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1,this->blockCount());
    while( max >= 10)
    {
        max /= 10;
        ++digits;
    }

    /*
     *By default we will give space for 2 digits
     */
    if(digits == 1)
        digits++;

    int onlyTextSpace = fontMetrics().width(QLatin1Char('9'))*digits;
    return onlyTextSpace;
}

void Editor::updateLeftAreaWidth(int)
{
    this->setViewportMargins(d->leftSideArea->sizeHint().width(),0,0,0);
}

void Editor::updateLeftSideArea(const QRect &rect, int dy)
{
    if(dy)
        d->leftSideArea->scroll(0,dy);
    else
    {
        d->leftSideArea->update(0,rect.y(),d->leftSideArea->sizeHint().width(),rect.height());
    }

    if(rect.contains(viewport()->rect())) //this is always true
        updateLeftAreaWidth(0);
}

void Editor::resizeEvent(QResizeEvent *e)
{
        QPlainTextEdit::resizeEvent(e);
        QRect cr = contentsRect();
        d->leftSideArea->setGeometry(cr.left(),cr.top(),d->leftSideArea->sizeHint().width(),cr.height());
}
