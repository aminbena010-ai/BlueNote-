#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QCloseEvent>
#include "EditorTab.h"
#include "SyntaxHighlighter.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

    static constexpr int MaxRecentFiles = 5;

    EditorTab* currentTab() const;

public slots:
    void newFile();
    void openFile();
    void openFile(const QString &filePath);
    bool saveFile();
    bool saveFileAs();
    void saveAllFiles();
    void reloadFile();
    void closeCurrentTab();
    void closeTab(int index);
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void selectAll();
    void openRecentFile();

    // Diálogo de búsqueda y reemplazo
    void showFindReplaceDialog();

    // Detección automática de lenguaje por extensión
    void detectAndApplyLanguage(EditorTab *editorTab, const QString &filePath);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void createMenus();
    void setupStatusBar();                  // Inicializa la barra inferior
    void updateStatusBarMetrics();          // Actualiza palabras/caracteres en tiempo real
    void updateRecentFilesMenu();
    void addToRecentFiles(const QString &path);

    QTabWidget *m_tabWidget = nullptr;
    QMenu *m_fileMenu = nullptr;
    QMenu *m_editMenu = nullptr;
    QMenu *m_recentFilesMenu = nullptr;
    QAction *m_undoAction = nullptr;
    QAction *m_redoAction = nullptr;
    QAction *m_cutAction = nullptr;
    QAction *m_copyAction = nullptr;
    QAction *m_pasteAction = nullptr;
    QAction *m_selectAllAction = nullptr;
    QAction *m_recentFileActions[MaxRecentFiles] = {nullptr};

    // Widgets de la barra de estado inferior
    QLabel *m_wordCountLabel = nullptr;     
    QLabel *m_charCountLabel = nullptr;     
    QLabel *m_encodingLabel = nullptr;      
    QLabel *m_saveStatusLabel = nullptr;    // Indicador de guardado (*)

    // Diálogo flotante de búsqueda y reemplazo
    class FindReplaceDialog *findReplaceDialog = nullptr;
};

#endif // MAINWINDOW_H