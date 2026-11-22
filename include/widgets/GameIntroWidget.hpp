#pragma once

#include <QBoxLayout>
#include <QListWidget>
#include <QToolBar>
#include <QAction>

#include "data/PyWrightGameInfo.hpp"


class GameIntroWidget : public QWidget
{
    Q_OBJECT

    public:
        GameIntroWidget(PyWrightGameInfo& gameInfo, QWidget* parent = nullptr);
        void loadIntroTxt(const PyWrightGameInfo& selectedGameInfo);
        void saveIntroTxt();

    private:
        void createActions();
        void connectSignalsAndSlots();
        void prepareMainLayout();
        void populateCasesList();
        void updateWidgetToolbarButtons();

        QListWidget* m_gameCasesListWidget;
        QToolBar* m_widgetToolbar;
        QAction* m_addNewCaseAction;
        QAction* m_addExistingCaseAction;
        QAction* m_removeCaseAction;
        QAction* m_casePropertiesAction;
        QAction* m_moveCaseUpAction;
        QAction* m_moveCaseDownAction;

        PyWrightGameInfo& m_gameInfo;

    private slots:
        void handleListWidgetClicked();
        void handleAddNewCase();
        void handleAddExistingCase();
        void handleRemoveCase();
        void handleCaseProperties();
        void handleCaseMoveUp();
        void handleCaseMoveDown();
};
