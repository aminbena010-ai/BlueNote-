#!/bin/bash
set -e

if [ ! -f "BlueNotePlus-x86_64.AppImage" ]; then
    echo "Error: No se encuentra 'BlueNotePlus-x86_64.AppImage'."
    echo "Ejecuta primero './package.sh' para generar el empaquetado."
    exit 1
fi

echo "=========================================="
echo "Limpiando instalación anterior..."
echo "=========================================="
rm -f ~/.local/bin/BlueNotePlus.AppImage
rm -f ~/.local/share/applications/bluenoteplus.desktop
rm -f ~/.local/share/icons/bluenoteplus.png

echo "=========================================="
echo "Instalando nueva versión en el sistema..."
echo "=========================================="
mkdir -p ~/.local/share/applications
mkdir -p ~/.local/share/icons
mkdir -p ~/.local/bin

cp BlueNotePlus-x86_64.AppImage ~/.local/bin/BlueNotePlus.AppImage
chmod +x ~/.local/bin/BlueNotePlus.AppImage

# Copiar el icono directamente desde la carpeta de recursos del proyecto
if [ -f "resources/logo.png" ]; then
    cp resources/logo.png ~/.local/share/icons/bluenoteplus.png
else
    ~/.local/bin/BlueNotePlus.AppImage --appimage-extract usr/share/icons/hicolor/256x256/apps/bluenoteplus.png > /dev/null 2>&1
    cp squashfs-root/usr/share/icons/hicolor/256x256/apps/bluenoteplus.png ~/.local/share/icons/bluenoteplus.png 2>/dev/null || true
    rm -rf squashfs-root
fi

# Crear el archivo .desktop usando la ruta absoluta oficial con $HOME
cat << DESKTOP > ~/.local/share/applications/bluenoteplus.desktop
[Desktop Entry]
Type=Application
Name=BlueNote+
Exec=$HOME/.local/bin/BlueNotePlus.AppImage %f
Icon=bluenoteplus
Categories=Utility;TextEditor;
Terminal=false
StartupWMClass=BlueNotePlus
MimeType=text/plain;
DESKTOP

update-desktop-database ~/.local/share/applications/

echo "=========================================="
echo "¡Instalación completada con éxito!"
echo "Ya puedes buscar 'BlueNote+' en el menú de aplicaciones de tu Ubuntu."
echo "=========================================="
