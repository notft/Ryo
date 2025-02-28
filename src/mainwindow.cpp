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
#include <QProgressDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QSplitter>

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
    setupDarkTheme();
    
    // Create main splitter
    mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // Left side - File tree
    QWidget* leftWidget = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    
    treeView = new QTreeView(this);
    treeView->setModel(fileModel);
    treeView->setRootIndex(fileModel->index(QDir::homePath()));
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeView->setColumnWidth(0, 250);
    treeView->setSortingEnabled(true);
    
    leftLayout->addWidget(treeView);
    
    // Right side - Details and duplicates
    QWidget* rightWidget = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    
    detailsLabel = new QLabel(this);
    detailsLabel->setWordWrap(true);
    rightLayout->addWidget(detailsLabel);
    
    setupDuplicatesUI();
    rightLayout->addWidget(duplicatesList);
    rightLayout->addWidget(removeDuplicatesButton);
    
    // Add widgets to splitter
    mainSplitter->addWidget(leftWidget);
    mainSplitter->addWidget(rightWidget);
    mainSplitter->setStretchFactor(0, 2);
    mainSplitter->setStretchFactor(1, 1);
    
    setCentralWidget(mainSplitter);
}

void MainWindow::setupDarkTheme() {
    QString darkStyle = R"(
        QMainWindow, QWidget {
            background-color: #2b2b2b;
            color: #ffffff;
        }
        QTreeView, QListWidget {
            background-color: #333333;
            color: #ffffff;
            border: 1px solid #555555;
        }
        QTreeView::item:selected, QListWidget::item:selected {
            background-color: #2d5a88;
        }
        QLabel {
            color: #ffffff;
        }
        QPushButton {
            background-color: #444444;
            color: #ffffff;
            border: 1px solid #555555;
            padding: 5px 15px;
            border-radius: 3px;
        }
        QPushButton:hover {
            background-color: #555555;
        }
        QPushButton:pressed {
            background-color: #666666;
        }
        QMenuBar {
            background-color: #2b2b2b;
            color: #ffffff;
        }
        QMenuBar::item:selected {
            background-color: #444444;
        }
        QMenu {
            background-color: #2b2b2b;
            color: #ffffff;
        }
        QMenu::item:selected {
            background-color: #444444;
        }
        QToolBar {
            background-color: #333333;
            border: 1px solid #555555;
        }
        QStatusBar {
            background-color: #2b2b2b;
            color: #ffffff;
        }
    )";
    
    setStyleSheet(darkStyle);
}

void MainWindow::setupDuplicatesUI() {
    duplicatesList = new QListWidget(this);
    duplicatesList->setSelectionMode(QAbstractItemView::MultiSelection);
    duplicatesList->setHidden(true);
    
    removeDuplicatesButton = new QPushButton("Remove Selected Duplicates", this);
    removeDuplicatesButton->setHidden(true);
    
    connect(removeDuplicatesButton, &QPushButton::clicked, this, &MainWindow::onRemoveDuplicates);
}

void MainWindow::setupConnections() {
    // Now these actions are properly declared and can be connected
    connect(selectDirAction, &QAction::triggered, this, &MainWindow::onDirectorySelected);
    connect(batchRenameAction, &QAction::triggered, this, &MainWindow::onBatchRename);
    connect(findDuplicatesAction, &QAction::triggered, this, &MainWindow::onFindDuplicates);
    connect(analyzeContentAction, &QAction::triggered, this, &MainWindow::onAnalyzeContent);
}

void MainWindow::onBatchRename() {
    QModelIndexList selection = treeView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select files to rename");
        return;
    }

    // Collect file paths
    QStringList files;
    for (const QModelIndex& index : selection) {
        files << fileModel->filePath(index);
    }

    // Show pattern input dialog with help text
    bool ok;
    QString helpText = "Available patterns:\n"
                      "%n - number (001, 002, etc)\n"
                      "%d - date (YYYYMMDD)\n"
                      "%o - original name\n"
                      "%e - extension";
                      
    QString pattern = QInputDialog::getText(this, "Batch Rename",
                                          "Enter rename pattern:\n\n" + helpText,
                                          QLineEdit::Normal,
                                          "file_%n", &ok);
    if (!ok || pattern.isEmpty()) {
        return;
    }

    // Create progress dialog
    QProgressDialog progress("Renaming files...", "Cancel", 0, files.size(), this);
    progress.setWindowModality(Qt::WindowModal);

    // Connect signals
    connect(fileManager, &FileManager::progressUpdated,
            &progress, &QProgressDialog::setValue);
    
    connect(fileManager, &FileManager::operationCompleted,
            this, [this](bool success, const QString& message) {
                if (success) {
                    QMessageBox::information(this, "Success", message);
                } else {
                    QMessageBox::warning(this, "Operation Complete", message);
                }
            });

    // Start batch rename
    fileManager->batchRename(files, pattern);
}

void MainWindow::onAnalyzeContent() {
    // Implementation coming soon
}

void MainWindow::onFindDuplicates() {
    QString currentPath = fileModel->filePath(treeView->rootIndex());
    
    QProgressDialog progress("Finding duplicates...", "Cancel", 0, 100, this);
    progress.setWindowModality(Qt::WindowModal);
    
    connect(fileManager, &FileManager::progressUpdated,
            &progress, &QProgressDialog::setValue);
            
    QStringList duplicates = fileManager->findDuplicatesByContent(currentPath);
    onDuplicatesFound(duplicates);
}

void MainWindow::onDuplicatesFound(const QStringList& duplicates) {
    duplicatesList->clear();
    
    if (duplicates.isEmpty()) {
        QMessageBox::information(this, "No Duplicates", "No duplicate files found.");
        duplicatesList->setHidden(true);
        removeDuplicatesButton->setHidden(true);
        return;
    }
    
    for (const QString& file : duplicates) {
        QListWidgetItem* item = new QListWidgetItem(file);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        duplicatesList->addItem(item);
    }
    
    duplicatesList->setHidden(false);
    removeDuplicatesButton->setHidden(false);
}

void MainWindow::onRemoveDuplicates() {
    QStringList filesToRemove;
    
    for (int i = 0; i < duplicatesList->count(); ++i) {
        QListWidgetItem* item = duplicatesList->item(i);
        if (item->checkState() == Qt::Checked) {
            filesToRemove << item->text();
        }
    }
    
    if (filesToRemove.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select files to remove");
        return;
    }
    
    if (QMessageBox::question(this, "Confirm Deletion",
                            QString("Are you sure you want to remove %1 files?")
                            .arg(filesToRemove.size())) == QMessageBox::Yes) {
        fileManager->removeDuplicates(filesToRemove);
    }
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
