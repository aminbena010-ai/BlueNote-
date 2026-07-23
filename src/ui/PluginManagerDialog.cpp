#include "PluginManagerDialog.h"
#include "../core/PluginManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QProcess>

namespace Ui {

PluginManagerDialog::PluginManagerDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Catálogo y Gestión de Plugins - BlueNote+");
    resize(700, 450);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_tabWidget = new QTabWidget(this);

    // --- PESTAÑA 1: Plugins Instalados & Modo Desarrollador ---
    QWidget *installedTab = new QWidget(this);
    QVBoxLayout *installedLayout = new QVBoxLayout(installedTab);

    installedLayout->addWidget(new QLabel("Plugins instalados en tu sistema:", this));
    m_installedListWidget = new QListWidget(this);
    installedLayout->addWidget(m_installedListWidget);

    // Sección Modo Desarrollador
    QHBoxLayout *devLayout = new QHBoxLayout();
    m_btnLoadZip = new QPushButton("Cargar Plugin (.zip)", this);
    m_btnLoadFolder = new QPushButton("Cargar Plugin (Carpeta)", this);
    devLayout->addWidget(m_btnLoadZip);
    devLayout->addWidget(m_btnLoadFolder);

    installedLayout->addLayout(devLayout);
    m_tabWidget->addTab(installedTab, "Mis Plugins");

    // --- PESTAÑA 2: Catálogo Oficial ---
    QWidget *storeTab = new QWidget(this);
    QVBoxLayout *storeLayout = new QVBoxLayout(storeTab);
    storeLayout->addWidget(new QLabel("Catálogo oficial de extensiones comunitarias:", this));
    m_storeListWidget = new QListWidget(this);
    m_storeListWidget->addItem("🔌 Git Integration Pro (Oficial) - Sincroniza tus repositorios al vuelo.");
    m_storeListWidget->addItem("🎨 Material Dark Themes (Oficial) - Nuevas paletas visuales para el editor.");
    storeLayout->addWidget(m_storeListWidget);
    m_tabWidget->addTab(storeTab, "Catálogo Online");

    mainLayout->addWidget(m_tabWidget);

    // Conexiones
    connect(m_btnLoadZip, &QPushButton::clicked, this, &PluginManagerDialog::onInstallZipClicked);
    connect(m_btnLoadFolder, &QPushButton::clicked, this, &PluginManagerDialog::onInstallFolderClicked);

    loadPluginsList();
}

void PluginManagerDialog::loadPluginsList() {
    m_installedListWidget->clear();
    Core::PluginManager::instance().scanPlugins();
    auto plugins = Core::PluginManager::instance().getAvailablePlugins();

    if (plugins.isEmpty()) {
        m_installedListWidget->addItem("No hay plugins de terceros instalados todavía. Usa el Modo Desarrollador abajo.");
    } else {
        for (const auto &p : plugins) {
            m_installedListWidget->addItem(QString("🧩 %1 (v%2) - %3").arg(p.name, p.version, p.description));
        }
    }
}

void PluginManagerDialog::onInstallZipClicked() {
    QString zipPath = QFileDialog::getOpenFileName(this, "Seleccionar Paquete de Plugin", "", "Archivos ZIP (*.zip)");
    if (!zipPath.isEmpty()) {
        if (Core::PluginManager::instance().installPluginFromZip(zipPath)) {
            QMessageBox::information(this, "Plugin Instalado", "¡El plugin se ha instalado con éxito!\nLa aplicación se reiniciará ahora para aplicarlo.");
            
            // Cierre y reinicio automático de la aplicación
            qApp->quit();
            QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
        } else {
            QMessageBox::warning(this, "Error", "No se pudo descomprimir o instalar el archivo ZIP.");
        }
    }
}

void PluginManagerDialog::onInstallFolderClicked() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "Seleccionar Carpeta del Plugin");
    if (!folderPath.isEmpty()) {
        if (Core::PluginManager::instance().installPluginFromFolder(folderPath)) {
            QMessageBox::information(this, "Plugin Instalado", "¡Carpeta de plugin integrada con éxito!\nLa aplicación se reiniciará ahora.");
            
            qApp->quit();
            QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
        }
    }
}

} // namespace Ui