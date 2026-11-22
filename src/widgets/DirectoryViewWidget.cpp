#include "widgets/DirectoryViewWidget.hpp"

#include <QFileIconProvider>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QBoxLayout>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QDesktopServices>

#include "dialogs/CasePropertiesEditorDialog.hpp"
#include "data/IconThemes.hpp"

static const QStringList importantFiles = {"data.txt", "intro.txt", "evidence.txt"};
static const QStringList nonSubScriptableExtensions = {
    ".mcro",
    ".examine.txt",
    ".move.txt",
    ".talk.txt",
    ".present.txt",
    "menu.txt",
    "menu.script.txt"
};
static const QStringList nonCaseFolders = {
    "art",
    "music",
    "movies",
    "sfx"
};

void DeselectableTreeView::mousePressEvent(QMouseEvent* event)
{
    clearSelection();
    QTreeView::mousePressEvent(event);
}

DirectoryViewWidget::DirectoryViewWidget(QWidget* parent)
    : QDockWidget(parent),
      m_mainWidget(new QWidget()),
      m_gameTitleLabel(new QLabel()),
      m_gamePropertiesButton(new QPushButton()),
      m_directoryView(new DeselectableTreeView()),
      m_topWidget(new QWidget()),
      m_dummyTitleBar(new QWidget),
      m_newCaseAction(new QAction("Add New Case", this)),
      m_newScriptAction(new QAction("Add New Script", this)),
      m_openDirAction(new QAction("Open Folder in File Manager", this)),
      m_renameFileAction(new QAction("Rename", this)),
      m_removeFileAction(new QAction("Remove", this)),
      m_createExamineScriptAction(new QAction("Create Examine Script", this)),
      m_createMoveScriptAction(new QAction("Create Move Script", this)),
      m_createPresentScriptAction(new QAction("Create Present Script", this)),
      m_createTalkScriptAction(new QAction("Create Talk Script", this))
{
    setWindowTitle("Directory View");
    setObjectName("DirectoryViewWidget");
    setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    setMinimumWidth(150);

    auto gamePropertiesIconPath = iconPathFromTheme(ICON_NAME_SETTINGS);
    m_gamePropertiesButton->setIcon(QIcon(gamePropertiesIconPath));
    m_gamePropertiesButton->setToolTip("Game Properties");
    m_gamePropertiesButton->setEnabled(false);
    m_gamePropertiesButton->setFlat(true);
    m_gamePropertiesButton->setMaximumWidth(30);

    m_directoryView->setContextMenuPolicy(Qt::CustomContextMenu);

    m_dummyTitleBar->setLayout(new QHBoxLayout());
    m_dummyTitleBar->layout()->setContentsMargins(2, 2, 2, 2);

    m_newCaseAction->setStatusTip("Add a new case to the current game");
    m_newScriptAction->setStatusTip("Add a new script to the selected case folder");
    m_openDirAction->setStatusTip("Open selected folder in the default File Manager");
    m_renameFileAction->setStatusTip("Rename selected item");
    m_removeFileAction->setStatusTip("Remove selected item");
    m_createExamineScriptAction->setStatusTip("Create Examine script for the selected scene");
    m_createMoveScriptAction->setStatusTip("Create Move script for the selected scene");
    m_createPresentScriptAction->setStatusTip("Create Present script for the selected scene");
    m_createTalkScriptAction->setStatusTip("Create Talk script for the selected scene");

    createCustomTitlebar();

    connectSignalsAndSlots();
    prepareMainLayout();

    setWidget(m_mainWidget);
    setTitleBarWidget(m_dummyTitleBar);
}

void DirectoryViewWidget::connectSignalsAndSlots()
{
    connect(this, &QDockWidget::visibilityChanged,
        this, &DirectoryViewWidget::handleVisibilityChange);
    connect(m_directoryView, &DeselectableTreeView::doubleClicked,
        this, &DirectoryViewWidget::handleTreeViewDoubleClicked);
    connect(m_gamePropertiesButton, &QPushButton::clicked,
        this, &DirectoryViewWidget::handleGamePropertiesButtonClicked);
    connect(m_directoryView, &DeselectableTreeView::customContextMenuRequested,
        this, &DirectoryViewWidget::handleViewContextMenu);

    connect(m_newCaseAction, &QAction::triggered,
        this, &DirectoryViewWidget::handleAddNewCaseAction);
    connect(m_newScriptAction, &QAction::triggered,
        this, &DirectoryViewWidget::handleAddNewScriptAction);
    connect(m_openDirAction, &QAction::triggered,
        this, &DirectoryViewWidget::handleOpenDirAction);
    connect(m_renameFileAction, &QAction::triggered,
        this, &DirectoryViewWidget::handleRenameFileAction);
    connect(m_removeFileAction, &QAction::triggered,
        this, &DirectoryViewWidget::handleRemoveFileAction);
    connect(m_createExamineScriptAction, &QAction::triggered,
        this, [this]() { createSubScriptFile("examine"); });
    connect(m_createMoveScriptAction, &QAction::triggered,
        this, [this]() { createSubScriptFile("move"); });
    connect(m_createPresentScriptAction, &QAction::triggered,
        this, [this]() { createSubScriptFile("present"); });
    connect(m_createTalkScriptAction, &QAction::triggered,
        this, [this]() { createSubScriptFile("talk"); });
}

void DirectoryViewWidget::prepareMainLayout()
{
    auto mainWidgetLayout = new QVBoxLayout();

    mainWidgetLayout->addWidget(m_topWidget);
    mainWidgetLayout->addWidget(m_directoryView);

    m_mainWidget->setLayout(mainWidgetLayout);

    setLayout(new QVBoxLayout());
}

void DirectoryViewWidget::createCustomTitlebar()
{
    auto layout = new QHBoxLayout();
    
    layout->addWidget(m_gameTitleLabel);
    layout->addStretch();
    layout->addWidget(m_gamePropertiesButton);

    layout->setContentsMargins(4, 0, 2, 0);

    m_topWidget->setLayout(layout);
}

void DirectoryViewWidget::updateDirectoryView(const PyWrightGameInfo& selectedGameInfo)
{
    auto fsModel = new QFileSystemModel(this);
    auto iconProvider = new QFileIconProvider();

    fsModel->setIconProvider(iconProvider);

    fsModel->setNameFilters({"*.txt", "*.mcro"});
    fsModel->setNameFilterDisables(false);

    m_gameInfo = selectedGameInfo;

    m_gameTitleLabel->setText(m_gameInfo->getGameFolderName());

    m_directoryView->setModel(fsModel);
    m_directoryView->setRootIndex(fsModel->setRootPath(QString{m_gameInfo->getGamePath().c_str()}));
    m_directoryView->setIndentation(20);
    m_directoryView->setSortingEnabled(true);
    m_directoryView->sortByColumn(0, Qt::AscendingOrder);
    m_directoryView->header()->hide();
    m_directoryView->hideColumn(1);
    m_directoryView->hideColumn(2);
    m_directoryView->hideColumn(3);

    m_gamePropertiesButton->setEnabled(true);
}

void DirectoryViewWidget::clearDirectoryView()
{
    m_directoryView->setModel(nullptr);
    m_gameTitleLabel->setText("");
    m_gamePropertiesButton->setEnabled(false);
}

void DirectoryViewWidget::createSubScriptFile(const QString& subScriptName)
{
    auto index = m_directoryView->selectionModel()->selectedIndexes()[0];
    auto model = static_cast<const QFileSystemModel*>(index.model());

    auto dirPath = fs::path{model->filePath(index).toStdString()} / "..";
    auto fileName = model->fileName(index);

    // Check if the file has an extra .script suffix first
    auto scriptNameSections = fileName.split(".");

    if (scriptNameSections.size() < 2)
        return;

    auto baseScriptName = scriptNameSections[0];

    auto resultingScriptName = baseScriptName + "." + subScriptName + ".txt";

    auto menuScriptName = baseScriptName + "menu";

    if (scriptNameSections[1].toLower() == "script")
        menuScriptName += "." + scriptNameSections[1];

    menuScriptName += ".txt";

    auto menuScriptFullPath = dirPath / menuScriptName.toStdString();

    if (!fs::exists(menuScriptFullPath) || !fs::is_regular_file(menuScriptFullPath))
    {
        QFile menuFile(menuScriptFullPath);

        if (menuFile.open(QIODevice::WriteOnly | QIODevice::Text))
            menuFile.close();
    }

    auto fullFilePath = dirPath / resultingScriptName.toStdString();

    if (fs::exists(fullFilePath) && fs::is_regular_file(fullFilePath))
    {
        QMessageBox::critical(this,
            "Error",
            QString{"Script named \"%1\" already exists!"}.arg(resultingScriptName)
        );
        return;
    }

    QFile fullFile(fullFilePath);

    if (fullFile.open(QIODevice::WriteOnly | QIODevice::Text))
            fullFile.close();
}

void DirectoryViewWidget::handleTreeViewDoubleClicked(const QModelIndex& index)
{
    auto itemPath = QFileSystemModel().filePath(index);

    if (fs::is_regular_file(fs::path{itemPath.toStdString()}))
        emit openNewTab(itemPath);
}

void DirectoryViewWidget::handleGamePropertiesButtonClicked()
{
    emit openGamePropertiesTab();
}

void DirectoryViewWidget::handleViewContextMenu(const QPoint& pos)
{
    if (m_gameTitleLabel->text().isEmpty())
        return;

    auto indexes = m_directoryView->selectionModel()->selectedIndexes();

    auto menu = new QMenu(this);

    auto index = indexes[0];
    auto model = static_cast<const QFileSystemModel*>(index.model());

    if (!indexes.isEmpty())
    {
        auto fileName = model->fileName(index);

        if (model->isDir(index) && !nonCaseFolders.contains(fileName))
        {
            menu->addAction(m_newScriptAction);
        }
        else if ((fileName.endsWith(".txt") || fileName.endsWith(".mcro")) && !importantFiles.contains(fileName))
        {
            bool shouldAddCreateSections = true;

            for (auto& ext : nonSubScriptableExtensions)
            {
                if (fileName.endsWith(ext))
                {
                    shouldAddCreateSections = false;
                    break;
                }
            }

            if (shouldAddCreateSections)
            {
                menu->addAction(m_createExamineScriptAction);
                menu->addAction(m_createMoveScriptAction);
                menu->addAction(m_createPresentScriptAction);
                menu->addAction(m_createTalkScriptAction);
            }

            menu->addAction(m_renameFileAction);
            menu->addAction(m_removeFileAction);
        }
        menu->addSeparator();
    }
    menu->addAction(m_newCaseAction);

    if (indexes.isEmpty() || model->isDir(index))
    {
        menu->addSeparator();
        menu->addAction(m_openDirAction);
    }

    menu->exec(m_directoryView->mapToGlobal(pos));
}

void DirectoryViewWidget::handleAddNewCaseAction()
{
    auto addDialog = new CasePropertiesEditorDialog(*m_gameInfo, nullptr, this);

    if (addDialog->exec())
        m_gameInfo->createNewCase(*addDialog->getCase());
}

void DirectoryViewWidget::handleAddNewScriptAction()
{
    auto index = m_directoryView->selectionModel()->selectedIndexes()[0];
    auto model = static_cast<const QFileSystemModel*>(index.model());

    if (!model->isDir(index))
        return;

    auto dirPath = model->filePath(index);

    auto name = QInputDialog::getText(this, "New Script", "Enter file name", QLineEdit::Normal);

    if (!name.isEmpty())
    {
        if (!name.endsWith(".mcro") || !name.endsWith(".txt"))
            name += ".txt";

        auto finalPath = QString{"%1/%2"}.arg(dirPath, name);

        if (fs::exists(fs::path{finalPath.toStdString()}))
        {
            QMessageBox::critical(this, "Error", "File already exists!");
            return;
        }

        QFile newFile(finalPath);

        // We just need to create a new file
        if (newFile.open(QIODevice::WriteOnly | QIODevice::Text))
            newFile.close();
    }
}

void DirectoryViewWidget::handleOpenDirAction()
{
    auto selectedIndexes = m_directoryView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty())
    {
        // Open the game's root dir instead
        QDesktopServices::openUrl(QUrl::fromLocalFile(QString{m_gameInfo->getGamePath().c_str()}));
        return;
    }

    auto index = selectedIndexes[0];
    auto model = static_cast<const QFileSystemModel*>(index.model());
    auto dirPath = model->filePath(index);
    QDesktopServices::openUrl(QUrl::fromLocalFile(dirPath));
}

void DirectoryViewWidget::handleRenameFileAction()
{
    auto selectedIndexes = m_directoryView->selectionModel()->selectedIndexes();

    if (selectedIndexes.isEmpty())
        return;

    auto index = selectedIndexes[0];
    auto model = static_cast<QFileSystemModel*>(m_directoryView->model());
    bool wasReadOnly = model->isReadOnly();

    auto oldName = model->fileName(index);
    
    for (auto& importantFile : importantFiles)
    {
        if (importantFile == oldName)
        {
            QMessageBox::critical(this,
                "Error",
                QString{"Important file \"%1\" cannot be renamed!"}.arg(oldName)
            );
            return;
        }
    }

    auto dirPath = fs::path{model->filePath(index).toStdString()};

    bool ok;

    auto newName = QInputDialog::getText(this,
        "Rename File",
        "Enter file name",
        QLineEdit::Normal,
        oldName,
        &ok
    );

    if (!ok)
        return;

    if (newName.isEmpty())
    {
        QMessageBox::critical(this,
            "Error",
            "Name cannot be empty!"
        );
        return;
    }

    if (newName == oldName)
    {
        QMessageBox::critical(this,
            "Error",
            "New name cannot be the same as old name!"
        );
        return;
    }

    auto finalPath = dirPath / ".." / newName.toStdString();

    if (fs::exists(finalPath) && fs::is_regular_file(finalPath))
    {
        QMessageBox::critical(this,
                "Error",
                QString{"File \"%1\" already exists!"}.arg(newName)
            );
            return;
    }

    model->setReadOnly(false);
    model->setData(index, newName);
    model->setReadOnly(wasReadOnly);
}

void DirectoryViewWidget::handleRemoveFileAction()
{
    auto selectedIndexes = m_directoryView->selectionModel()->selectedIndexes();

    if (selectedIndexes.isEmpty())
        return;

    auto index = selectedIndexes[0];
    auto model = static_cast<QFileSystemModel*>(m_directoryView->model());
    
    if (model->isDir(index))
        return;

    auto fileName = model->fileName(index);

    for (auto& importantFile : importantFiles)
    {
        if (importantFile == fileName)
        {
            QMessageBox::critical(this,
                "Error",
                QString{"Important file \"%1\" cannot be removed!"}.arg(fileName)
            );
            return;
        }
    }

    auto prompt = QMessageBox::question(this,
        "Question",
        QString{"Are you sure you want to remove \"%1\"?"}.arg(fileName),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (prompt == QMessageBox::Yes)
        model->remove(index);
}

void DirectoryViewWidget::handleVisibilityChange()
{
    emit updateToolbarToggleButtons();
}
