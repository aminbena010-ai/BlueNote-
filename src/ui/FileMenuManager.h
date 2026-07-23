#pragma once

#include <QObject>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QFileInfo>
#include <QSettings>
#include <QList>

namespace Ui {
    class BlueNoteEditor;
}

class FileMenuManager : public QObject {
    Q_OBJECT

public:
    explicit FileMenuManager(Ui::BlueNoteEditor *editor, QWidget *parentWindow = nullptr);

    QMenu* createFileMenu();
    
    // Métodos principales de archivo
    bool newFile();
    bool openFile(const QString &filePath = QString());
    bool saveFile();
    bool saveFileAs();
    bool reloadFile();

    QString currentFilePath() const { return m_currentFilePath; }
    bool isModified() const;

signals:
    void filePathChanged(const QString &newPath);
    void fileSaved();

private slots:
    void openRecentFile();
    void updateRecentFilesMenu();

private:
    void setCurrentFilePath(const QString &path);
    void addToRecentFiles(const QString &path);

    Ui::BlueNoteEditor *m_editor;
    QWidget *m_parentWindow;
    QString m_currentFilePath;

    QMenu *m_recentFilesMenu;
    static const int MaxRecentFiles = 8;
    QAction *m_recentFileActions[MaxRecentFiles];
};