#pragma once

#include <QGroupBox>
#include <QPushButton>

#include "IDETabWidget.hpp"
#include "widgets/GameDataWidget.hpp"
#include "widgets/GameIntroWidget.hpp"

#include "data/PyWrightGameInfo.hpp"

class GamePropertiesWidget : public IDETabWidget
{
    Q_OBJECT

    public:
        GamePropertiesWidget(PyWrightGameInfo& selectedGameInfo, QWidget* parent = nullptr);
        void loadGame(PyWrightGameInfo& selectedGameInfo);
        void saveGame();
        void setGameIconPath(const QString& newPath);

    private:
        void connectSignalsAndSlots();
        void prepareMainLayout();

        GameDataWidget* m_gameDataWidget;
        GameIntroWidget* m_gameIntroWidget;
        QPushButton* m_saveDataButton;
        QPushButton* m_revertDataButton;
        QGroupBox* m_dataTxtGroup;
        QGroupBox* m_introTxtGroup;
        PyWrightGameInfo& m_gameInfo;

    private slots:
        void handleDataTxtFieldsChange();
};
