#include "SyntaxHighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {
    setCustomThemeColors();
    setLanguage(Language::Cpp); // Lenguaje por defecto
}

void SyntaxHighlighter::setCustomThemeColors() {
    // Paleta de colores vibrante y moderna adaptada a entornos oscuros
    keywordFormat.setForeground(QColor("#FF79C6")); // Rosa neon para palabras clave
    keywordFormat.setFontWeight(QFont::Bold);

    classFormat.setForeground(QColor("#8BE9FD"));   // Azul cian para Clases / Tipos
    classFormat.setFontWeight(QFont::Bold);

    functionFormat.setForeground(QColor("#50FA7B")); // Verde brillante para funciones
    
    singleLineCommentFormat.setForeground(QColor("#6272A4")); // Gris azulado para comentarios
    singleLineCommentFormat.setFontItalic(true);

    multiLineCommentFormat.setForeground(QColor("#6272A4"));
    multiLineCommentFormat.setFontItalic(true);

    quotationFormat.setForeground(QColor("#F1FA8C")); // Amarillo pastel para cadenas de texto
    
    numberFormat.setForeground(QColor("#BD93F9"));    // Morado para números
    
    operatorFormat.setForeground(QColor("#FFB86C"));  // Naranja para operadores
    
    preprocessorFormat.setForeground(QColor("#FF5555")); // Rojo suave para macros/imports
}

void SyntaxHighlighter::setLanguage(Language lang) {
    highlightingRules.clear();

    switch (lang) {
        case Language::Cpp:
            setupCppRules();
            break;
        case Language::Python:
            setupPythonRules();
            break;
        case Language::JavaScript:
            setupJavaScriptRules();
            break;
        case Language::Rust:
            setupRustRules();
            break;
        case Language::Go:
            setupGoRules();
            break;
        case Language::Markdown:
            setupMarkdownRules();
            break;
        case Language::Plain:
        default:
            break;
    }
    rehighlight();
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    // Aplicar reglas basadas en regex
    for (const auto &rule : std::as_const(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    // Manejo de comentarios multilínea (ej. C/C++, Rust)
    setCurrentBlockState(0);
    if (!commentStartExpression.pattern().isEmpty()) {
        int startIndex = 0;
        if (previousBlockState() != 1) {
            startIndex = text.indexOf(commentStartExpression);
        }

        while (startIndex >= 0) {
            QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - match.capturedLength() - startIndex;
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
    }
}

// --- CONFIGURACIÓN DE LENGUAJES ---

void SyntaxHighlighter::setupCppRules() {
    HighlightingRule rule;

    // Palabras clave de C++
    QStringList keywordPatterns = {
        R"(\bchar\b)", R"(\bclass\b)", R"(\bconst\b)", R"(\bdouble\b)", R"(\benum\b)", R"(\bexplicit\b)",
        R"(\bfriend\b)", R"(\binline\b)", R"(\bint\b)", R"(\blong\b)", R"(\bnamespace\b)", R"(\bprivate\b)",
        R"(\bprotected\b)", R"(\bpublic\b)", R"(\bshort\b)", R"(\bsignals\b)", R"(\bsigned\b)", R"(\bslots\b)",
        R"(\bstatic\b)", R"(\bstruct\b)", R"(\btemplate\b)", R"(\btypedef\b)", R"(\btypename\b)", R"(\bunion\b)",
        R"(\bunsigned\b)", R"(\bvirtual\b)", R"(\bvoid\b)", R"(\bvolatile\b)", R"(\bbool\b)", R"(\btrue\b)", R"(\bfalse\b)",
        R"(\bif\b)", R"(\belse\b)", R"(\bfor\b)", R"(\bwhile\b)", R"(\breturn\b)", R"(\bswitch\b)", R"(\bcase\b)",
        R"(\bbreak\b)", R"(\bcontinue\b)", R"(\bnew\b)", R"(\bdelete\b)", R"(\btry\b)", R"(\bcatch\b)", R"(\bauto\b)"
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Comentarios
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));

    // Cadenas de texto
    rule.pattern = QRegularExpression(QStringLiteral("\".*?\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // Funciones
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Números
    rule.pattern = QRegularExpression(QStringLiteral("\\b\\d+([.eE]\\d+)?\\b"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::setupPythonRules() {
    HighlightingRule rule;

    QStringList keywordPatterns = {
        R"(\band\b)", R"(\bas\b)", R"(\bassert\b)", R"(\bbreak\b)", R"(\bclass\b)", R"(\bcontinue\b)",
        R"(\bdef\b)", R"(\bdel\b)", R"(\belif\b)", R"(\belse\b)", R"(\bexcept\b)", R"(\bFalse\b)",
        R"(\bfinally\b)", R"(\bfor\b)", R"(\bfrom\b)", R"(\bglobal\b)", R"(\bif\b)", R"(\bimport\b)",
        R"(\bin\b)", R"(\bis\b)", R"(\blambda\b)", R"(\bNone\b)", R"(\bnonlocal\b)", R"(\bnot\b)",
        R"(\bor\b)", R"(\bpass\b)", R"(\braise\b)", R"(\breturn\b)", R"(\bTrue\b)", R"(\btry\b)",
        R"(\bwhile\b)", R"(\bwith\b)", R"(\byield\b)", R"(\bself\b)", R"(\basync\b)", R"(\bawait\b)"
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Comentarios Python (#)
    rule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // Strings (simples y dobles, incluyendo triples)
    rule.pattern = QRegularExpression(QStringLiteral("\".*?\"|'.*?'"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // Funciones def
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z_][A-Za-z0-9_]*(?=\\s*\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Números
    rule.pattern = QRegularExpression(QStringLiteral("\\b\\d+(\\.\\d+)?\\b"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::setupJavaScriptRules() {
    HighlightingRule rule;
    QStringList keywordPatterns = {
        R"(\bvar\b)", R"(\blet\b)", R"(\bconst\b)", R"(\bfunction\b)", R"(\breturn\b)", R"(\bif\b)",
        R"(\belse\b)", R"(\bfor\b)", R"(\bwhile\b)", R"(\bdo\b)", R"(\bswitch\b)", R"(\bcase\b)",
        R"(\bdefault\b)", R"(\bbreak\b)", R"(\bcontinue\b)", R"(\btry\b)", R"(\bcatch\b)", R"(\bfinally\b)",
        R"(\bclass\b)", R"(\bextends\b)", R"(\bnew\b)", R"(\bthis\b)", R"(\bsuper\b)", R"(\bimport\b)",
        R"(\bexport\b)", R"(\basync\b)", R"(\bawait\b)", R"(\btypeof\b)", R"(\binstanceof\b)",
        R"(\btrue\b)", R"(\bfalse\b)", R"(\bundefined\b)", R"(\bnull\b)"
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Comentarios
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));

    // Strings (comillas dobles, simples y template literals ``)
    rule.pattern = QRegularExpression(QStringLiteral("\".*?\"|'.*?'|`.*?`"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // Números
    rule.pattern = QRegularExpression(QStringLiteral("\\b\\d+(\\.\\d+)?\\b"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::setupRustRules() {
    HighlightingRule rule;
    QStringList keywordPatterns = {
        R"(\bfn\b)", R"(\blet\b)", R"(\bmut\b)", R"(\bpub\b)", R"(\buse\b)", R"(\bmod\b)", R"(\bstruct\b)",
        R"(\benum\b)", R"(\btrait\b)", R"(\bimpl\b)", R"(\bmatch\b)", R"(\bif\b)", R"(\belse\b)", R"(\bwhile\b)",
        R"(\bfor\b)", R"(\bin\b)", R"(\bloop\b)", R"(\bge\b)", R"(\breturn\b)", R"(\bref\b)", R"(\bwhere\b)",
        R"(\bas\b)", R"(\bconst\b)", R"(\bstatic\b)", R"(\bunsafe\b)", R"(\basync\b)", R"(\bawait\b)",
        R"(\btrue\b)", R"(\bfalse\b)", R"(\bSelf\b)", R"(\bself\b)"
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\".*?\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\b\\d+(_?\\d+)*\\b"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::setupGoRules() {
    HighlightingRule rule;
    QStringList keywordPatterns = {
        R"(\bbreak\b)", R"(\bcase\b)", R"(\bchan\b)", R"(\bconst\b)", R"(\bcontinue\b)", R"(\bdefault\b)",
        R"(\bdefer\b)", R"(\belse\b)", R"(\bfallthrough\b)", R"(\bfor\b)", R"(\bfunc\b)", R"(\bgo\b)",
        R"(\bgoto\b)", R"(\bif\b)", R"(\bimport\b)", R"(\binterface\b)", R"(\bmap\b)", R"(\bpackage\b)",
        R"(\brange\b)", R"(\breturn\b)", R"(\bselect\b)", R"(\bstruct\b)", R"(\bswitch\b)", R"(\btype\b)",
        R"(\bvar\b)", R"(\btrue\b)", R"(\bfalse\b)", R"(\bnil\b)"
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\".*?\"|`.*?`"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::setupMarkdownRules() {
    HighlightingRule rule;
    // Títulos de Markdown (# Header)
    rule.pattern = QRegularExpression(QStringLiteral("^#+\\s+.*$"));
    rule.format = keywordFormat;
    highlightingRules.append(rule);

    // Enlaces [texto](url)
    rule.pattern = QRegularExpression(QStringLiteral("\\[([^\\]]+)\\]\\(([^\\)]+)\\)"));
    rule.format = classFormat;
    highlightingRules.append(rule);
}