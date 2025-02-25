#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QObject>

class FileManager : public QObject {
    Q_OBJECT

public:
    explicit FileManager(QObject *parent = nullptr);
    
    bool batchRename(const QStringList& files, const QString& pattern);
    QStringList findDuplicatesByContent(const QString& directory);
    QStringList findDuplicatesByMetadata(const QString& directory);
    bool removeDuplicates(const QStringList& files);

private:
    QString generateNewName(const QString& pattern, const QFileInfo& file, int index);
    bool compareFiles(const QString& file1, const QString& file2);
    QByteArray calculateFileHash(const QString& filePath);
};

#endif // FILEMANAGER_H
