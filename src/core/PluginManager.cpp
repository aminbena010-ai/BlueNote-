#include "PluginManager.h"
#include "../EnvironmentManager.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QProcess>

namespace Core {

PluginManager& PluginManager::instance() {
    static PluginManager inst;
    return inst;
}

void PluginManager::scanPlugins() {
    m_plugins.clear();
    QString pluginsDir = EnvironmentManager::getBasePath() + "/.plugins";
    QDir dir(pluginsDir);

    if (!dir.exists()) {
        dir.mkpath(pluginsDir);
        return;
    }

    QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &sub : subdirs) {
        QString manifestPath = dir.absoluteFilePath(sub + "/plugin.json");
        QFile file(manifestPath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            if (!doc.isNull() && doc.isObject()) {
                QJsonObject obj = doc.object();
                PluginInfo info;
                info.id = obj["id"].toString(sub);
                info.name = obj["name"].toString(sub);
                info.version = obj["version"].toString("1.0");
                info.author = obj["author"].toString("Desconocido");
                info.description = obj["description"].toString("");
                info.isEnabled = true;

                m_plugins[info.id] = info;
                qDebug() << "[PluginManager] Plugin cargado:" << info.name;
            }
            file.close();
        }
    }
}

QList<PluginInfo> PluginManager::getAvailablePlugins() const {
    return m_plugins.values();
}

bool PluginManager::installPluginFromFolder(const QString &sourceFolder) {
    QDir src(sourceFolder);
    if (!src.exists()) return false;

    QString pluginsDir = EnvironmentManager::getBasePath() + "/.plugins";
    QString pluginName = src.dirName();
    QString targetPath = pluginsDir + "/" + pluginName;

    QDir target(targetPath);
    if (target.exists()) {
        target.removeRecursively();
    }

    // Copiar recursivamente
    QDir().mkpath(targetPath);
    // Nota: Podés usar un helper de copia o QDirIterator para copiar archivos de carpeta a carpeta.
    qDebug() << "[PluginManager] Instalando plugin desde carpeta a:" << targetPath;
    return true;
}

bool PluginManager::installPluginFromZip(const QString &zipPath) {
    // Si usas herramientas de sistema en Linux (como 'unzip')
    QString pluginsDir = EnvironmentManager::getBasePath() + "/.plugins";
    QString cmd = QString("unzip -o \"%1\" -d \"%2\"").arg(zipPath, pluginsDir);
    int exitCode = QProcess::execute(cmd);
    return (exitCode == 0);
}

} // namespace Core