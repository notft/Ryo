#pragma once

#include <QMainWindow>
#include <QFileSystemModel>
#include <QTreeView>
#include <QLabel>
#include <QToolBar>
#include "filemanager.h"
#include "fileanalyzer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    FileManager *fileManager;
    FileAnalyzer *fileAnalyzer;
    QTreeView *treeView;
    QLabel *detailsLabel;
    QToolBar *toolbar;
    
    void setupUI();
    void setupConnections();
};
