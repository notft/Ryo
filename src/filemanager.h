#pragma once

#include <QString>
#include <QStringList>
#include <QFileInfo>

class FileManager {
public:
    FileManager();
    
    bool batchRename(const QStringList& files, const QString& pattern);
    QStringList findDuplicatesByContent(const QString& directory);
    QStringList findDuplicatesByMetadata(const QString& directory);
    bool removeDuplicates(const QStringList& files);

private:
    QString generateNewName(const QString& pattern, const QFileInfo& file, int index);
    bool compareFiles(const QString& file1, const QString& file2);
    QByteArray calculateFileHash(const QString& filePath);
};
