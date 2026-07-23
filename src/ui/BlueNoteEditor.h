#pragma once

#include <QPlainTextEdit>
#include <QWidget>
#include <QMap>
#include <QList>
#include <QMouseEvent>
#include <QString>

namespace Ui {

class LineNumberArea;

enum class LineChangeState {
    Clean,
    ModifiedUnsaved,
    ModifiedSaved
};

struct ScopeRegion {
    int startLine;
    int endLine;
    int depthLevel;
};

class BlueNoteEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit BlueNoteEditor(QWidget *parent = nullptr);

    int lineNumberAreaWidth();
    void markAllLinesAsSaved();
    void setLanguageExtension(const QString &ext) { m_fileExtension = ext.toLower(); }
    
    // Método para aplicar los ajustes del motor de configuración .fgc
    void applyConfiguration();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
    void handleContentsChange(int position, int charsRemoved, int charsAdded);

private:
    friend class LineNumberArea;
    void paintLineNumbers(QPaintEvent *event);
    void handleMarginClick(int yPos);

    // --- MOTOR DE PLEGADO Y ÁMBITOS MULTILÍNEA ---
    bool isFoldStart(const QTextBlock &block, QString &outType) const;
    int findFoldEndBlock(int startBlockNumber, const QString &type) const;
    
    int findDelimiterFoldEnd(int startBlockNumber, QChar openChar, QChar closeChar) const;
    int findCommentFoldEnd(int startBlockNumber) const;
    int findIndentFoldEnd(int startBlockNumber) const;
    
    int blockIndentLevel(const QTextBlock &block) const;
    QString sanitizeLine(const QString &line) const;
    
    QList<ScopeRegion> calculateVisibleScopes(int startVisibleLine, int endVisibleLine) const;

    void toggleFold(int blockNumber);

    QWidget *m_lineNumberArea;
    QString m_fileExtension;

    QMap<int, LineChangeState> m_lineStates;
    QMap<int, int> m_foldedBlocks;
};

class LineNumberArea : public QWidget {
public:
    explicit LineNumberArea(BlueNoteEditor *editor) : QWidget(editor), m_editor(editor) {}

    QSize sizeHint() const override {
        return QSize(m_editor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        m_editor->paintLineNumbers(event);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            m_editor->handleMarginClick(event->pos().y());
        }
    }

private:
    BlueNoteEditor *m_editor;
};

} // namespace Ui