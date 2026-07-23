#!/bin/bash
set -e

echo "=========================================="
echo "1. Limpiando directorios anteriores..."
echo "=========================================="
rm -rf build dist *.AppImage

echo "=========================================="
echo "2. Configurando y compilando con CMake..."
echo "=========================================="
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

echo "=========================================="
echo "3. Creando estructura AppDir (dist)..."
echo "=========================================="
mkdir -p dist/usr/bin
mkdir -p dist/usr/share/applications
mkdir -p dist/usr/share/icons/hicolor/256x256/apps

cp build/BlueNotePlus dist/usr/bin/
cp resources/logo.png dist/usr/share/icons/hicolor/256x256/apps/bluenoteplus.png
cp resources/logo.png dist/bluenoteplus.png

cat << 'APPRUN' > dist/AppRun
#!/bin/bash
HERE="$(dirname "$(readlink -f "${0}")")"
exec "${HERE}/usr/bin/BlueNotePlus" "$@"
APPRUN
chmod +x dist/AppRun

cat << 'DESKTOP' > dist/usr/share/applications/bluenoteplus.desktop
[Desktop Entry]
Type=Application
Name=BlueNote+
Exec=BlueNotePlus
Icon=bluenoteplus
Categories=Utility;TextEditor;
Terminal=false
StartupWMClass=BlueNotePlus
MimeType=text/plain;
DESKTOP

cp dist/usr/share/applications/bluenoteplus.desktop dist/bluenoteplus.desktop

echo "=========================================="
echo "4. Generando empaquetado final..."
echo "=========================================="
wget -q https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod +x appimagetool-x86_64.AppImage
./appimagetool-x86_64.AppImage dist BlueNotePlus-x86_64.AppImage
rm appimagetool-x86_64.AppImage

echo "=========================================="
echo "¡Empaquetado completado con éxito!"
echo "=========================================="
