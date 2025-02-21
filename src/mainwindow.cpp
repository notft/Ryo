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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , fileModel(new QFileSystemModel(this))
    , fileManager(new FileManager())
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
}

MainWindow::~MainWindow() {
    delete fileManager;
}

void MainWindow::setupUI() {
    // Set a minimal style
    setStyleSheet("QMainWindow { background-color: #f0f0f0; }");

    // Create central widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Create toolbar
    QToolBar* toolbar = new QToolBar(this);
    toolbar->setStyleSheet("QToolBar { background-color: #ffffff; }");
    addToolBar(toolbar);
    
    // Add toolbar actions with icons
    QAction* selectDirAction = toolbar->addAction(QIcon::fromTheme("folder-open"), "Select Directory");
    QAction* batchRenameAction = toolbar->addAction(QIcon::fromTheme("edit-rename"), "Batch Rename");
    QAction* findDuplicatesAction = toolbar->addAction(QIcon::fromTheme("edit-delete"), "Find Duplicates");
    QAction* analyzeContentAction = toolbar->addAction(QIcon::fromTheme("view-refresh"), "Analyze Content");
    
    // Create tree view
    QTreeView* treeView = new QTreeView(this);
    treeView->setModel(fileModel);
    treeView->setRootIndex(fileModel->index(QDir::homePath()));
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeView->setColumnWidth(0, 250); // Name column
    treeView->setSortingEnabled(true);
    
    // Add tree view to main layout
    mainLayout->addWidget(treeView);
    
    // Add animations
    QPropertyAnimation *animation = new QPropertyAnimation(treeView, "opacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

void MainWindow::setupConnections() {
    // Connect toolbar actions to slots
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
