#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <vector>
#include <QString>

class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    enum class Language {
        Plain,
        Cpp,
        Python,
        JavaScript,
        Rust,
        Go,
        Markdown
    };

    explicit SyntaxHighlighter(QTextDocument *parent = nullptr);
    void setLanguage(Language lang);
    void setCustomThemeColors(); // Permite ajustar o recargar estilos

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    // Patrones comunes
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    // Formatos visuales (Estilo Dark / Glassmorphism)
    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat operatorFormat;
    QTextCharFormat preprocessorFormat;

    void setupCppRules();
    void setupPythonRules();
    void setupJavaScriptRules();
    void setupRustRules();
    void setupGoRules();
    void setupMarkdownRules();
};