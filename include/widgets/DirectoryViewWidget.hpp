#pragma once

#include <QTreeView>
#include <QDockWidget>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QModelIndex>

#include <optional>

#include "data/PyWrightGameInfo.hpp"


class DeselectableTreeView : public QTreeView
{
    public:
        void mousePressEvent(QMouseEvent* event) override;
};


class DirectoryViewWidget : public QDockWidget
{
    Q_OBJECT

    public:
        DirectoryViewWidget(QWidget* parent = nullptr);
        void updateDirectoryView(const PyWrightGameInfo& selectedGameInfo);
        void clearDirectoryView();

    signals:
        void openNewTab(const QString& path);
        void openGamePropertiesTab();
        void updateToolbarToggleButtons();
        void FileRenamedFromDirectoryView(const QString& oldName, const QString& newName);

    private:
        void connectSignalsAndSlots();
        void prepareMainLayout();
        void createCustomTitlebar();
        void createSubScriptFile(const QString& subScriptName);

        QWidget* m_mainWidget;
        QLabel* m_gameTitleLabel;
        QPushButton* m_gamePropertiesButton;
        DeselectableTreeView* m_directoryView;
        QWidget* m_topWidget;
        QWidget* m_dummyTitleBar;

        QAction* m_newCaseAction;
        QAction* m_newScriptAction;
        QAction* m_openDirAction;
        QAction* m_renameFileAction;
        QAction* m_removeFileAction;
        QAction* m_createExamineScriptAction;
        QAction* m_createMoveScriptAction;
        QAction* m_createPresentScriptAction;
        QAction* m_createTalkScriptAction;

        std::optional<PyWrightGameInfo> m_gameInfo;

    private slots:
        void handleTreeViewDoubleClicked(const QModelIndex& index);
        void handleGamePropertiesButtonClicked();
        void handleViewContextMenu(const QPoint& pos);
        void handleAddNewCaseAction();
        void handleAddNewScriptAction();
        void handleOpenDirAction();
        void handleRenameFileAction();
        void handleRemoveFileAction();
        void handleVisibilityChange();

};
