# BlueNote+

A high-performance, modular C++17 text editor built with the Qt6 framework, designed for developers and power users seeking a minimalist, distraction-free environment.

---

## Features

- **Tabbed Interface**: Manage multiple documents simultaneously with clean tab lifecycle controls.
- **Syntax Highlighting**: Real-time token-based code formatting and custom syntax rules.
- **Find & Replace**: Advanced modal search and batch replacement tool supporting active editing contexts.
- **Dark UI Aesthetic**: Native integration with custom dark palettes and smooth UI responsiveness.
- **Modular Architecture**: Clean separation between core system managers (`src/core`) and the UI presentation layer (`src/ui`).

---

## Quick Installation & Execution

To compile, package, and install **BlueNote+** on Ubuntu with absolute precision, run the following commands sequentially in your terminal:

### 1. Install Build Dependencies
\`\`\`bash
sudo apt update
sudo apt install -y build-essential cmake qt6-base-dev libgl1-mesa-dev
\`\`\`

### 2. Give Execution Permissions to Automation Scripts
\`\`\`bash
chmod +x package.sh install.sh
\`\`\`

### 3. Compile and Package the Project
\`\`\`bash
./package.sh
\`\`\`

### 4. Install System-Wide
\`\`\`bash
sudo ./install.sh
\`\`\`

Once completed, **BlueNote+** will be fully integrated into your desktop application menu and ready for execution.
