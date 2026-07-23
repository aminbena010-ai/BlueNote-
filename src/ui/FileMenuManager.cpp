#include "FileMenuManager.h"
#include "BlueNoteEditor.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QKeySequence>
#include <QCoreApplication>

FileMenuManager::FileMenuManager(Ui::BlueNoteEditor *editor, QWidget *parentWindow)
    : QObject(parentWindow), m_editor(editor), m_parentWindow(parentWindow)
{
    m_recentFilesMenu = new QMenu(tr("Archivos Recientes"), parentWindow);
    for (int i = 0; i < MaxRecentFiles; ++i) {
        m_recentFileActions[i] = new QAction(this);
        m_recentFileActions[i]->setVisible(false);
        connect(m_recentFileActions[i], &QAction::triggered, this, &FileMenuManager::openRecentFile);
        m_recentFilesMenu->addAction(m_recentFileActions[i]);
    }
}

QMenu* FileMenuManager::createFileMenu() {
    QMenu *fileMenu = new QMenu(tr("&Archivo"), m_parentWindow);

    // 1. Nuevo (Ctrl+N)
    QAction *actionNew = fileMenu->addAction(tr("&Nuevo"), this, &FileMenuManager::newFile, QKeySequence::New);
    actionNew->setIcon(QIcon::fromTheme("document-new"));

    // 2. Abrir (Ctrl+O)
    QAction *actionOpen = fileMenu->addAction(tr("&Abrir..."), [this]() { openFile(); }, QKeySequence::Open);
    actionOpen->setIcon(QIcon::fromTheme("document-open"));

    // 3. Submenú Archivos Recientes
    fileMenu->addMenu(m_recentFilesMenu);
    updateRecentFilesMenu();

    fileMenu->addSeparator();

    // 4. Guardar (Ctrl+S)
    QAction *actionSave = fileMenu->addAction(tr("&Guardar"), this, &FileMenuManager::saveFile, QKeySequence::Save);
    actionSave->setIcon(QIcon::fromTheme("document-save"));

    // 5. Guardar Como... (Ctrl+Shift+S)
    QAction *actionSaveAs = fileMenu->addAction(tr("Guardar &Como..."), this, &FileMenuManager::saveFileAs, QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    actionSaveAs->setIcon(QIcon::fromTheme("document-save-as"));

    // 6. Recargar desde Disco (F5)
    QAction *actionReload = fileMenu->addAction(tr("&Recargar desde Disco"), this, &FileMenuManager::reloadFile, QKeySequence(Qt::Key_F5));
    actionReload->setIcon(QIcon::fromTheme("view-refresh"));

    fileMenu->addSeparator();

    // 7. Cerrar (Ctrl+W)
    QAction *actionClose = fileMenu->addAction(tr("&Cerrar Documento"), [this]() { newFile(); }, QKeySequence::Close);

    // 8. Salir (Ctrl+Q)
    QAction *actionExit = fileMenu->addAction(tr("&Salir"), QCoreApplication::instance(), &QCoreApplication::quit, QKeySequence::Quit);

    return fileMenu;
}

bool FileMenuManager::newFile() {
    if (isModified()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(
            m_parentWindow, tr("Guardar Cambios"),
            tr("El documento tiene cambios no guardados. ¿Deseas guardarlos antes de continuar?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
        );

        if (ret == QMessageBox::Save) {
            if (!saveFile()) return false;
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }

    m_editor->clear();
    setCurrentFilePath(QString());
    return true;
}

bool FileMenuManager::openFile(const QString &filePath) {
    QString path = filePath;

    if (path.isEmpty()) {
        path = QFileDialog::getOpenFileName(
            m_parentWindow,
            tr("Abrir Archivo"),
            QString(),
            tr("Todos los Archivos (*);;Archivos C++ (*.cpp *.h);;Python (*.py);;JSON (*.json)")
        );
    }

    if (path.isEmpty()) return false;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(m_parentWindow, tr("Error"), tr("No se pudo abrir el archivo:\n%1").arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    m_editor->setPlainText(in.readAll());
    file.close();

    setCurrentFilePath(path);
    addToRecentFiles(path);

    // Sincroniza estado guardado y extensión con el editor
    m_editor->markAllLinesAsSaved();

    return true;
}

bool FileMenuManager::saveFile() {
    if (m_currentFilePath.isEmpty()) {
        return saveFileAs();
    }

    QFile file(m_currentFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(m_parentWindow, tr("Error"), tr("No se pudo guardar el archivo:\n%1").arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out << m_editor->toPlainText();
    file.close();

    // Actualiza indicador visual de guardado en el editor
    m_editor->markAllLinesAsSaved();
    emit fileSaved();

    return true;
}

bool FileMenuManager::saveFileAs() {
    QString path = QFileDialog::getSaveFileName(
        m_parentWindow,
        tr("Guardar Como..."),
        m_currentFilePath.isEmpty() ? QString("documento.txt") : m_currentFilePath,
        tr("Todos los Archivos (*);;Archivos C++ (*.cpp *.h);;Python (*.py);;JSON (*.json)")
    );

    if (path.isEmpty()) return false;

    setCurrentFilePath(path);
    addToRecentFiles(path);
    return saveFile();
}

bool FileMenuManager::reloadFile() {
    if (m_currentFilePath.isEmpty()) return false;

    if (isModified()) {
        QMessageBox::StandardButton ret = QMessageBox::question(
            m_parentWindow, tr("Confirmar Recarga"),
            tr("Perderás todos los cambios no guardados. ¿Seguro que quieres recargar?"),
            QMessageBox::Yes | QMessageBox::No
        );
        if (ret == QMessageBox::No) return false;
    }

    return openFile(m_currentFilePath);
}

bool FileMenuManager::isModified() const {
    return m_editor->document()->isModified();
}

void FileMenuManager::setCurrentFilePath(const QString &path) {
    m_currentFilePath = path;
    m_editor->document()->setModified(false);

    if (!path.isEmpty()) {
        QFileInfo info(path);
        m_editor->setLanguageExtension(info.suffix());
    } else {
        m_editor->setLanguageExtension(QString());
    }

    emit filePathChanged(m_currentFilePath);
}

void FileMenuManager::addToRecentFiles(const QString &path) {
    QSettings settings("BlueNote", "Editor");
    QStringList files = settings.value("recentFiles").toStringList();

    files.removeAll(path);
    files.prepend(path);

    while (files.size() > MaxRecentFiles) {
        files.removeLast();
    }

    settings.setValue("recentFiles", files);
    updateRecentFilesMenu();
}

void FileMenuManager::updateRecentFilesMenu() {
    QSettings settings("BlueNote", "Editor");
    QStringList files = settings.value("recentFiles").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = QString("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
        m_recentFileActions[i]->setText(text);
        m_recentFileActions[i]->setData(files[i]);
        m_recentFileActions[i]->setVisible(true);
    }

    for (int j = numRecentFiles; j < MaxRecentFiles; ++j) {
        m_recentFileActions[j]->setVisible(false);
    }

    m_recentFilesMenu->setEnabled(numRecentFiles > 0);
}

void FileMenuManager::openRecentFile() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        openFile(action->data().toString());
    }
}