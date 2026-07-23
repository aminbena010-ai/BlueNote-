# Installation Guide

Follow these simple instructions to build, package, and install **BlueNote+** on Ubuntu and other Linux distributions with absolute precision.

---

## Prerequisites & Dependencies

Before building from source, ensure your system has a modern C++17 compiler, CMake, and the required Qt6 development packages installed. Run the following command in your terminal:

```bash
sudo apt update
sudo apt install -y build-essential cmake qt6-base-dev libgl1-mesa-dev
```

---

## Method 1: Automated Build & Installation (Recommended)

The project includes automated scripts (`package.sh` and `install.sh`) to streamline compilation, packaging, and system deployment.

1. **Navigate to the project root directory:**
   ```bash
   cd ~/Documentos/BlueNote
   ```

2. **Grant execution permissions to the scripts:**
   ```bash
   chmod +x package.sh install.sh
   ```

3. **Compile and package the project:**
   ```bash
   ./package.sh
   ```

4. **Install system-wide:**
   ```bash
   sudo ./install.sh
   ```

---

## Method 2: Manual Compilation via CMake

If you prefer to compile the application manually without automation scripts:

1. **Configure the project build environment:**
   ```bash
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   ```

2. **Build the executable:**
   ```bash
   cmake --build build --parallel $(nproc)
   ```

3. **Locate the compiled binary:**
   The compiled executable will be available inside the `build/` directory:
   ```bash
   ./build/BlueNotePlus
   ```

---

## Running the Application

Once installed via the system script, you can launch **BlueNote+** directly from your desktop application launcher (GNOME/KDE/Wayland) or by typing its name in your terminal:

```bash
BlueNotePlus
```