cmake_minimum_required(VERSION 3.16)
project(BlueNotePlus VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Habilitar MOC, UIC y RCC para Qt (Automático)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Buscar dependencias de Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Lista de archivos de código fuente, cabeceras y recursos
set(PROJECT_SOURCES
    src/main.cpp

    # Módulo UI
    src/ui/MainWindow.h
    src/ui/MainWindow.cpp
    src/ui/EditorTab.h
    src/ui/EditorTab.cpp
    src/ui/BlueNoteEditor.h
    src/ui/BlueNoteEditor.cpp
    src/ui/SyntaxHighlighter.h
    src/ui/SyntaxHighlighter.cpp
    src/ui/FindReplaceDialog.cpp
    src/ui/FindReplaceDialog.h

    # Módulo Core
    src/core/FileManager.h
    src/core/FileManager.cpp
    src/core/ConfigManager.h
    src/core/ConfigManager.cpp

    # Archivo de Recursos (Logo e Iconos)
    resources/resources.qrc
)

# Crear el ejecutable del proyecto
add_executable(BlueNotePlus ${PROJECT_SOURCES})

# Incluir rutas de cabeceras
target_include_directories(BlueNotePlus PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
)

# Enlazar módulos de Qt
target_link_libraries(BlueNotePlus PRIVATE Qt6::Widgets)

# Ocultar consola de terminal en Windows para ejecutable en modo Release
if(WIN32 AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(BlueNotePlus PROPERTIES WIN32_EXECUTABLE TRUE)
endif()