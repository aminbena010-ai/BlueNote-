#include "FindReplaceDialog.h"
#include <QTextDocument>
#include <QMessageBox>

FindReplaceDialog::FindReplaceDialog(QPlainTextEdit *editor, QWidget *parent)
    : QDialog(parent), m_editor(editor)
{
    setupUI();
}

void FindReplaceDialog::setupUI() {
    setWindowTitle("Buscar y Reemplazar");
    resize(400, 150);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Layout para Buscar
    QHBoxLayout *findLayout = new QHBoxLayout();
    QLabel *findLabel = new QLabel("Buscar:", this);
    m_findInput = new QLineEdit(this);
    findLayout->addWidget(findLabel);
    findLayout->addWidget(m_findInput);
    mainLayout->addLayout(findLayout);

    // Layout para Reemplazar
    QHBoxLayout *replaceLayout = new QHBoxLayout();
    QLabel *replaceLabel = new QLabel("Reemplazar:", this);
    m_replaceInput = new QLineEdit(this);
    replaceLayout->addWidget(replaceLabel);
    replaceLayout->addWidget(m_replaceInput);
    mainLayout->addLayout(replaceLayout);

    // Opciones y Botones
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_findButton = new QPushButton("Buscar Siguiente", this);
    m_replaceButton = new QPushButton("Reemplazar", this);
    m_replaceAllButton = new QPushButton("Reemplazar Todo", this);
    
    buttonLayout->addWidget(m_findButton);
    buttonLayout->addWidget(m_replaceButton);
    buttonLayout->addWidget(m_replaceAllButton);
    mainLayout->addLayout(buttonLayout);

    // Conexiones básicas
    connect(m_findButton, &QPushButton::clicked, this, &FindReplaceDialog::findNext);
    connect(m_replaceButton, &QPushButton::clicked, this, &FindReplaceDialog::replaceCurrent);
    connect(m_replaceAllButton, &QPushButton::clicked, this, &FindReplaceDialog::replaceAll);
}

void FindReplaceDialog::findNext() {
    if (!m_editor) return;
    QString searchText = m_findInput->text();
    if (searchText.isEmpty()) return;

    bool found = m_editor->find(searchText);
    if (!found) {
        // Volver a buscar desde el inicio si llega al final
        m_editor->moveCursor(QTextCursor::Start);
        if (!m_editor->find(searchText)) {
            QMessageBox::information(this, "Buscar", "No se encontró el texto especificado.");
        }
    }
}

void FindReplaceDialog::replaceCurrent() {
    if (!m_editor) return;
    if (m_editor->textCursor().hasSelection()) {
        m_editor->textCursor().insertText(m_replaceInput->text());
    }
    findNext();
}

void FindReplaceDialog::replaceAll() {
    if (!m_editor) return;
    QString searchText = m_findInput->text();
    QString replaceText = m_replaceInput->text();
    if (searchText.isEmpty()) return;

    QTextCursor cursor = m_editor->textCursor();
    cursor.beginEditBlock();
    
    m_editor->moveCursor(QTextCursor::Start);
    int count = 0;
    while (m_editor->find(searchText)) {
        m_editor->textCursor().insertText(replaceText);
        count++;
    }
    
    cursor.endEditBlock();
    QMessageBox::information(this, "Reemplazar Todo", QString("Se reemplazaron %1 coincidencias.").arg(count));
}