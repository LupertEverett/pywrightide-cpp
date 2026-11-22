#pragma once

#include <QMainWindow>
#include <QBoxLayout>
#include "widgets/MainWindowCentralWidget.hpp"
#include "widgets/MainWindowStatusBar.hpp"
#include "widgets/MainWindowTopToolbar.hpp"
#include "widgets/DirectoryViewWidget.hpp"

#include "data/PyWrightGameInfo.hpp"

#include <filesystem>

namespace fs = std::filesystem;


class IDEMainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        IDEMainWindow();

        bool isDirectoryViewVisible() const;
        bool isAssetBrowserVisible() const;
        bool isLoggerViewVisible() const;

    private:
        void connectSignalsAndSlots();
        bool attemptClosingUnsavedTabs();
        void pickGame(const PyWrightGameInfo& newGame);
        void pickGameFolder(const fs::path& gamePath);
        void pickGameFolderAndOpenGamePropertiesTab(const fs::path& gamePath);

        MainWindowCentralWidget* m_centralWidget;
        MainWindowStatusBar* m_statusBar;
        MainWindowTopToolbar* m_topToolbar;
        DirectoryViewWidget* m_directoryView;
        std::optional<PyWrightGameInfo> m_currentGameInfo;

    private slots:
        void handleNewGameClicked();
        void handleOpenGameClicked();
        void handleNewFileClicked();
        void handleOpenFileClicked();
        void handleSaveFileClicked();
        void handleUndoClicked();
        void handleRedoClicked();
        void handleDirectoryViewToggled(bool status);
        void handleUpdateToolbarToggleButtons();
        void handleDirectoryViewOpenNewTab(const QString& path);

        void handleUpdateLineAndCol(Scintilla::sptr_t line, Scintilla::sptr_t col);
        void handleSaveButtonChange(bool enable);
        void handleUndoRedoButtonsChange(bool canUndo, bool canRedo);
};
