#include "widgets/MainWindowTopToolbar.hpp"

#include "IDEMainWindow.hpp"
#include "data/IconThemes.hpp"

#include <QIcon>
#include <QString>
#include <QKeySequence>

MainWindowTopToolbar::MainWindowTopToolbar(IDEMainWindow* parent)
    : QToolBar(parent)
{
    setWindowTitle("Main Toolbar");
    setObjectName("MainToolbar");
    setMovable(false);

    // We must not be able to disable this toolbar
    toggleViewAction()->setEnabled(false);
    toggleViewAction()->setVisible(false);

    createActions();
    connectSignalsAndSlots();
}

void MainWindowTopToolbar::updatePyWrightExecutableStatusTip(const QString& executableName)
{
    m_runPyWrightAction->setStatusTip("Run PyWright executable (" + executableName + ") [" + m_runPyWrightAction->shortcut().toString() + "]");
}

void MainWindowTopToolbar::updateToolbarButtons(bool hasPyWright, bool hasPyWrightGame)
{
    m_newPyWrightGameAction->setEnabled(hasPyWright);
    m_openPyWrightGameAction->setEnabled(hasPyWright);

    m_newFileAction->setEnabled(hasPyWrightGame);
    m_openFileAction->setEnabled(hasPyWrightGame);

    m_findReplaceAction->setEnabled(hasPyWrightGame);
    m_runPyWrightAction->setEnabled(hasPyWrightGame);

    updateToolbarToggleButtons();
}

void MainWindowTopToolbar::updateToolbarToggleButtons()
{
    auto mainWindow = static_cast<IDEMainWindow*>(parent());

    m_directoryViewToggleAction->setChecked(mainWindow->isDirectoryViewVisible());
    m_assetBrowserToggleAction->setChecked(mainWindow->isAssetBrowserVisible());
    m_loggerViewToggleAction->setChecked(mainWindow->isLoggerViewVisible());
}

void MainWindowTopToolbar::updateSaveButton(bool enable)
{
    m_saveFileAction->setEnabled(enable);
}

void MainWindowTopToolbar::updateUndoRedoButtons(bool undoAvailable, bool redoAvailable)
{
    m_undoAction->setEnabled(undoAvailable);
    m_redoAction->setEnabled(redoAvailable);
}

void MainWindowTopToolbar::updateRecentFoldersList()
{
    // TODO: Implement
}

void MainWindowTopToolbar::createActions()
{
    auto newPyWrightIconPath = iconPathFromTheme(ICON_NAME_NEW_GAME);
    m_newPyWrightGameAction = new QAction(QIcon(newPyWrightIconPath), QString("New PyWright Game"));
    m_newPyWrightGameAction->setShortcut(QKeySequence("Ctrl+n"));
    m_newPyWrightGameAction->setStatusTip("Create a new PyWright Game [" + m_newPyWrightGameAction->shortcut().toString() + "]");

    auto openPyWrightIconPath = iconPathFromTheme(ICON_NAME_OPEN_GAME);
    m_openPyWrightGameAction = new QAction(QIcon(openPyWrightIconPath), QString("Open PyWright Game"));
    m_openPyWrightGameAction->setShortcut(QKeySequence("Ctrl+Shift+o"));
    m_openPyWrightGameAction->setStatusTip("Open an existing PyWright Game [" + m_openPyWrightGameAction->shortcut().toString() + "]");

    auto newFileIconPath = iconPathFromTheme(ICON_NAME_NEW_FILE);
    m_newFileAction = new QAction(QIcon(newFileIconPath), QString("New File"));
    m_newFileAction->setEnabled(false);
    m_newFileAction->setShortcut(QKeySequence("Ctrl+t"));
    m_newFileAction->setStatusTip("Create a new File [" + m_newFileAction->shortcut().toString() + "]");

    auto openFileIconPath = iconPathFromTheme(ICON_NAME_OPEN_FILE);
    m_openFileAction = new QAction(QIcon(openFileIconPath), QString("Open File"));
    m_openFileAction->setEnabled(false);
    m_openFileAction->setShortcut(QKeySequence("Ctrl+o"));
    m_openFileAction->setStatusTip("Open an existing File [" + m_openFileAction->shortcut().toString() + "]");

    auto saveFileIconPath = iconPathFromTheme(ICON_NAME_SAVE_FILE);
    m_saveFileAction = new QAction(QIcon(saveFileIconPath), QString("Save File"));
    m_saveFileAction->setEnabled(false);
    m_saveFileAction->setShortcut(QKeySequence("Ctrl+s"));
    m_saveFileAction->setStatusTip("Save the currently open File [" + m_saveFileAction->shortcut().toString() + "]");

    auto undoIconPath = iconPathFromTheme(ICON_NAME_UNDO);
    m_undoAction = new QAction(QIcon(undoIconPath), QString("Undo"));
    m_undoAction->setEnabled(false);
    m_undoAction->setShortcut(QKeySequence("Ctrl+z"));
    m_undoAction->setStatusTip("Undo the last action [" + m_undoAction->shortcut().toString() + "]");

    auto redoIconPath = iconPathFromTheme(ICON_NAME_REDO);
    m_redoAction = new QAction(QIcon(redoIconPath), QString("Redo"));
    m_redoAction->setEnabled(false);
    m_redoAction->setShortcut(QKeySequence("Ctrl+y"));
    m_redoAction->setStatusTip("Redo the previously undoed action [" + m_redoAction->shortcut().toString() + "]");

    auto findreplaceIconPath = iconPathFromTheme(ICON_NAME_FIND_REPLACE);
    m_findReplaceAction = new QAction(QIcon(findreplaceIconPath), QString("Find/Replace"));
    m_findReplaceAction->setEnabled(false);
    m_findReplaceAction->setShortcut(QKeySequence("Ctrl+f"));
    m_findReplaceAction->setStatusTip("Find/Replace words [" + m_findReplaceAction->shortcut().toString() + "]");

    auto directoryToggleIconPath = iconPathFromTheme(ICON_NAME_DIRECTORY_VIEW_TOGGLE);
    m_directoryViewToggleAction = new QAction(QIcon(directoryToggleIconPath), QString("Toggle Directory View"));
    m_directoryViewToggleAction->setCheckable(true);
    m_directoryViewToggleAction->setStatusTip("Toggle Directory View ON or OFF");

    auto assetBrowserIconPath = iconPathFromTheme(ICON_NAME_ASSET_BROWSER_TOGGLE);
    m_assetBrowserToggleAction = new QAction(QIcon(assetBrowserIconPath), QString("Toggle Asset Browser"));
    m_assetBrowserToggleAction->setEnabled(false);
    m_assetBrowserToggleAction->setCheckable(true);
    m_assetBrowserToggleAction->setStatusTip("Toggle Asset Browser ON or OFF");

    auto loggerIconPath = iconPathFromTheme(ICON_NAME_LOGGER_TOGGLE);
    m_loggerViewToggleAction = new QAction(QIcon(loggerIconPath), QString("Toggle Logger View"));
    m_loggerViewToggleAction->setEnabled(false);
    m_loggerViewToggleAction->setCheckable(true);
    m_loggerViewToggleAction->setStatusTip("Toggle Logger View ON or OFF");

    auto characterViewerIconPath = iconPathFromTheme(ICON_NAME_CHARACTER_VIEWER);
    m_characterViewerAction = new QAction(QIcon(characterViewerIconPath), QString("Character Viewer"));
    m_characterViewerAction->setEnabled(false);
    m_characterViewerAction->setStatusTip("Open Character Viewer tool");

    auto runPyWrightIconPath = iconPathFromTheme(ICON_NAME_RUN_PYWRIGHT);
    m_runPyWrightAction = new QAction(QIcon(runPyWrightIconPath), QString("Run PyWright"));
    m_runPyWrightAction->setEnabled(false);
    m_runPyWrightAction->setShortcut(QKeySequence("Ctrl+r"));
    m_runPyWrightAction->setStatusTip("Run PyWright executable (None found) [" + m_runPyWrightAction->shortcut().toString() + "]");

    auto settingsIconPath = iconPathFromTheme(ICON_NAME_SETTINGS);
    m_settingsAction = new QAction(QIcon(settingsIconPath), QString("Settings"));
    m_settingsAction->setEnabled(false);
    m_settingsAction->setStatusTip("Open Settings");

    auto aboutIconPath = iconPathFromTheme(ICON_NAME_ABOUT);
    m_aboutAction = new QAction(QIcon(aboutIconPath), QString("About"));
    m_aboutAction->setEnabled(false);
    m_aboutAction->setStatusTip("About PyWright IDE");

    addAction(m_newPyWrightGameAction);
    addAction(m_openPyWrightGameAction);
    addSeparator();
    addAction(m_newFileAction);
    addAction(m_openFileAction);
    addAction(m_saveFileAction);
    addSeparator();
    addAction(m_undoAction);
    addAction(m_redoAction);
    addSeparator();
    addAction(m_findReplaceAction);
    addSeparator();
    addAction(m_directoryViewToggleAction);
    addAction(m_assetBrowserToggleAction);
    addAction(m_loggerViewToggleAction);
    addSeparator();
    addAction(m_characterViewerAction);
    addSeparator();
    addAction(m_runPyWrightAction);
    addSeparator();
    addAction(m_settingsAction);
    addAction(m_aboutAction);
}

void MainWindowTopToolbar::connectSignalsAndSlots()
{
    connect(m_newPyWrightGameAction, &QAction::triggered,
        this, [this]() { emit newGameClicked(); });
    connect(m_openPyWrightGameAction, &QAction::triggered,
        this, [this]() { emit openGameClicked(); });
    connect(m_newFileAction, &QAction::triggered,
        this, [this]() { emit newFileClicked(); });
    connect(m_openFileAction, &QAction::triggered,
        this, [this]() { emit openFileClicked(); });
    connect(m_saveFileAction, &QAction::triggered,
        this, [this]() { emit saveFileClicked(); });
    connect(m_undoAction, &QAction::triggered,
        this, [this]() { emit undoClicked(); });
    connect(m_redoAction, &QAction::triggered,
        this, [this]() { emit redoClicked(); });
    connect(m_directoryViewToggleAction, &QAction::triggered,
        this, [this]() { emit directoryViewToggled(m_directoryViewToggleAction->isChecked()); });
}
