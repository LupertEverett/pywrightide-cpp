#include "widgets/GamePropertiesWidget.hpp"

#include <QBoxLayout>

GamePropertiesWidget::GamePropertiesWidget(PyWrightGameInfo& selectedGameInfo, QWidget* parent)
    : IDETabWidget(TabKind::GameProperties, parent),
      m_gameDataWidget(new GameDataWidget(this)),
      m_gameIntroWidget(new GameIntroWidget(selectedGameInfo, this)),
      m_saveDataButton(new QPushButton("Save")),
      m_revertDataButton(new QPushButton("Revert")),
      m_dataTxtGroup(new QGroupBox("Game Info")),
      m_introTxtGroup(new QGroupBox("Available Cases")),
      m_gameInfo(selectedGameInfo)
{
    connectSignalsAndSlots();
    prepareMainLayout();

    handleDataTxtFieldsChange();
}

void GamePropertiesWidget::connectSignalsAndSlots()
{
    connect(m_gameDataWidget, &GameDataWidget::dataTxtFieldsChanged,
        this, &GamePropertiesWidget::handleDataTxtFieldsChange);
    connect(m_saveDataButton, &QPushButton::clicked,
        this, [this]() { m_gameDataWidget->saveDataTxt(); });
    connect(m_revertDataButton, &QPushButton::clicked,
        this, [this]() { m_gameDataWidget->populateGameInfo(); });
}

void GamePropertiesWidget::prepareMainLayout()
{
    auto dataTxtGroupLayout = new QVBoxLayout();
    auto introTxtGroupLayout = new QVBoxLayout();
    auto dataButtonsLayout = new QHBoxLayout();
    auto mainLayout = new QVBoxLayout();

    dataButtonsLayout->addStretch();
    dataButtonsLayout->addWidget(m_saveDataButton);
    dataButtonsLayout->addWidget(m_revertDataButton);

    dataTxtGroupLayout->addWidget(m_gameDataWidget);
    dataTxtGroupLayout->addLayout(dataButtonsLayout);

    introTxtGroupLayout->addWidget(m_gameIntroWidget);

    m_dataTxtGroup->setLayout(dataTxtGroupLayout);
    m_introTxtGroup->setLayout(introTxtGroupLayout);

    mainLayout->addWidget(m_dataTxtGroup);
    mainLayout->addWidget(m_introTxtGroup);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void GamePropertiesWidget::loadGame(PyWrightGameInfo& selectedGameInfo)
{
    m_gameInfo = selectedGameInfo;
    m_gameDataWidget->loadDataTxt(m_gameInfo);
    m_gameIntroWidget->loadIntroTxt(m_gameInfo);
}

void GamePropertiesWidget::saveGame()
{
    m_gameDataWidget->saveDataTxt();
    m_gameIntroWidget->saveIntroTxt();
}

void GamePropertiesWidget::setGameIconPath(const QString& newPath)
{
    m_gameDataWidget->setGameIconPathTextField(newPath);
}

void GamePropertiesWidget::handleDataTxtFieldsChange()
{
    bool validToChange = m_gameDataWidget->areDataTxtAreasDifferent();

    m_saveDataButton->setEnabled(validToChange);
    m_revertDataButton->setEnabled(validToChange);
}
