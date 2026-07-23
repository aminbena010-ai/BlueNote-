#include "SyntaxHighlighter.h"

namespace Ui {

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {
    initFormats();
}

void SyntaxHighlighter::initFormats() {
    m_keywordFormat.setForeground(QColor(86, 156, 214));
    m_keywordFormat.setFontWeight(QFont::Bold);

    m_typeFormat.setForeground(QColor(78, 201, 176));

    m_stringFormat.setForeground(QColor(206, 145, 120));

    m_commentFormat.setForeground(QColor(106, 153, 85));
    m_commentFormat.setFontItalic(true);

    m_numberFormat.setForeground(QColor(181, 206, 168));

    m_functionFormat.setForeground(QColor(220, 220, 170));
}

void SyntaxHighlighter::setLanguageFromExtension(const QString &ext) {
    m_highlightingRules.clear();
    QString extLower = ext.toLower();

    if (extLower == "cpp" || extLower == "h" || extLower == "hpp" || extLower == "c" || extLower == "cc") {
        setupCppRules();
    } else if (extLower == "py") {
        setupPythonRules();
    } else if (extLower == "json") {
        setupJsonRules();
    }

    rehighlight();
}

void SyntaxHighlighter::setupCppRules() {
    HighlightingRule rule;

    QStringList typePatterns = {
        "\\bvoid\\b", "\\bint\\b", "\\bchar\\b", "\\bfloat\\b",
        "\\bdouble\\b", "\\bbool\\b", "\\bauto\\b", "\\blong\\b",
        "\\bshort\\b", "\\bsize_t\\b", "\\bint32_t\\b", "\\bint64_t\\b"
    };
    for (const QString &pattern : typePatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = m_typeFormat;
        m_highlightingRules.append(rule);
    }

    QStringList keywordPatterns = {
        "\\bclass\\b", "\\bstruct\\b", "\\bpublic\\b", "\\bprivate\\b",
        "\\bprotected\\b", "\\bnamespace\\b", "\\btemplate\\b", "\\btypename\\b",
        "\\busing\\b", "\\bif\\b", "\\belse\\b", "\\bfor\\b", "\\bwhile\\b",
        "\\breturn\\b", "\\bswitch\\b", "\\bcase\\b", "\\bbreak\\b",
        "\\bconst\\b", "\\boverride\\b", "\\bexplicit\\b", "\\bconstexpr\\b"
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = m_keywordFormat;
        m_highlightingRules.append(rule);
    }

    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = m_functionFormat;
    m_highlightingRules.append(rule);

    rule.pattern = QRegularExpression("\\b\\d+(\\.\\d+)?f?\\b");
    rule.format = m_numberFormat;
    m_highlightingRules.append(rule);

    rule.pattern = QRegularExpression("\".*?\"");
    rule.format = m_stringFormat;
    m_highlightingRules.append(rule);

    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = m_commentFormat;
    m_highlightingRules.append(rule);
}

void SyntaxHighlighter::setupPythonRules() {
    HighlightingRule rule;

    QStringList keywords = {
        "\\bdef\\b", "\\bclass\\b", "\\bimport\\b", "\\bfrom\\b",
        "\\breturn\\b", "\\bif\\b", "\\belif\\b", "\\belse\\b",
        "\\bfor\\b", "\\bwhile\\b", "\\btry\\b", "\\bexcept\\b",
        "\\bwith\\b", "\\bas\\b", "\\bNone\\b", "\\bTrue\\b", "\\bFalse\\b"
    };
    for (const QString &pattern : keywords) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = m_keywordFormat;
        m_highlightingRules.append(rule);
    }

    // Funciones
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = m_functionFormat;
    m_highlightingRules.append(rule);

    // Strings (dobles y simples)
    rule.pattern = QRegularExpression("\".*?\"|'.*?'");
    rule.format = m_stringFormat;
    m_highlightingRules.append(rule);

    // Comentarios #
    rule.pattern = QRegularExpression("#[^\n]*");
    rule.format = m_commentFormat;
    m_highlightingRules.append(rule);
}

void SyntaxHighlighter::setupJsonRules() {
    HighlightingRule rule;

    rule.pattern = QRegularExpression("\".*?\"(?=\\s*:)");
    rule.format = m_keywordFormat;
    m_highlightingRules.append(rule);

    rule.pattern = QRegularExpression("(?<=:\\s*)\".*?\"");
    rule.format = m_stringFormat;
    m_highlightingRules.append(rule);

    rule.pattern = QRegularExpression("\\b(true|false|null|\\d+(\\.\\d+)?)\\b");
    rule.format = m_numberFormat;
    m_highlightingRules.append(rule);
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : m_highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

} // namespace Ui