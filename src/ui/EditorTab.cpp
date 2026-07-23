#include "EditorTab.h"
#include <QVBoxLayout>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QFontDatabase>

EditorTab::EditorTab(QWidget *parent) : QWidget(parent) {
    m_editor = new Ui::BlueNoteEditor(this);

    // Crear y asociar el resaltador de sintaxis (corregido sin prefijo Ui::)
    m_highlighter = new SyntaxHighlighter(m_editor->document());

    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(11);
    m_editor->setFont(font);

    m_editor->setTabStopDistance(m_editor->fontMetrics().horizontalAdvance(' ') * 4);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_editor);

    connect(m_editor->document(), &QTextDocument::modificationChanged,
            this, &EditorTab::modificationChanged);
}

void EditorTab::updateHighlighter() {
    QString ext = QFileInfo(m_filePath).suffix().toLower();
    
    // Configuración universal de resaltado según extensión ampliada
    if (ext == "cpp" || ext == "cc" || ext == "cxx" || ext == "h" || ext == "hpp" || ext == "c") {
        m_highlighter->setLanguage(SyntaxHighlighter::Language::Cpp);
    } else if (ext == "py" || ext == "pyw") {
        m_highlighter->setLanguage(SyntaxHighlighter::Language::Python);
    } else if (ext == "js" || ext == "ts" || ext == "jsx" || ext == "tsx" || ext == "json") {
        m_highlighter->setLanguage(SyntaxHighlighter::Language::JavaScript);
    } else if (ext == "rs") {
        m_highlighter->setLanguage(SyntaxHighlighter::Language::Rust);
    } else if (ext == "go") {
        m_highlighter->setLanguage(SyntaxHighlighter::Language::Go);
    } else if (ext == "md" || ext == "markdown") {
        m_highlighter->setLanguage(SyntaxHighlighter::Language::Markdown);
    } else {
        m_highlighter->setLanguage(SyntaxHighlighter::Language::Plain);
    }
    
    // Configuración universal de plegado de código del editor
    m_editor->setLanguageExtension(ext); 
}

bool EditorTab::loadFromFile(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    m_editor->setPlainText(in.readAll());
    file.close();

    m_filePath = path;
    m_editor->document()->setModified(false);
    
    updateHighlighter(); // Actualizar colores según extensión
    return true;
}

bool EditorTab::saveToFile(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << m_editor->toPlainText();
    file.close();

    m_filePath = path;
    m_editor->document()->setModified(false);
    
    // Convierte todas las líneas editadas (Amarillas) a guardadas (Verdes)
    m_editor->markAllLinesAsSaved();
    
    updateHighlighter();
    return true;
}

bool EditorTab::save() {
    if (m_filePath.isEmpty()) return false;
    return saveToFile(m_filePath);
}

QString EditorTab::fileName() const {
    if (m_filePath.isEmpty()) return "Sin título";
    return QFileInfo(m_filePath).fileName();
}

bool EditorTab::isModified() const {
    return m_editor->document()->isModified();
}