#ifndef ENVIRONMENTMANAGER_H
#define ENVIRONMENTMANAGER_H

#include <QString>

class EnvironmentManager {
public:
    static void initEnvironment();
    static QString getBasePath();
    static QString getConfigPath();
    static QString getPluginsPath();
};

#endif // ENVIRONMENTMANAGER_H