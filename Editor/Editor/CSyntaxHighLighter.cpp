#include "CSyntaxHighLighter.h"

struct HighLightingRule
{
    QRegExp pattern;
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
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat macroFormat;
};



CSyntaxHighLighterData::CSyntaxHighLighterData()
{
    HighLightingRule rule;
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");

    multipleLineCommentFormat.setForeground(MULTILINE_COMMENT_FORMAT);
    multipleLineCommentFormat.setFontItalic(true);

    singleLineCommentFormat.setForeground(SINGLE_LINE_COMMENT_FORMAT);
    singleLineCommentFormat.setFontItalic(true);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highLightingRules.append(rule);

    macroFormat.setForeground(MACRO_FORMAT);
    rule.pattern = QRegExp("#[^\n]*");
    rule.format = macroFormat;
    highLightingRules.append(rule);
}

CSyntaxHighLighter::CSyntaxHighLighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    d = new CSyntaxHighLighterData;
}

void CSyntaxHighLighter::highlightBlock(const QString& text)
{
    foreach(const HighLightingRule& rule, d->highLightingRules)
    {
        QRegExp regExp(rule.pattern);
        int index = regExp.indexIn(text);
        while(index >= 0)
        {
            int length = regExp.matchedLength();
            setFormat(index,length,rule.format);
            index = regExp.indexIn(text, index + length);
        }
    }

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

