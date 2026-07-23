# Architecture Overview

BlueNote+ is structured around a modular, decoupled C++ architecture leveraging the Qt6 framework. The codebase is cleanly separated into core system management and the graphical presentation layer.

## Directory Structure & Component Design

### 1. Core Layer (`src/core/`)
Handles the non-graphical backend operations, disk interactions, and application state.
- **FileManager (`FileManager.h` / `FileManager.cpp`)**: Manages input/output streams, encoding detection, safe file writing, and disk parsing.
- **ConfigManager (`ConfigManager.h` / `ConfigManager.cpp`)**: Handles persistent settings, user preferences, and window states across sessions.

### 2. Presentation / UI Layer (`src/ui/`)
Manages the user interface, custom widget subclassing, and event loops.
- **MainWindow (`MainWindow.h` / `MainWindow.cpp`)**: Root container holding the menu bar, status bar, dock widgets, and the central tab management system.
- **EditorTab (`EditorTab.h` / `EditorTab.cpp`)**: Encapsulates individual tab views and binds local editor instances.
- **BlueNoteEditor (`BlueNoteEditor.h` / `BlueNoteEditor.cpp`)**: Custom text editing widget subclassing `QPlainTextEdit` optimized for high-performance rendering.
- **SyntaxHighlighter (`SyntaxHighlighter.h` / `SyntaxHighlighter.cpp`)**: Implements real-time text parsing and token-based color formatting.
- **FileMenuManager (`FileMenuManager.h` / `FileMenuManager.cpp`)**: Controls file history, recent document lists, and session states.
- **FindReplaceDialog (`FindReplaceDialog.h` / `FindReplaceDialog.cpp`)**: Modal UI widget handling text lookup, iteration, and batch replacement operations over active documents.

### 3. Application Lifecycle (`src/`)
- **main.cpp**: Initializes the `QApplication` event loop, loads application styles, and instantiates the `MainWindow`.
