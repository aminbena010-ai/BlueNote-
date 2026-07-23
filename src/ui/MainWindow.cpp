#include "MainWindow.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QFileInfo>
#include <QStatusBar>
#include <QTextDocument>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setTabsClosable(true);
    setCentralWidget(m_tabWidget);
    
    setupStatusBar();

    connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(m_tabWidget, &QTabWidget::currentChanged, this, [this](int) {
        updateStatusBarMetrics();
    });

    resize(1200, 742);
    setWindowTitle("BlueNote+");
    createMenus();
    newFile();
}

EditorTab* MainWindow::currentTab() const {
    return qobject_cast<EditorTab*>(m_tabWidget->currentWidget());
}

void MainWindow::setupStatusBar() {
    m_saveStatusLabel = new QLabel("● Guardado", this);
    m_wordCountLabel = new QLabel("Palabras: 0", this);
    m_charCountLabel = new QLabel("Caracteres: 0", this);
    m_encodingLabel = new QLabel("UTF-8", this);

    QString labelStyle = "QLabel { color: #aaaaaa; padding: 0 10px; font-size: 11px; }";
    m_wordCountLabel->setStyleSheet(labelStyle);
    m_charCountLabel->setStyleSheet(labelStyle);
    m_encodingLabel->setStyleSheet(labelStyle);
    
    m_saveStatusLabel->setStyleSheet("QLabel { color: #4289da; padding: 0 10px; font-size: 11px; font-weight: bold; }");

    statusBar()->addWidget(m_saveStatusLabel);
    statusBar()->addPermanentWidget(m_wordCountLabel);
    statusBar()->addPermanentWidget(m_charCountLabel);
    statusBar()->addPermanentWidget(m_encodingLabel);

    statusBar()->setStyleSheet("QStatusBar { background-color: #222222; border-top: 1px solid #333333; }");
}

void MainWindow::updateStatusBarMetrics() {
    auto tab = currentTab();
    if (!tab || !tab->editor()) {
        m_wordCountLabel->setText("Palabras: 0");
        m_charCountLabel->setText("Caracteres: 0");
        m_saveStatusLabel->setText("● Guardado");
        return;
    }

    QString text = tab->editor()->toPlainText();
    int chars = text.length();
    int words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).count();

    m_wordCountLabel->setText(QString("Palabras: %1").arg(words));
    m_charCountLabel->setText(QString("Caracteres: %1").arg(chars));

    if (tab->editor()->document()->isModified()) {
        m_saveStatusLabel->setText("○ Sin guardar (*)");
        m_saveStatusLabel->setStyleSheet("QLabel { color: #e5c07b; padding: 0 10px; font-size: 11px; font-weight: bold; }");
    } else {
        m_saveStatusLabel->setText("● Guardado");
        m_saveStatusLabel->setStyleSheet("QLabel { color: #4289da; padding: 0 10px; font-size: 11px; font-weight: bold; }");
    }
}

void MainWindow::createMenus() {
    QMenuBar *bar = menuBar();
    m_fileMenu = bar->addMenu("&Archivo");
    
    QAction *newAct = m_fileMenu->addAction("&Nuevo");
    newAct->setShortcut(QKeySequence::New);
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    QAction *openAct = m_fileMenu->addAction("&Abrir...");
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, static_cast<void (MainWindow::*)()>(&MainWindow::openFile));

    QAction *saveAct = m_fileMenu->addAction("&Guardar");
    saveAct->setShortcut(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);

    QAction *saveAsAct = m_fileMenu->addAction("Guardar &como...");
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveFileAs);

    QAction *saveAllAct = m_fileMenu->addAction("Guardar &todo");
    connect(saveAllAct, &QAction::triggered, this, &MainWindow::saveAllFiles);

    m_fileMenu->addSeparator();

    QAction *reloadAct = m_fileMenu->addAction("&Recargar");
    connect(reloadAct, &QAction::triggered, this, &MainWindow::reloadFile);

    m_recentFilesMenu = m_fileMenu->addMenu("Archivos &Recientes");
    for (int i = 0; i < MaxRecentFiles; ++i) {
        m_recentFileActions[i] = new QAction(this);
        m_recentFileActions[i]->setVisible(false);
        connect(m_recentFileActions[i], &QAction::triggered, this, &MainWindow::openRecentFile);
        m_recentFilesMenu->addAction(m_recentFileActions[i]);
    }
    updateRecentFilesMenu();

    m_fileMenu->addSeparator();

    QAction *closeTabAct = m_fileMenu->addAction("&Cerrar pestaña");
    closeTabAct->setShortcut(QKeySequence::Close);
    connect(closeTabAct, &QAction::triggered, this, &MainWindow::closeCurrentTab);

    m_editMenu = bar->addMenu("&Editar");

    m_undoAction = m_editMenu->addAction("&Deshacer");
    m_undoAction->setShortcut(QKeySequence::Undo);
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::undo);

    m_redoAction = m_editMenu->addAction("&Rehacer");
    m_redoAction->setShortcut(QKeySequence::Redo);
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::redo);

    m_editMenu->addSeparator();

    m_cutAction = m_editMenu->addAction("Cor&tar");
    m_cutAction->setShortcut(QKeySequence::Cut);
    connect(m_cutAction, &QAction::triggered, this, &MainWindow::cut);

    m_copyAction = m_editMenu->addAction("&Copiar");
    m_copyAction->setShortcut(QKeySequence::Copy);
    connect(m_copyAction, &QAction::triggered, this, &MainWindow::copy);

    m_pasteAction = m_editMenu->addAction("&Pegar");
    m_pasteAction->setShortcut(QKeySequence::Paste);
    connect(m_pasteAction, &QAction::triggered, this, &MainWindow::paste);

    m_editMenu->addSeparator();

    m_selectAllAction = m_editMenu->addAction("Seleccionar &todo");
    m_selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(m_selectAllAction, &QAction::triggered, this, &MainWindow::selectAll);
}

void MainWindow::undo() {
    if (auto tab = currentTab()) {
        tab->editor()->undo();
    }
}

void MainWindow::redo() {
    if (auto tab = currentTab()) {
        tab->editor()->redo();
    }
}

void MainWindow::cut() {
    if (auto tab = currentTab()) {
        tab->editor()->cut();
    }
}

void MainWindow::copy() {
    if (auto tab = currentTab()) {
        tab->editor()->copy();
    }
}

void MainWindow::paste() {
    if (auto tab = currentTab()) {
        tab->editor()->paste();
    }
}

void MainWindow::selectAll() {
    if (auto tab = currentTab()) {
        tab->editor()->selectAll();
    }
}

void MainWindow::newFile() {
    EditorTab *tab = new EditorTab(this);
    
    connect(tab->editor(), &QPlainTextEdit::textChanged, this, [this]() {
        updateStatusBarMetrics();
    });
    
    connect(tab->editor()->document(), &QTextDocument::modificationChanged, this, [this](bool) {
        updateStatusBarMetrics();
        auto tab = currentTab();
        if (tab) {
            int idx = m_tabWidget->currentIndex();
            QString title = tab->fileName();
            if (tab->editor()->document()->isModified()) {
                title += " (*)";
            }
            m_tabWidget->setTabText(idx, title);
        }
    });

    int index = m_tabWidget->addTab(tab, "Sin título");
    m_tabWidget->setCurrentIndex(index);
    updateStatusBarMetrics();
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Abrir archivo");
    if (!fileName.isEmpty()) {
        openFile(fileName);
    }
}

void MainWindow::openFile(const QString &filePath) {
    if (filePath.isEmpty()) return;
    
    EditorTab *current = currentTab();
    if (current && current->filePath().isEmpty() && !current->isModified()) {
        if (current->loadFromFile(filePath)) {
            int idx = m_tabWidget->currentIndex();
            m_tabWidget->setTabText(idx, current->fileName());
            addToRecentFiles(filePath);
            updateStatusBarMetrics();
            return;
        }
    }

    EditorTab *tab = new EditorTab(this);
    
    connect(tab->editor(), &QPlainTextEdit::textChanged, this, [this]() {
        updateStatusBarMetrics();
    });
    
    connect(tab->editor()->document(), &QTextDocument::modificationChanged, this, [this](bool) {
        updateStatusBarMetrics();
        auto tab = currentTab();
        if (tab) {
            int idx = m_tabWidget->currentIndex();
            QString title = tab->fileName();
            if (tab->editor()->document()->isModified()) {
                title += " (*)";
            }
            m_tabWidget->setTabText(idx, title);
        }
    });

    if (tab->loadFromFile(filePath)) {
        int index = m_tabWidget->addTab(tab, tab->fileName());
        m_tabWidget->setCurrentIndex(index);
        addToRecentFiles(filePath);
        updateStatusBarMetrics();
    } else {
        delete tab;
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo.");
    }
}

bool MainWindow::saveFile() {
    EditorTab *tab = currentTab();
    if (!tab) return false;

    if (tab->filePath().isEmpty()) {
        return saveFileAs();
    }

    if (tab->save()) {
        m_tabWidget->setTabText(m_tabWidget->currentIndex(), tab->fileName());
        updateStatusBarMetrics();
        return true;
    }
    return false;
}

bool MainWindow::saveFileAs() {
    EditorTab *tab = currentTab();
    if (!tab) return false;

    QString fileName = QFileDialog::getSaveFileName(this, "Guardar archivo");
    if (fileName.isEmpty()) return false;

    if (tab->saveToFile(fileName)) {
        m_tabWidget->setTabText(m_tabWidget->currentIndex(), tab->fileName());
        addToRecentFiles(fileName);
        updateStatusBarMetrics();
        return true;
    }
    return false;
}

void MainWindow::saveAllFiles() {
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        if (auto tab = qobject_cast<EditorTab*>(m_tabWidget->widget(i))) {
            if (tab->filePath().isEmpty()) {
                m_tabWidget->setCurrentIndex(i);
                saveFileAs();
            } else {
                tab->save();
                m_tabWidget->setTabText(i, tab->fileName());
            }
        }
    }
    updateStatusBarMetrics();
}

void MainWindow::reloadFile() {
    if (auto tab = currentTab()) {
        if (!tab->filePath().isEmpty()) {
            tab->loadFromFile(tab->filePath());
            updateStatusBarMetrics();
        }
    }
}

void MainWindow::closeCurrentTab() {
    closeTab(m_tabWidget->currentIndex());
}

void MainWindow::closeTab(int index) {
    if (index < 0 || index >= m_tabWidget->count()) return;

    QWidget *widget = m_tabWidget->widget(index);
    if (widget) {
        m_tabWidget->removeTab(index);
        widget->deleteLater();
        updateStatusBarMetrics();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
}

void MainWindow::addToRecentFiles(const QString &path) {
    QSettings settings("BlueNotePlus", "BlueNotePlus");
    QStringList files = settings.value("recentFileList").toStringList();

    files.removeAll(path);
    files.prepend(path);
    while (files.size() > MaxRecentFiles) {
        files.removeLast();
    }

    settings.setValue("recentFileList", files);
    updateRecentFilesMenu();
}

void MainWindow::updateRecentFilesMenu() {
    QSettings settings("BlueNotePlus", "BlueNotePlus");
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), static_cast<int>(MaxRecentFiles));

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = QString("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
        m_recentFileActions[i]->setText(text);
        m_recentFileActions[i]->setData(files[i]);
        m_recentFileActions[i]->setVisible(true);
    }

    for (int j = numRecentFiles; j < MaxRecentFiles; ++j) {
        m_recentFileActions[j]->setVisible(false);
    }
}

void MainWindow::openRecentFile() {
    if (auto action = qobject_cast<QAction*>(sender())) {
        openFile(action->data().toString());
    }
}