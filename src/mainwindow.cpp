#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QPushButton>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QPropertyAnimation>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , fileModel(new QFileSystemModel(this))
    , fileManager(new FileManager(this))
{
    ui->setupUi(this);
    
    // Create menus if they don't exist in the UI
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
    
    // Initialize the actions
    selectDirAction = new QAction(tr("Select Directory"), this);
    batchRenameAction = new QAction(tr("Batch Rename"), this);
    findDuplicatesAction = new QAction(tr("Find Duplicates"), this);
    analyzeContentAction = new QAction(tr("Analyze Content"), this);
    
    // Add actions to menus
    fileMenu->addAction(selectDirAction);
    toolsMenu->addAction(batchRenameAction);
    toolsMenu->addAction(findDuplicatesAction);
    toolsMenu->addAction(analyzeContentAction);
    
    setupUI();
    setupConnections();
}

MainWindow::~MainWindow() {
    delete ui;
    // fileModel and fileManager are QObject children and will be deleted automatically
}

void MainWindow::setupUI() {
    // Set a minimal style
    setStyleSheet("QMainWindow { background-color: #f0f0f0; }");

    // Fix: Initialize member variables that are used in other functions
    treeView = new QTreeView(this);
    treeView->setModel(fileModel);
    treeView->setRootIndex(fileModel->index(QDir::homePath()));
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeView->setColumnWidth(0, 250);
    treeView->setSortingEnabled(true);
    
    // Initialize detailsLabel
    detailsLabel = new QLabel(this);
    detailsLabel->setWordWrap(true);
    
    // Create and initialize toolbar
    toolbar = new QToolBar(this);
    toolbar->setStyleSheet("QToolBar { background-color: #ffffff; }");
    addToolBar(toolbar);
    
    // Create layout
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(treeView);
    mainLayout->addWidget(detailsLabel);
    setCentralWidget(centralWidget);
    
    // Connect selection model signals
    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onSelectionChanged);
    
    // Add animations
    QPropertyAnimation *animation = new QPropertyAnimation(treeView, "opacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

void MainWindow::setupConnections() {
    // Now these actions are properly declared and can be connected
    connect(selectDirAction, &QAction::triggered, this, &MainWindow::onDirectorySelected);
    connect(batchRenameAction, &QAction::triggered, this, &MainWindow::onBatchRename);
    connect(findDuplicatesAction, &QAction::triggered, this, &MainWindow::onRemoveDuplicates);
    connect(analyzeContentAction, &QAction::triggered, this, &MainWindow::onAnalyzeContent);
}

void MainWindow::onBatchRename() {
    // Implementation coming soon
}

void MainWindow::onAnalyzeContent() {
    // Implementation coming soon
}

void MainWindow::onRemoveDuplicates() {
    // Implementation coming soon
}

void MainWindow::onDirectorySelected() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory",
                                                  QDir::homePath(),
                                                  QFileDialog::ShowDirsOnly);
    if (!dir.isEmpty()) {
        treeView->setRootIndex(fileModel->index(dir));
        statusBar()->showMessage("Directory changed: " + dir);
    }
}

void MainWindow::onSelectionChanged() {
    QModelIndexList selection = treeView->selectionModel()->selectedIndexes();
    if (!selection.isEmpty()) {
        QString path = fileModel->filePath(selection.first());
        QFileInfo fileInfo(path);
        
        // Update details panel
        QString details = QString("Name: %1\nSize: %2 bytes\nModified: %3")
                         .arg(fileInfo.fileName())
                         .arg(fileInfo.size())
                         .arg(fileInfo.lastModified().toString());
        detailsLabel->setText(details);
        
        // Update status bar
        statusBar()->showMessage(QString("%1 items selected").arg(selection.size() / 4));
    }
}
