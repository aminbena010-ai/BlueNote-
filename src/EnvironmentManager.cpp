#include "EnvironmentManager.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDebug>

QString EnvironmentManager::getBasePath() {
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.bluenote";
}

QString EnvironmentManager::getConfigPath() {
    return getBasePath() + "/config.fgc";
}

QString EnvironmentManager::getPluginsPath() {
    return getBasePath() + "/.plugins";
}

void EnvironmentManager::initEnvironment() {
    QString base = getBasePath();
    QString configDir = base + "/.config";
    QString pluginsDir = getPluginsPath();
    QString configFile = getConfigPath();

    QDir dir;

    // 1. Crear directorios principales y submódulos si no existen
    if (!dir.exists(base)) {
        dir.mkpath(base);
        qDebug() << "[BlueNote] Directorio base creado en:" << base;
    }
    
    // Forzar la creación de la carpeta .config/ explícitamente
    if (!dir.exists(configDir)) {
        if (dir.mkpath(configDir)) {
            qDebug() << "[BlueNote] Directorio de configuración avanzada creado en:" << configDir;
        }
    }

    if (!dir.exists(pluginsDir)) {
        dir.mkpath(pluginsDir);
        qDebug() << "[BlueNote] Directorio de plugins creado en:" << pluginsDir;
    }

    // 2. Crear el archivo config.fgc maestro por defecto si no existe
    QFile file(configFile);
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "# ==========================================\n";
            out << "# BlueNote+ - Archivo Maestro de Configuracion\n";
            out << "# ==========================================\n\n";
            out << "[Modules]\n";
            out << "load_editor = true\n";
            out << "load_themes = true\n\n";
            out << "[Editor]\n";
            out << "tab_size = 4\n";
            out << "use_spaces = true\n";
            out << "font_family = \"Monospace\"\n";
            out << "font_size = 11\n";
            out << "word_wrap = false\n";
            file.close();
            qDebug() << "[BlueNote] Archivo maestro 'config.fgc' predeterminado generado con exito.";
        }
    }
}