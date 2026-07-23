#pragma once

#include <QString>

namespace Core {

enum class LineEnding {
    LF,      // Linux / macOS
    CRLF,    // Windows
    CR       // Legacy Mac
};

struct FileReadResult {
    bool success = false;
    QString content;
    QString encodingName = "UTF-8";
    LineEnding lineEnding = LineEnding::LF;
    QString errorMessage;
};

class FileManager {
public:
    // Lee un archivo detectando codificación UTF-8 / BOM y fin de línea
    static FileReadResult readFile(const QString &path);

    // Guarda de forma atómica (usando archivo temporal previo al swap)
    static bool writeFile(const QString &path, const QString &content, LineEnding lineEnding = LineEnding::LF);

    // Utilidades
    static QString lineEndingToString(LineEnding le);
    static LineEnding detectLineEnding(const QString &content);
    static QString formatFileSize(qint64 bytes);
};

} // namespace Core