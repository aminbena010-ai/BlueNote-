#include "FileManager.h"
#include <QFile>
#include <QSaveFile>
#include <QTextStream>
#include <QFileInfo>

namespace Core {

FileReadResult FileManager::readFile(const QString &path) {
    FileReadResult result;
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        result.success = false;
        result.errorMessage = file.errorString();
        return result;
    }

    QTextStream in(&file);
    // Configuración por defecto UTF-8 en Qt6
    in.setEncoding(QStringConverter::Utf8);

    result.content = in.readAll();
    file.close();

    result.success = true;
    result.lineEnding = detectLineEnding(result.content);
    return result;
}

bool FileManager::writeFile(const QString &path, const QString &content, LineEnding lineEnding) {
    // QSaveFile escribe en un archivo temporal y solo reemplaza el original
    // al llamar a commit(). Si hay un crash, el archivo original NO se corrompe.
    QSaveFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QString processedContent = content;

    // Normalizar saltos de línea según la preferencia del archivo
    if (lineEnding == LineEnding::CRLF) {
        processedContent.replace("\r\n", "\n");
        processedContent.replace("\n", "\r\n");
    } else if (lineEnding == LineEnding::LF) {
        processedContent.replace("\r\n", "\n");
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << processedContent;

    return file.commit();
}

LineEnding FileManager::detectLineEnding(const QString &content) {
    if (content.contains("\r\n")) {
        return LineEnding::CRLF;
    } else if (content.contains('\r') && !content.contains('\n')) {
        return LineEnding::CR;
    }
    return LineEnding::LF;
}

QString FileManager::lineEndingToString(LineEnding le) {
    switch (le) {
        case LineEnding::CRLF: return "CRLF (Windows)";
        case LineEnding::CR:   return "CR (Legacy Mac)";
        case LineEnding::LF:   
        default:               return "LF (Unix/Mac)";
    }
}

QString FileManager::formatFileSize(qint64 bytes) {
    if (bytes < 1024) return QString("%1 B").arg(bytes);
    if (bytes < 1024 * 1024) return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 2);
}

} // namespace Core