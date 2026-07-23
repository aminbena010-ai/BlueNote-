#include "ConfigManager.h"
#include "../EnvironmentManager.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

namespace Core {

ConfigManager& ConfigManager::instance() {
    static ConfigManager inst;
    return inst;
}

ConfigManager::ConfigManager() {
}

void ConfigManager::load() {
    QString masterPath = EnvironmentManager::getConfigPath();
    parseFile(masterPath);

    QString configDirPath = EnvironmentManager::getBasePath() + "/.config";
    QDir dir(configDirPath);

    if (dir.exists()) {
        QStringList filters;
        filters << "*.fgc";
        QFileInfoList list = dir.entryInfoList(filters, QDir::Files);

        for (const QFileInfo &fileInfo : list) {
            qDebug() << "[ConfigManager] Leyendo submódulo:" << fileInfo.fileName();
            parseFile(fileInfo.absoluteFilePath());
        }
    }
}

void ConfigManager::save() {
    QString masterPath = EnvironmentManager::getConfigPath();
    QFile file(masterPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "# ==========================================\n";
        out << "# BlueNote+ - Archivo Maestro de Configuracion\n";
        out << "# ==========================================\n\n";
        out << "[Modules]\n";
        out << "load_editor = true\n";
        out << "load_themes = true\n\n";
        out << "[Editor]\n";
        out << "tab_size = " << m_tabSize << "\n";
        out << "use_spaces = " << (m_useSpacesForTabs ? "true" : "false") << "\n";
        out << "font_family = \"" << m_fontFamily << "\"\n";
        out << "font_size = " << m_fontSize << "\n";
        out << "word_wrap = " << (m_wordWrap ? "true" : "false") << "\n";
        out << "light_theme = " << (m_isLightTheme ? "true" : "false") << "\n";
        file.close();
    }
}

void ConfigManager::parseFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString currentSection = "";

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) continue;

        if (line.startsWith('[') && line.endsWith(']')) {
            currentSection = line.mid(1, line.length() - 2).trimmed();
            continue;
        }

        int eqIndex = line.indexOf('=');
        if (eqIndex != -1) {
            QString key = line.left(eqIndex).trimmed();
            QString value = line.mid(eqIndex + 1).trimmed();

            if (value.startsWith('"') && value.endsWith('"')) {
                value = value.mid(1, value.length() - 2);
            }

            if (currentSection == "Editor") {
                if (key == "font_size") m_fontSize = value.toInt();
                else if (key == "tab_size") m_tabSize = value.toInt();
                else if (key == "use_spaces") m_useSpacesForTabs = (value.toLower() == "true");
                else if (key == "font_family") m_fontFamily = value;
                else if (key == "word_wrap") m_wordWrap = (value.toLower() == "true");
                else if (key == "light_theme") m_isLightTheme = (value.toLower() == "true");
            }
        }
    }
    file.close();
}

} // namespace Core