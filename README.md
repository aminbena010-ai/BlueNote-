# BlueNote+

> Developed by **BluePandaOpn** under **BluePanda Studios**.

**BlueNote+** is a modern, lightweight, and cross-platform text and code editor built with **C++17** and **Qt6 (Widgets)**. Designed for speed and flexibility, it features a clean modular architecture, customizable tabs, real-time syntax highlighting, and robust local file management.

---

## Key Features

- **Blazing Fast Performance**: Native C++17 core execution without heavy web-wrapper overhead.
- **Modular Architecture**: Clean separation between core backend logic (`src/core/`) and Qt GUI components (`src/ui/`).
- **Dynamic Tabbed Interface**: Seamlessly manage multiple open documents with custom editing widgets (`BlueNoteEditor`).
- **Syntax Highlighting**: Built-in customizable syntax highlighting engine (`SyntaxHighlighter`).
- **Cross-Platform Ready**: Fully compatible with Linux (AppImage) and Windows portable deployment.

---

## Project Structure
```
BlueNote/
├── src/
│   ├── core/           # Backend systems (ConfigManager, FileManager)
│   └── ui/             # Frontend views (MainWindow, EditorTab, BlueNoteEditor)
├── resources/          # Qt Resource System (.qrc) and icons
├── docs/               # Comprehensive developer documentation
└── dist/               # Packaging scripts and deployment assets
```
---

## Building from Source

Ensure you have **CMake (3.16+)**, a C++17 compiler, and **Qt6 Widgets** installed.

# Configure the project
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build the executable
cmake --build build

---

## Documentation & Guides

Explore the `docs/` directory for detailed technical guides:
- **[Installation Guide](docs/INSTALLATION.md)**: How to run and install BlueNote+.
- **[Contributing Guide](CONTRIBUTING.md)**: Coding standards and PR workflows.
- **[Architecture Overview](ARCHITECTURE.md)**: Deep dive into the internal design.
- **[Changelog](docs/CHANGELOG.md)**: Version history and planned features.

---

## License

This project is licensed under the **GNU Lesser General Public License v3.0 (LGPLv3)**. See the [LICENSE.md](LICENSE.md) file for details.
