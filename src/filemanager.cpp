#include "filemanager.h"
#include <QCryptographicHash>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QRegularExpression>

FileManager::FileManager(QObject *parent) : QObject(parent) {
}

bool FileManager::batchRename(const QStringList& files, const QString& pattern) {
    if (files.isEmpty() || pattern.isEmpty()) {
        emit operationCompleted(false, "No files selected or empty pattern");
        return false;
    }

    int total = files.size();
    int success = 0;
    QStringList errors;

    for (int i = 0; i < files.size(); ++i) {
        QString filePath = files[i];
        QFileInfo fileInfo(filePath);
        
        if (!fileInfo.exists()) {
            errors << QString("File not found: %1").arg(filePath);
            continue;
        }

        QString newName = pattern;
        
        newName.replace("%n", QString::number(i + 1).rightJustified(3, '0'));
        newName.replace("%d", QDateTime::currentDateTime().toString("yyyyMMdd"));
        newName.replace("%o", fileInfo.baseName());
        newName.replace("%e", fileInfo.suffix());

        QString newPath = fileInfo.absolutePath() + QDir::separator() + newName;
        if (!fileInfo.suffix().isEmpty()) {
            newPath += "." + fileInfo.suffix();
        }

        if (QFile::exists(newPath) && newPath != filePath) {
            errors << QString("Target file already exists: %1").arg(newPath);
            continue;
        }

        if (QFile::rename(filePath, newPath)) {
            success++;
        } else {
            errors << QString("Failed to rename: %1").arg(filePath);
        }

        emit progressUpdated(i + 1, total);
    }

    QString message = QString("Renamed %1 of %2 files").arg(success).arg(total);
    if (!errors.isEmpty()) {
        message += "\nErrors:\n" + errors.join("\n");
    }

    emit operationCompleted(success == total, message);
    return success == total;
}

QStringList FileManager::findDuplicatesByContent(const QString& directory) {
    QStringList duplicates;
    QDir dir(directory);
    QStringList files = dir.entryList(QDir::Files);
    
    QMap<QByteArray, QStringList> hashMap;
    int total = files.size();
    
    for (int i = 0; i < files.size(); ++i) {
        QString filePath = dir.absoluteFilePath(files[i]);
        QByteArray hash = calculateFileHash(filePath);
        
        if (!hash.isEmpty()) {
            hashMap[hash].append(filePath);
        }
        
        emit progressUpdated(i + 1, total);
    }
    
    for (const QStringList& fileList : hashMap) {
        if (fileList.size() > 1) {
            duplicates.append(fileList);
        }
    }
    
    emit operationCompleted(true, 
        QString("Found %1 sets of duplicate files").arg(duplicates.size() / 2));
    return duplicates;
}

QStringList FileManager::findDuplicatesByMetadata(const QString& directory) {
    QStringList duplicates;
    QDir dir(directory);
    QStringList files = dir.entryList(QDir::Files);
    
    return duplicates;
}

bool FileManager::removeDuplicates(const QStringList& files) {
    if (files.isEmpty()) {
        emit operationCompleted(false, "No files selected");
        return false;
    }
    
    int total = files.size();
    int removed = 0;
    QStringList errors;
    
    for (int i = 0; i < files.size(); ++i) {
        QFile file(files[i]);
        if (file.remove()) {
            removed++;
        } else {
            errors << QString("Failed to remove: %1").arg(files[i]);
        }
        
        emit progressUpdated(i + 1, total);
    }
    
    QString message = QString("Removed %1 of %2 duplicate files").arg(removed).arg(total);
    if (!errors.isEmpty()) {
        message += "\nErrors:\n" + errors.join("\n");
    }
    
    emit operationCompleted(removed == total, message);
    return removed == total;
}

QString FileManager::generateNewName(const QString& pattern, const QFileInfo& file, int index) {
    QString dir = file.absolutePath();
    QString newName = pattern.arg(index);
    return dir + QDir::separator() + newName + "." + file.suffix();
}

bool FileManager::compareFiles(const QString& file1, const QString& file2) {
    return calculateFileHash(file1) == calculateFileHash(file2);
}

QByteArray FileManager::calculateFileHash(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly)) {
        return QByteArray();
    }
    
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(&file);
    return hash.result();
}
