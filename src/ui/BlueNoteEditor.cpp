#include "BlueNoteEditor.h"
#include <QPainter>
#include <QTextBlock>
#include <QScrollBar>
#include <QFontMetrics>

namespace Ui {

BlueNoteEditor::BlueNoteEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    m_lineNumberArea = new LineNumberArea(this);

    connect(this, &BlueNoteEditor::blockCountChanged, this, &BlueNoteEditor::updateLineNumberAreaWidth);
    connect(this, &BlueNoteEditor::updateRequest, this, &BlueNoteEditor::updateLineNumberArea);
    connect(this, &BlueNoteEditor::cursorPositionChanged, this, &BlueNoteEditor::highlightCurrentLine);
    connect(document(), &QTextDocument::contentsChange, this, &BlueNoteEditor::handleContentsChange);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    setLineWrapMode(QPlainTextEdit::NoWrap);
    
    QFont font("Monospace");
    font.setStyleHint(QFont::Monospace);
    font.setPointSizeF(10.5);
    setFont(font);
}

int BlueNoteEditor::lineNumberAreaWidth() {
    int digits = 1;
    int maxBlock = qMax(1, blockCount());
    while (maxBlock >= 10) {
        maxBlock /= 10;
        digits++;
    }

    int space = 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    int stateBarWidth = 5;  
    int foldIconWidth = 26;

    return stateBarWidth + space + foldIconWidth;
}

void BlueNoteEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void BlueNoteEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy) {
        m_lineNumberArea->scroll(0, dy);
    } else {
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void BlueNoteEditor::resizeEvent(QResizeEvent *event) {
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void BlueNoteEditor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(255, 255, 255, 12);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
    m_lineNumberArea->update();
}

void BlueNoteEditor::handleContentsChange(int position, int charsRemoved, int charsAdded) {
    Q_UNUSED(charsRemoved);
    Q_UNUSED(charsAdded);

    QTextBlock startBlock = document()->findBlock(position);
    if (startBlock.isValid()) {
        int startLine = startBlock.blockNumber();
        m_lineStates[startLine] = LineChangeState::ModifiedUnsaved;
        m_lineNumberArea->update();
    }
}

void BlueNoteEditor::markAllLinesAsSaved() {
    for (auto it = m_lineStates.begin(); it != m_lineStates.end(); ++it) {
        if (it.value() == LineChangeState::ModifiedUnsaved) {
            it.value() = LineChangeState::ModifiedSaved;
        }
    }
    m_lineNumberArea->update();
}

void BlueNoteEditor::handleMarginClick(int yPos) {
    QTextBlock block = firstVisibleBlock();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();

    while (block.isValid()) {
        int bottom = top + (int) blockBoundingRect(block).height();

        if (block.isVisible() && yPos >= top && yPos <= bottom) {
            int blockNum = block.blockNumber();
            QString foldType;
            if (isFoldStart(block, foldType) || m_foldedBlocks.contains(blockNum)) {
                toggleFold(blockNum);
            }
            break;
        }

        top = bottom;
        block = block.next();
    }
}

QString BlueNoteEditor::sanitizeLine(const QString &line) const {
    QString result;
    bool inString = false;
    QChar stringChar;

    for (int i = 0; i < line.length(); ++i) {
        QChar c = line[i];

        if (!inString && (c == '"' || c == '\'')) {
            inString = true;
            stringChar = c;
            result.append(' ');
            continue;
        }

        if (inString) {
            if (c == stringChar && (i == 0 || line[i - 1] != '\\')) {
                inString = false;
            }
            result.append(' ');
            continue;
        }

        if (c == '/' && i + 1 < line.length() && line[i + 1] == '/') {
            break;
        }
        if (m_fileExtension == "py" || m_fileExtension == "yaml" || m_fileExtension == "yml") {
            if (c == '#') break;
        }

        result.append(c);
    }

    return result;
}

int BlueNoteEditor::blockIndentLevel(const QTextBlock &block) const {
    QString text = block.text();
    int indent = 0;
    for (QChar c : text) {
        if (c == ' ') indent++;
        else if (c == '\t') indent += 4;
        else break;
    }
    return indent;
}

int BlueNoteEditor::findDelimiterFoldEnd(int startBlockNumber, QChar openChar, QChar closeChar) const {
    int depth = 0;
    bool foundOpen = false;

    for (int i = startBlockNumber; i < blockCount(); ++i) {
        QTextBlock block = document()->findBlockByNumber(i);
        QString text = sanitizeLine(block.text());

        for (QChar c : text) {
            if (c == openChar) {
                depth++;
                foundOpen = true;
            } else if (c == closeChar) {
                if (foundOpen) {
                    depth--;
                    if (depth == 0) {
                        return i; // Retorna la línea de cierre exacta
                    }
                }
            }
        }
    }
    return -1;
}

int BlueNoteEditor::findCommentFoldEnd(int startBlockNumber) const {
    for (int i = startBlockNumber; i < blockCount(); ++i) {
        QTextBlock block = document()->findBlockByNumber(i);
        if (block.text().contains("*/")) {
            return i;
        }
    }
    return -1;
}

int BlueNoteEditor::findIndentFoldEnd(int startBlockNumber) const {
    QTextBlock startBlock = document()->findBlockByNumber(startBlockNumber);
    int baseIndent = blockIndentLevel(startBlock);
    int lastValidBlock = startBlockNumber;

    for (int i = startBlockNumber + 1; i < blockCount(); ++i) {
        QTextBlock block = document()->findBlockByNumber(i);
        QString text = block.text().trimmed();

        if (text.isEmpty()) continue;

        if (blockIndentLevel(block) <= baseIndent) {
            break;
        }

        lastValidBlock = i;
    }

    return (lastValidBlock > startBlockNumber) ? lastValidBlock : -1;
}

// DETECCION ESTRICTA: Solo es inicio de bloque si el cierre esta en una LINEA POSTERIOR
bool BlueNoteEditor::isFoldStart(const QTextBlock &block, QString &outType) const {
    int lineNum = block.blockNumber();
    QString text = sanitizeLine(block.text()).trimmed();
    if (text.isEmpty()) return false;

    // 1. Plegado por Llaves {}
    if (text.contains('{')) {
        int end = findDelimiterFoldEnd(lineNum, '{', '}');
        if (end > lineNum) {
            outType = "brace";
            return true;
        }
    }

    // 2. Plegado por Corchetes []
    if (text.contains('[')) {
        int end = findDelimiterFoldEnd(lineNum, '[', ']');
        if (end > lineNum) {
            outType = "bracket";
            return true;
        }
    }

    // 3. Plegado por Paréntesis () (Solo si abarcan varias líneas)
    if (text.contains('(')) {
        int end = findDelimiterFoldEnd(lineNum, '(', ')');
        if (end > lineNum) {
            outType = "paren";
            return true;
        }
    }

    // 4. Comentarios multilínea /* */
    if (text.contains("/*")) {
        int end = findCommentFoldEnd(lineNum);
        if (end > lineNum) {
            outType = "comment";
            return true;
        }
    }

    // 5. Lenguajes por indentación (Python, GDScript, YAML, etc.) o bloques con ':'
    bool isIndentLang = (m_fileExtension == "py" || m_fileExtension == "yaml" || 
                         m_fileExtension == "yml" || m_fileExtension == "gd");

    if (isIndentLang || text.endsWith(':')) {
        int end = findIndentFoldEnd(lineNum);
        if (end > lineNum) {
            outType = "indent";
            return true;
        }
    }

    outType.clear();
    return false;
}

int BlueNoteEditor::findFoldEndBlock(int startBlockNumber, const QString &type) const {
    if (type == "brace") return findDelimiterFoldEnd(startBlockNumber, '{', '}');
    if (type == "bracket") return findDelimiterFoldEnd(startBlockNumber, '[', ']');
    if (type == "paren") return findDelimiterFoldEnd(startBlockNumber, '(', ')');
    if (type == "comment") return findCommentFoldEnd(startBlockNumber);
    if (type == "indent") return findIndentFoldEnd(startBlockNumber);

    QTextBlock block = document()->findBlockByNumber(startBlockNumber);
    QString dummyType;
    if (isFoldStart(block, dummyType)) {
        return findFoldEndBlock(startBlockNumber, dummyType);
    }

    return -1;
}

QList<ScopeRegion> BlueNoteEditor::calculateVisibleScopes(int startVisibleLine, int endVisibleLine) const {
    QList<ScopeRegion> scopes;
    int searchStart = qMax(0, startVisibleLine - 300);

    for (int i = searchStart; i <= endVisibleLine; ++i) {
        QTextBlock block = document()->findBlockByNumber(i);
        if (!block.isValid()) break;

        QString type;
        if (isFoldStart(block, type) && !m_foldedBlocks.contains(i)) {
            int endBlock = findFoldEndBlock(i, type);
            if (endBlock > i && endBlock >= startVisibleLine) {
                int depth = 0;
                for (const auto &s : scopes) {
                    if (i > s.startLine && endBlock <= s.endLine) {
                        depth++;
                    }
                }
                scopes.append({i, endBlock, depth});
            }
        }
    }

    return scopes;
}

void BlueNoteEditor::paintLineNumbers(QPaintEvent *event) {
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), QColor(30, 30, 30));

    QTextBlock block = firstVisibleBlock();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    int currentLine = textCursor().block().blockNumber();
    int areaWidth = m_lineNumberArea->width();

    int stateBarWidth = 4;  
    int foldAreaWidth = 24; 
    int lineHeaderHeight = fontMetrics().height();

    int firstVisibleLine = block.blockNumber();
    int lastVisibleLine = document()->findBlock(event->rect().bottom()).blockNumber();

    QList<ScopeRegion> scopes = calculateVisibleScopes(firstVisibleLine, lastVisibleLine);

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            int lineNum = block.blockNumber();

            // 1. Estado de modificación de la línea
            if (m_lineStates.contains(lineNum)) {
                LineChangeState state = m_lineStates[lineNum];
                if (state == LineChangeState::ModifiedUnsaved) {
                    painter.fillRect(0, top, stateBarWidth, lineHeaderHeight, QColor(229, 192, 123));
                } else if (state == LineChangeState::ModifiedSaved) {
                    painter.fillRect(0, top, stateBarWidth, lineHeaderHeight, QColor(87, 171, 90));
                }
            }

            // 2. Número de línea
            QString number = QString::number(lineNum + 1);
            if (lineNum == currentLine) {
                painter.setPen(QColor(255, 255, 255));
                QFont font = painter.font();
                font.setBold(true);
                painter.setFont(font);
            } else {
                painter.setPen(QColor(120, 120, 120));
                QFont font = painter.font();
                font.setBold(false);
                painter.setFont(font);
            }

            int numberX = stateBarWidth + 4;
            int numberWidth = areaWidth - stateBarWidth - foldAreaWidth - 6;
            painter.drawText(numberX, top, numberWidth, lineHeaderHeight, Qt::AlignRight | Qt::AlignVCenter, number);

            // 3. Líneas de Ámbito Multilineales (Scope Lines)
            int foldBaseX = areaWidth - foldAreaWidth + 4;

            for (const auto &scope : scopes) {
                if (lineNum >= scope.startLine && lineNum <= scope.endLine) {
                    int lineX = foldBaseX + (scope.depthLevel * 5);
                    if (lineX >= areaWidth - 2) continue;

                    bool isActiveScope = (currentLine >= scope.startLine && currentLine <= scope.endLine);
                    QColor lineCol = isActiveScope ? QColor(75, 139, 245) : QColor(70, 70, 75);

                    painter.setPen(QPen(lineCol, 1, Qt::SolidLine));

                    if (lineNum == scope.startLine && !m_foldedBlocks.contains(lineNum)) {
                        painter.drawLine(lineX, top + lineHeaderHeight - 2, lineX, top + lineHeaderHeight);
                    } else if (lineNum == scope.endLine) {
                        int midY = top + lineHeaderHeight / 2;
                        painter.drawLine(lineX, top, lineX, midY);
                        painter.drawLine(lineX, midY, lineX + 4, midY);
                    } else if (lineNum > scope.startLine && lineNum < scope.endLine) {
                        painter.drawLine(lineX, top, lineX, top + lineHeaderHeight);
                    }
                }
            }

            // 4. Botón de Plegado ([+] / [-]) - Solo si abarca varias líneas
            QString foldType;
            bool isStart = isFoldStart(block, foldType);
            bool isFolded = m_foldedBlocks.contains(lineNum);

            if (isStart || isFolded) {
                int iconX = areaWidth - 16;
                int iconY = top + (lineHeaderHeight - 10) / 2;

                painter.fillRect(iconX - 1, iconY - 1, 12, 12, QColor(30, 30, 30));

                painter.setPen(QColor(180, 180, 180));
                painter.drawRect(iconX, iconY, 10, 10);
                
                painter.drawLine(iconX + 2, iconY + 5, iconX + 8, iconY + 5);

                if (isFolded) {
                    painter.drawLine(iconX + 5, iconY + 2, iconX + 5, iconY + 8);
                }
            }
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
    }
}

void BlueNoteEditor::toggleFold(int blockNumber) {
    if (m_foldedBlocks.contains(blockNumber)) {
        int endBlock = m_foldedBlocks.take(blockNumber);
        for (int i = blockNumber + 1; i <= endBlock; ++i) {
            QTextBlock b = document()->findBlockByNumber(i);
            if (b.isValid()) b.setVisible(true);
        }
    } else {
        QTextBlock block = document()->findBlockByNumber(blockNumber);
        QString type;
        if (isFoldStart(block, type)) {
            int endBlock = findFoldEndBlock(blockNumber, type);
            if (endBlock > blockNumber) {
                m_foldedBlocks[blockNumber] = endBlock;
                for (int i = blockNumber + 1; i <= endBlock; ++i) {
                    QTextBlock b = document()->findBlockByNumber(i);
                    if (b.isValid()) b.setVisible(false);
                }
            }
        }
    }

    document()->markContentsDirty(0, document()->characterCount());
    viewport()->update();
    m_lineNumberArea->update();
}

} // namespace Ui