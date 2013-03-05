#ifndef CSYNTAXHIGHLIGHTER_H
#define CSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include "EditorConstants.h"

struct HighLightingRule;
struct CSyntaxHighLighterData;

class CSyntaxHighLighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    CSyntaxHighLighter(QTextDocument* parent = 0);
    
protected:
    void highlightBlock(const QString& text);
signals:
    
public slots:

private:
    CSyntaxHighLighterData* d;
    
};

#endif // CSYNTAXHIGHLIGHTER_H
