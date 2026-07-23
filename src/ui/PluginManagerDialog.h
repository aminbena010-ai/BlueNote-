#pragma once

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QTabWidget>

namespace Ui {

class PluginManagerDialog : public QDialog {
    Q_OBJECT
public:
    explicit PluginManagerDialog(QWidget *parent = nullptr);

private slots:
    void onInstallZipClicked();
    void onInstallFolderClicked();

private:
    void loadPluginsList();

    QTabWidget *m_tabWidget;
    QListWidget *m_installedListWidget;
    QListWidget *m_storeListWidget;
    QPushButton *m_btnLoadZip;
    QPushButton *m_btnLoadFolder;
};

} // namespace Ui