#pragma once

#include <QString>
#include <QStringList>
#include <QMap>

namespace Core {

struct PluginInfo {
    QString id;
    QString name;
    QString version;
    QString author;
    QString description;
    QString entryPoint;
    bool isEnabled;
};

class PluginManager {
public:
    static PluginManager& instance();

    void scanPlugins();
    QList<PluginInfo> getAvailablePlugins() const;
    
    // Modo Desarrollador: Instalar desde ZIP o Carpeta
    bool installPluginFromZip(const QString &zipPath);
    bool installPluginFromFolder(const QString &sourceFolder);

private:
    PluginManager() = default;
    ~PluginManager() = default;

    QMap<QString, PluginInfo> m_plugins;
};

} // namespace Core