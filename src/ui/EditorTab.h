#pragma once

#include <QWidget>
#include <QString>
#include "BlueNoteEditor.h"
#include "SyntaxHighlighter.h"

class EditorTab : public QWidget {
    Q_OBJECT

public:
    explicit EditorTab(QWidget *parent = nullptr);

    bool loadFromFile(const QString &path);
    bool saveToFile(const QString &path);
    bool save();

    QString filePath() const { return m_filePath; }
    QString fileName() const;
    bool isModified() const;

    Ui::BlueNoteEditor* editor() const { return m_editor; }
    
    // Método getter para acceder al resaltador de sintaxis desde MainWindow
    SyntaxHighlighter* getHighlighter() const { return m_highlighter; }

signals:
    void modificationChanged(bool modified);

private:
    void updateHighlighter();

    Ui::BlueNoteEditor *m_editor;
    SyntaxHighlighter *m_highlighter; // Corregido: Sin el prefijo Ui::
    QString m_filePath;
};