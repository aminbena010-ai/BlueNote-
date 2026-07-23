#include "ConfigManager.h"
#include <QSettings>

namespace Core {

ConfigManager& ConfigManager::instance() {
    static ConfigManager inst;
    return inst;
}

ConfigManager::ConfigManager() {
    load();
}

void ConfigManager::load() {
    QSettings settings("BlueNotePlus", "BlueNote");

    m_tabSize = settings.value("editor/tabSize", 4).toInt();
    m_useSpacesForTabs = settings.value("editor/useSpaces", true).toBool();
    m_fontFamily = settings.value("editor/fontFamily", "Monospace").toString();
    m_fontSize = settings.value("editor/fontSize", 11).toInt();
    m_wordWrap = settings.value("editor/wordWrap", false).toBool();
}

void ConfigManager::save() {
    QSettings settings("BlueNotePlus", "BlueNote");

    settings.setValue("editor/tabSize", m_tabSize);
    settings.setValue("editor/useSpaces", m_useSpacesForTabs);
    settings.setValue("editor/fontFamily", m_fontFamily);
    settings.setValue("editor/fontSize", m_fontSize);
    settings.setValue("editor/wordWrap", m_wordWrap);
}

} // namespace Core