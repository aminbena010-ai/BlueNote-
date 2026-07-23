#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include "EditorTab.h"

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

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void createMenus();
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
};

#endif // MAINWINDOW_H