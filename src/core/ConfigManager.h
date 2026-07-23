#pragma once

#include <QString>

namespace Core {

class ConfigManager {
public:
    static ConfigManager& instance();

    void load();
    void save();

    // Getters y Setters
    int tabSize() const { return m_tabSize; }
    void setTabSize(int size) { m_tabSize = size; }

    bool useSpacesForTabs() const { return m_useSpacesForTabs; }
    void setUseSpacesForTabs(bool value) { m_useSpacesForTabs = value; }

    QString fontFamily() const { return m_fontFamily; }
    void setFontFamily(const QString &family) { m_fontFamily = family; }

    int fontSize() const { return m_fontSize; }
    void setFontSize(int size) { m_fontSize = size; }

    bool wordWrap() const { return m_wordWrap; }
    void setWordWrap(bool wrap) { m_wordWrap = wrap; }

private:
    ConfigManager();
    ~ConfigManager() = default;

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    int m_tabSize = 4;
    bool m_useSpacesForTabs = true;
    QString m_fontFamily = "Monospace";
    int m_fontSize = 11;
    bool m_wordWrap = false;
};

} // namespace Core