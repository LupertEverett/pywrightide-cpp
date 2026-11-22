#pragma once

#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <qtmetamacros.h>

class IDEMainWindow;

class MainWindowTopToolbar : public QToolBar
{
    Q_OBJECT

    public:
        MainWindowTopToolbar(IDEMainWindow* parent);

        void updatePyWrightExecutableStatusTip(const QString& executableName);
        void updateToolbarButtons(bool hasPyWright, bool hasPyWrightGame);
        void updateToolbarToggleButtons();
        void updateSaveButton(bool enable);
        void updateUndoRedoButtons(bool undoAvailable, bool redoAvailable);
        void updateRecentFoldersList();
        
    signals:
        void newGameClicked();
        void openGameClicked();
        void newFileClicked();
        void openFileClicked();
        void saveFileClicked();
        void undoClicked();
        void redoClicked();
        void directoryViewToggled(bool toggle);

    private:
        void createActions();
        void connectSignalsAndSlots();

        QAction* m_newPyWrightGameAction;
        QAction* m_openPyWrightGameAction;
        QAction* m_newFileAction;
        QAction* m_openFileAction;
        QAction* m_saveFileAction;
        QAction* m_undoAction;
        QAction* m_redoAction;
        QAction* m_findReplaceAction;
        QAction* m_directoryViewToggleAction;
        QAction* m_assetBrowserToggleAction;
        QAction* m_loggerViewToggleAction;
        QAction* m_characterViewerAction;
        QAction* m_runPyWrightAction;
        QAction* m_settingsAction;
        QAction* m_aboutAction;
};