#include "CSyntaxHighLighter.h"

struct HighLightingRule
{
    QString pattern;
    QTextCharFormat format;
};

struct CSyntaxHighLighterData
{
    // initialise it with proper formats
    CSyntaxHighLighterData();

    QVector<HighLightingRule> highLightingRules;
    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    /*
     *char format for all kind of situations
     */
    QTextCharFormat multipleLineCommentFormat;
};



CSyntaxHighLighterData::CSyntaxHighLighterData()
{
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");

    multipleLineCommentFormat.setForeground(MULTILINE_COMMENT_FORMAT);
    multipleLineCommentFormat.setFontItalic(true);
}

CSyntaxHighLighter::CSyntaxHighLighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    d = new CSyntaxHighLighterData;
}

void CSyntaxHighLighter::highlightBlock(const QString& text)
{
    setCurrentBlockState(0);
    int startIndex = 0;
    if(previousBlockState() != 1)
        startIndex = d->commentStartExpression.indexIn(text);

    while(startIndex >= 0)
    {
        int endIndex = d->commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if(endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
            commentLength = endIndex - startIndex + d->commentEndExpression.matchedLength();
        setFormat(startIndex,commentLength,d->multipleLineCommentFormat);
        startIndex = d->commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}

