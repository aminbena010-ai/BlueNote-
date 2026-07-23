#include "ui/MainWindow.h"
#include "core/ConfigManager.h"

#include <QApplication>
#include <QGuiApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QIcon>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 1. Metadatos de la aplicación
    QCoreApplication::setOrganizationName("BlueNotePlus");
    QCoreApplication::setOrganizationDomain("bluenoteplus.org");
    QCoreApplication::setApplicationName("BlueNote+");
    QCoreApplication::setApplicationVersion("0.1.0");

    // Vincula el proceso de Linux con el archivo .desktop para reemplazar el engranaje genérico
    QGuiApplication::setDesktopFileName("bluenoteplus");

    // 2. Comprobación y asignación del Icono / Logo
    if (!QFile::exists(":/logo.png")) {
        qWarning() << "[ERROR] Qt no encuentra ':/logo.png'. Revisa la ruta en resources.qrc";
    } else {
        qInfo() << "[OK] Logo cargado correctamente desde el sistema de recursos.";
    }

    QIcon appIcon(":/logo.png");
    app.setWindowIcon(appIcon);

    // 3. Estilo 'Fusion' con Paleta Oscura
    app.setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(30, 30, 30));
    darkPalette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, QColor(220, 220, 220));
    darkPalette.setColor(QPalette::Button, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    
    app.setPalette(darkPalette);

    // 4. Cargar configuración global
    Core::ConfigManager::instance().load();

    // 5. Instancia principal
    MainWindow window;
    window.setWindowIcon(appIcon);
    window.show();

    // 6. Comprobar si se ha pasado un archivo como argumento (ej. opción "Abrir con" del sistema)
    if (argc > 1) {
        QString filePath = QString::fromUtf8(argv[1]);
        if (!filePath.isEmpty() && QFile::exists(filePath)) {
            // Asegúrate de que MainWindow disponga de un método para abrir archivos por ruta (ej. openFile o loadFile)
            window.openFile(filePath);
        }
    }

    return app.exec();
}