#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QTreeView>
#include <QLabel>
#include <QToolBar>
#include <QMenu>
#include "filemanager.h"
#include "fileanalyzer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FileManager;  // Forward declaration

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onBatchRename();
    void onAnalyzeContent();
    void onRemoveDuplicates();
    void onDirectorySelected();
    void onSelectionChanged();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *fileModel;
    FileManager *fileManager;  // Fixed pointer declaration
    FileAnalyzer *fileAnalyzer;
    QTreeView *treeView;
    QLabel *detailsLabel;
    QToolBar *toolbar;
    
    // Add these QAction members
    QAction *selectDirAction;
    QAction *batchRenameAction;
    QAction *findDuplicatesAction;
    QAction *analyzeContentAction;
    
    void setupUI();
    void setupConnections();
};

#endif // MAINWINDOW_H
