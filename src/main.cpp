#include "ui/MainWindow.h"
#include "EnvironmentManager.h"
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

    QCoreApplication::setOrganizationName("BlueNotePlus");
    QCoreApplication::setOrganizationDomain("bluenoteplus.org");
    QCoreApplication::setApplicationName("BlueNote+");
    QCoreApplication::setApplicationVersion("0.2.4");

    QGuiApplication::setDesktopFileName("bluenoteplus");

    EnvironmentManager::initEnvironment();

    Core::ConfigManager::instance().load();
    bool lightTheme = Core::ConfigManager::instance().isLightTheme();

    if (!QFile::exists(":/logo.png")) {
        qWarning() << "[ERROR] Qt no encuentra ':/logo.png'. Revisa la ruta en resources.qrc";
    } else {
        qInfo() << "[OK] Logo cargado correctamente desde el sistema de recursos.";
    }

    QIcon appIcon(":/logo.png");
    app.setWindowIcon(appIcon);

    app.setStyle(QStyleFactory::create("Fusion"));

    if (lightTheme) {
        QPalette lightPalette;
        lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));
        lightPalette.setColor(QPalette::WindowText, Qt::black);
        lightPalette.setColor(QPalette::Base, Qt::white);
        lightPalette.setColor(QPalette::AlternateBase, QColor(225, 225, 225));
        lightPalette.setColor(QPalette::ToolTipBase, Qt::black);
        lightPalette.setColor(QPalette::ToolTipText, Qt::black);
        lightPalette.setColor(QPalette::Text, Qt::black);
        lightPalette.setColor(QPalette::Button, QColor(225, 225, 225));
        lightPalette.setColor(QPalette::ButtonText, Qt::black);
        lightPalette.setColor(QPalette::BrightText, Qt::red);
        lightPalette.setColor(QPalette::Link, QColor(0, 120, 215));
        lightPalette.setColor(QPalette::Highlight, QColor(0, 120, 215));
        lightPalette.setColor(QPalette::HighlightedText, Qt::white);
        
        app.setPalette(lightPalette);
        app.setStyleSheet(
            "QMainWindow { background-color: #f0f0f0; color: #000000; }"
            "QTabWidget::pane { border: 1px solid #cccccc; background-color: #ffffff; }"
            "QTabBar::tab { background: #e1e1e1; color: #333333; padding: 8px 12px; }"
            "QTabBar::tab:selected { background: #ffffff; color: #000000; font-weight: bold; }"
            "QMenuBar { background-color: #e1e1e1; color: #000000; }"
            "QMenu { background-color: #ffffff; color: #000000; border: 1px solid #cccccc; }"
            "QStatusBar { background-color: #e1e1e1; color: #333333; border-top: 1px solid #cccccc; }"
            "QPlainTextEdit { background-color: #ffffff; color: #000000; }"
        );
    } else {
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
        app.setStyleSheet("QStatusBar { background-color: #222222; border-top: 1px solid #333333; }");
    }

    MainWindow window;
    window.setWindowIcon(appIcon);
    window.show();

    if (argc > 1) {
        QString filePath = QString::fromUtf8(argv[1]);
        if (!filePath.isEmpty() && QFile::exists(filePath)) {
            window.openFile(filePath);
        }
    }

    return app.exec();
}