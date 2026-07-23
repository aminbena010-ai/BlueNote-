# Contributing to BlueNote+

Thank you for your interest in contributing to **BlueNote+**! We welcome contributions from developers of all skill levels. Please take a moment to review this guide to ensure a smooth and effective collaboration process.

---

## Code of Conduct

We are committed to providing a welcoming, respectful, and inclusive environment for everyone. Please maintain professionalism and courtesy in all project interactions, issue discussions, and pull requests.

---

## How to Contribute

### 1. Reporting Bugs
Before opening a new issue, search existing issues to see if the bug has already been reported.

When submitting a bug report, please include:
- **Operating System** (e.g., Ubuntu 24.04 LTS, Windows 11).
- **Desktop Environment / Display Server** (e.g., GNOME / Wayland, X11).
- **Steps to reproduce** the bug clearly.
- **Expected vs. Actual behavior**.
- Terminal stdout/stderr logs or screenshots if applicable.

### 2. Suggesting Enhancements
Feature requests are welcome! When opening a feature proposal issue, describe:
- The problem or missing functionality you want solved.
- How you envision the feature working inside the current UI/UX layout.

### 3. Submitting Code (Pull Requests)
1. **Fork** the repository and create a feature branch off `main`:
   git checkout -b feature/my-new-feature
2. Make your changes adhering to the coding standards below.
3. Ensure the project builds without errors or warnings:
   cmake -B build -DCMAKE_BUILD_TYPE=Debug
   cmake --build build
4. Commit your changes using clear, imperative commit messages:
   git commit -m "Add regex search support to BlueNoteEditor"
5. Push to your fork and open a **Pull Request (PR)** targeting the `main` branch.

---

## Coding & Style Standards

### C++ Guidelines
- **Standard**: C++17.
- **Naming Conventions**:
  - **Classes / Structs**: `PascalCase` (e.g., `BlueNoteEditor`, `ConfigManager`).
  - **Methods / Functions**: `camelCase` (e.g., `loadFromFile()`, `updateHighlighter()`).
  - **Member Variables**: Prefix with `m_` (e.g., `m_tabWidget`, `m_filePath`).
  - **Constants / Enums**: `ALL_CAPS` or `PascalCase` enum classes.
- **Architecture**: Keep UI logic (`src/ui/`) strictly decoupled from backend engine logic (`src/core/`).

### Qt Best Practices
- **Memory Management**: Use Qt's parent-child hierarchy for automatic memory cleanup (`QWidget(parent)`). Avoid raw `delete` calls where parent ownership applies.
- **Resource System**: Keep external icons, themes, and graphic assets registered inside `resources/resources.qrc`.
- **Dynamic Linking**: Ensure all Qt includes use forward declarations in headers (`.h`) whenever possible to reduce compile times.

---

## Licensing & Copyright

By contributing to **BlueNote+**, you agree that your contributions will be licensed under the project's **GNU Lesser General Public License v3.0 (LGPLv3)** or any compatible open-source license adopted by the repository maintainer.
