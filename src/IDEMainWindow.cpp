#include "IDEMainWindow.hpp"
#include "data/PyWrightFolder.hpp"
#include "dialogs/NewGameDialog.hpp"
#include "dialogs/OpenGameDialog.hpp"

#include <QIcon>
#include <QFileDialog>
#include <QMessageBox>


IDEMainWindow::IDEMainWindow()
    : m_centralWidget(new MainWindowCentralWidget()),
      m_statusBar(new MainWindowStatusBar()),
      m_topToolbar(new MainWindowTopToolbar(this)),
      m_directoryView(new DirectoryViewWidget(this))
{
    setWindowTitle("PyWright IDE");
    setMinimumSize(720, 540);
    setWindowIcon(QIcon("res/icons/ideicon.png"));

    addToolBar(m_topToolbar);
    setCentralWidget(m_centralWidget);
    addDockWidget(Qt::LeftDockWidgetArea, m_directoryView);
    setStatusBar(m_statusBar);

    connectSignalsAndSlots();

    m_directoryView->updateToolbarToggleButtons();
}

bool IDEMainWindow::isDirectoryViewVisible() const
{
    return m_directoryView->isVisible();
}

bool IDEMainWindow::isAssetBrowserVisible() const
{
    // TODO: Update this once the asset browser is added
    return false;
}

bool IDEMainWindow::isLoggerViewVisible() const
{
    // TODO: Update this once the logger view is added
    return false;
}

void IDEMainWindow::connectSignalsAndSlots()
{
    connect(m_topToolbar, &MainWindowTopToolbar::newGameClicked,
        this, &IDEMainWindow::handleNewGameClicked);
    connect(m_topToolbar, &MainWindowTopToolbar::openGameClicked,
        this, &IDEMainWindow::handleOpenGameClicked);
    connect(m_topToolbar, &MainWindowTopToolbar::newFileClicked,
        this, &IDEMainWindow::handleNewFileClicked);
    connect(m_topToolbar, &MainWindowTopToolbar::openFileClicked,
        this, &IDEMainWindow::handleOpenFileClicked);
    connect(m_topToolbar, &MainWindowTopToolbar::saveFileClicked,
        this, &IDEMainWindow::handleSaveFileClicked);
    connect(m_topToolbar, &MainWindowTopToolbar::undoClicked,
        this, &IDEMainWindow::handleUndoClicked);
    connect(m_topToolbar, &MainWindowTopToolbar::redoClicked,
        this, &IDEMainWindow::handleRedoClicked);
    connect(m_topToolbar, &MainWindowTopToolbar::directoryViewToggled,
        this, &IDEMainWindow::handleDirectoryViewToggled);
    connect(m_directoryView, &DirectoryViewWidget::openNewTab,
        this, &IDEMainWindow::handleDirectoryViewOpenNewTab);
    connect(m_directoryView, &DirectoryViewWidget::updateToolbarToggleButtons,
        this, &IDEMainWindow::handleUpdateToolbarToggleButtons);
    connect(m_directoryView, &DirectoryViewWidget::openGamePropertiesTab,
        this, [this]() { m_centralWidget->openGamePropertiesTab(); });

    connect(m_centralWidget, &MainWindowCentralWidget::currentTabCursorPositionChanged,
        this, &IDEMainWindow::handleUpdateLineAndCol);
    connect(m_centralWidget, &MainWindowCentralWidget::updateSaveButtonRequested,
        this, &IDEMainWindow::handleSaveButtonChange);
    connect(m_centralWidget, &MainWindowCentralWidget::undoRedoInfoChanged,
        this, &IDEMainWindow::handleUndoRedoButtonsChange);
}

bool IDEMainWindow::attemptClosingUnsavedTabs()
{
    return true;
}

void IDEMainWindow::handleNewGameClicked()
{
    auto newGameDialog = new NewGameDialog(CurrentPyWrightGame::getCurrentGame().currentPyWrightFolderPath(), this);

    if (newGameDialog->exec())
    {
        pickGame(newGameDialog->getNewGame().value());
    }
}

void IDEMainWindow::handleOpenGameClicked()
{
    auto picker = QFileDialog::getExistingDirectory(this);

    if (!picker.isEmpty())
    {
        auto pickerPath = fs::path{picker.toStdString()};

        if (PyWrightGameInfo::isValidGameFolder(pickerPath))
        {
            if (attemptClosingUnsavedTabs())
            {
                pickGameFolderAndOpenGamePropertiesTab(pickerPath);
                m_topToolbar->updateRecentFoldersList();
            }
        }
        else if (isValidPyWrightFolder(pickerPath))
        {
            auto openGameDialog = new OpenGameDialog(pickerPath, this);

            if (openGameDialog->exec())
            {
                if (attemptClosingUnsavedTabs())
                {
                    auto gamePath = pickerPath / "games" / openGameDialog->getSelectedGamePath();
                    pickGameFolderAndOpenGamePropertiesTab(gamePath);
                    m_topToolbar->updateRecentFoldersList();
                }
            }
        }
        else if (isValidPyWrightFolder(pickerPath.parent_path()))
        {
            auto openGameDialog = new OpenGameDialog(pickerPath.parent_path(), this);

            if (openGameDialog->exec())
            {
                if (attemptClosingUnsavedTabs())
                {
                    auto gamePath = pickerPath.parent_path() / "games" / openGameDialog->getSelectedGamePath();
                    pickGameFolderAndOpenGamePropertiesTab(gamePath);
                    m_topToolbar->updateRecentFoldersList();
                }
            }
        }
        else
            QMessageBox::critical(this, "Error", "Invalid folder selected!");
    }
}

void IDEMainWindow::pickGame(const PyWrightGameInfo& newGame)
{

}

void IDEMainWindow::pickGameFolder(const fs::path& gamePath)
{
    // TODO: Implement
    CurrentPyWrightGame::getCurrentGame().setCurrentGame(PyWrightGameInfo::loadFromFolder(gamePath));
    m_currentGameInfo = PyWrightGameInfo::loadFromFolder(gamePath);
    m_centralWidget->setSelectedGame(m_currentGameInfo.value());
    m_statusBar->setPyWrightPathInfo(QString{m_currentGameInfo->getPyWrightFolderPath().c_str()});
    m_topToolbar->updatePyWrightExecutableStatusTip(pickPyWrightExecutable(gamePath));
    m_topToolbar->updateToolbarButtons(!m_currentGameInfo->getPyWrightFolderPath().empty(),
        !m_currentGameInfo->getGamePath().empty()
    );

    m_directoryView->updateDirectoryView(m_currentGameInfo.value());

    setWindowTitle("PyWright IDE - " + m_currentGameInfo->getGameDataInfo().gameTitle);
}

void IDEMainWindow::pickGameFolderAndOpenGamePropertiesTab(const fs::path& gamePath)
{
    pickGameFolder(gamePath);
    m_centralWidget->openGamePropertiesTab();
}

void IDEMainWindow::handleNewFileClicked()
{
    m_centralWidget->openNewEditingTab("");
}

void IDEMainWindow::handleOpenFileClicked()
{
    auto openDialog = QFileDialog::getOpenFileName(this, 
        "Open File",
        QString{m_currentGameInfo->getGamePath().c_str()},
        "Text or Macro Files (*.txt *.mcro)"
    );

    if (!openDialog.isEmpty())
        m_centralWidget->openNewEditingTab(fs::path{openDialog.toStdString()});
}

void IDEMainWindow::handleSaveFileClicked()
{

}

void IDEMainWindow::handleUndoClicked()
{
    m_centralWidget->undoInCurrentTab();
}

void IDEMainWindow::handleRedoClicked()
{
    m_centralWidget->redoInCurrentTab();
}

void IDEMainWindow::handleDirectoryViewToggled(bool status)
{
    m_directoryView->setVisible(status);
}

void IDEMainWindow::handleUpdateToolbarToggleButtons()
{
    m_topToolbar->updateToolbarToggleButtons();
}

void IDEMainWindow::handleDirectoryViewOpenNewTab(const QString& path)
{
    m_centralWidget->openNewEditingTab(fs::path{path.toStdString()});
}

void IDEMainWindow::handleUpdateLineAndCol(Scintilla::sptr_t line, Scintilla::sptr_t col)
{
    m_statusBar->setCursorPositionInfo(line, col);
}

void IDEMainWindow::handleSaveButtonChange(bool enable)
{
    m_topToolbar->updateSaveButton(enable);
}

void IDEMainWindow::handleUndoRedoButtonsChange(bool canUndo, bool canRedo)
{
    m_topToolbar->updateUndoRedoButtons(canUndo, canRedo);
}
