#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QPushButton>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , fileModel(new QFileSystemModel(this))
    , fileManager(new FileManager())
{
    setupUI();
    setupConnections();
}

MainWindow::~MainWindow() {
    delete fileManager;
}

void MainWindow::setupUI() {
    // Create central widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Create toolbar
    QToolBar* toolbar = new QToolBar(this);
    addToolBar(toolbar);
    
    // Add toolbar actions
    QAction* selectDirAction = toolbar->addAction("Select Directory");
    QAction* batchRenameAction = toolbar->addAction("Batch Rename");
    QAction* findDuplicatesAction = toolbar->addAction("Find Duplicates");
    QAction* analyzeContentAction = toolbar->addAction("Analyze Content");
    
    // Create splitter for file view and details
    QHBoxLayout* contentLayout = new QHBoxLayout();
    
    // Setup file system model
    fileModel->setRootPath(QDir::rootPath());
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    
    // Create tree view
    QTreeView* treeView = new QTreeView(this);
    treeView->setModel(fileModel);
    treeView->setRootIndex(fileModel->index(QDir::homePath()));
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeView->setColumnWidth(0, 250); // Name column
    treeView->setSortingEnabled(true);
    
    // Create file details panel
    QWidget* detailsPanel = new QWidget(this);
    QVBoxLayout* detailsLayout = new QVBoxLayout(detailsPanel);
    
    // Add details widgets
    QLabel* detailsLabel = new QLabel("File Details", detailsPanel);
    detailsLabel->setStyleSheet("font-weight: bold;");
    detailsLayout->addWidget(detailsLabel);
    
    // Add preview area
    QLabel* previewLabel = new QLabel("Preview", detailsPanel);
    previewLabel->setStyleSheet("font-weight: bold;");
    detailsLayout->addWidget(previewLabel);
    
    QWidget* previewArea = new QWidget(detailsPanel);
    previewArea->setMinimumHeight(200);
    previewArea->setStyleSheet("background-color: #f0f0f0; border: 1px solid #ddd;");
    detailsLayout->addWidget(previewArea);
    
    detailsLayout->addStretch();
    
    // Add views to content layout
    contentLayout->addWidget(treeView, 7);  // 70% of space
    contentLayout->addWidget(detailsPanel, 3);  // 30% of space
    
    // Add layouts to main layout
    mainLayout->addLayout(contentLayout);
    
    // Create status bar
    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->showMessage("Ready");
    
    // Set window properties
    resize(1200, 800);
    setWindowTitle("File Manager");
    
    // Store widgets for later access
    this->treeView = treeView;
}

void MainWindow::setupConnections() {
    // Connect toolbar actions
    connect(toolbar->actions()[0], &QAction::triggered, this, &MainWindow::onDirectorySelected);
    connect(toolbar->actions()[1], &QAction::triggered, this, &MainWindow::onBatchRename);
    connect(toolbar->actions()[2], &QAction::triggered, this, &MainWindow::onRemoveDuplicates);
    connect(toolbar->actions()[3], &QAction::triggered, this, &MainWindow::onAnalyzeContent);
    
    // Connect tree view selection changes
    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onSelectionChanged);
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
