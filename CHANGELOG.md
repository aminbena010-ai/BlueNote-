# Changelog

All notable changes to the **BlueNote+** project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [Unreleased]

### Planned Features
- Full session persistence (auto-reopening previously closed tabs).
- Search and Replace functionality with regular expression support.
- Custom theme switching (Dark / Light palette toggle).

---

## [0.1.0] - 2026-07-23

### Added
- Core C++17 application engine built with Qt6 Widgets framework.
- Modular code architecture separating UI components (`src/ui`) from core systems (`src/core`).
- Tabbed editor system with custom `BlueNoteEditor` and syntax highlighting features.
- Cross-platform 'Fusion' Dark Palette theme initialization.
- Embedded Qt Resource System (`resources.qrc`) bundling the application PNG logo.
- Standalone Linux packaging strategy via `linuxdeployqt` (AppImage generation).
- Desktop entry integration (`bluenoteplus.desktop`) for GNOME/Wayland window managers.
- Complete developer documentation structure in `docs/`.

### Fixed
- Resolved compilation errors caused by legacy `Ui::` namespace qualification during Qt UI migration.
- Fixed generic system gear icon bug in GNOME taskbars by configuring explicit `DesktopFileName` bindings.
- Corrected relative path resolution in `linuxdeployqt` desktop icon lookup.
