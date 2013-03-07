#include "Editor.h"
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QTextBlock>
#include <QAbstractItemModel>
#include <QStringListModel>
#include <QApplication>
#include <QScrollBar>

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
    EditorData(): leftSideArea(0), syntaxHighlighter(0), completer(0) {

    }
    QWidget* leftSideArea;

    //highlighter
    QSyntaxHighlighter* syntaxHighlighter;

    //completer
    QCompleter* completer;
};

Editor::Editor(QWidget* parent): QPlainTextEdit(parent)
{
    d = new EditorData;
    d->leftSideArea = new LeftSideArea(this);

    //completer things
    d->completer = new QCompleter(this);
    d->completer->setWrapAround(false);
    d->completer->setModel(modelFromFile(":/Completer/CCompleterWords.txt"));
    d->completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    d->completer->setCaseSensitivity(Qt::CaseInsensitive);
    this->setCompleter(d->completer);

    //make sure to set NoWrap otherwise bloack will use multiple lines in view area
    setLineWrapMode(QPlainTextEdit::NoWrap);

    d->syntaxHighlighter = new CSyntaxHighLighter(this->document());

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

void Editor::setCompleter(QCompleter *c)
{
    if(d->completer)
        disconnect(d->completer,0,this,0);

    d->completer = c;

    if(!d->completer)
        return;
    d->completer->setWidget(this);
    d->completer->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(d->completer, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
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

void Editor::insertCompletion(const QString &completion)
{
    if(d->completer->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - d->completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);

}

void Editor::resizeEvent(QResizeEvent *e)
{
        QPlainTextEdit::resizeEvent(e);
        QRect cr = contentsRect();
        d->leftSideArea->setGeometry(cr.left(),cr.top(),d->leftSideArea->sizeHint().width(),cr.height());
}

void Editor::focusInEvent(QFocusEvent *e)
{
    if(d->completer)
        d->completer->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

void Editor::keyPressEvent(QKeyEvent *e)
{
    if (d->completer && d->completer->popup()->isVisible()) {
             // The following keys are forwarded by the completer to the widget
            switch (e->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                 e->ignore();
                 return; // let the completer do default behavior
            default:
                break;
            }
         }

         bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
         if (!d->completer || !isShortcut) // do not process the shortcut when we have a completer
             QPlainTextEdit::keyPressEvent(e);

         const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
         if (!d->completer || (ctrlOrShift && e->text().isEmpty()))
             return;

         static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
         bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
         QString completionPrefix = textUnderCursor();

         if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                           || eow.contains(e->text().right(1)))) {
             d->completer->popup()->hide();
             return;
         }

         if (completionPrefix != d->completer->completionPrefix()) {
             d->completer->setCompletionPrefix(completionPrefix);
             d->completer->popup()->setCurrentIndex(d->completer->completionModel()->index(0, 0));
         }
         QRect cr = cursorRect();
         cr.setWidth(d->completer->popup()->sizeHintForColumn(0)
                     + d->completer->popup()->verticalScrollBar()->sizeHint().width());
         d->completer->complete(cr); // popup it up!
}


QCompleter *Editor::completer() const
{
    return d->completer;
}

QString Editor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

QAbstractItemModel *Editor::modelFromFile(const QString& fileName)
{
    QFile file(fileName);
         if (!file.open(QFile::ReadOnly))
             return new QStringListModel(d->completer);

     #ifndef QT_NO_CURSOR
         QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     #endif
         QStringList words;

         while (!file.atEnd()) {
             QByteArray line = file.readLine();
             if (!line.isEmpty())
                 words << line.trimmed();
         }

     #ifndef QT_NO_CURSOR
         QApplication::restoreOverrideCursor();
     #endif
         return new QStringListModel(words, d->completer);
}
