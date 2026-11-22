#include "widgets/GameDataWidget.hpp"
#include "dialogs/IconPickerDialog.hpp"

#include <QBoxLayout>

GameDataWidget::GameDataWidget(QWidget* parent)
    : QWidget(parent),
      m_gameAuthorLineEdit(new QLineEdit()),
      m_gameIconLineEdit(new QLineEdit()),
      m_gameTitleLineEdit(new QLineEdit()),
      m_gameVersionLineEdit(new QLineEdit()),
      m_gameIconPickerButton(new QPushButton("..."))
{
    m_gameIconPickerButton->setFixedWidth(40);

    connectSignalsAndSlots();
    prepareMainLayout();
}

void GameDataWidget::prepareMainLayout()
{
    /*
    auto titleLayout = new QHBoxLayout();
    auto versionLayout = new QHBoxLayout();
    auto authorLayout = new QHBoxLayout();
    auto iconLayout = new QHBoxLayout();

    titleLayout->addWidget(new QLabel("Game Title:"));
    titleLayout->addStretch();
    titleLayout->addWidget(m_gameTitleLineEdit);

    versionLayout->addWidget(new QLabel("Game Version:"));
    versionLayout->addStretch();
    versionLayout->addWidget(m_gameVersionLineEdit);

    authorLayout->addWidget(new QLabel("Author:"));
    authorLayout->addStretch();
    authorLayout->addWidget(m_gameAuthorLineEdit);

    iconLayout->addWidget(new QLabel("Game Icon:"));
    iconLayout->addStretch();
    iconLayout->addWidget(m_gameIconLineEdit);
    iconLayout->addWidget(m_gameIconPickerButton);

    auto m_mainLayout = new QVBoxLayout();

    m_mainLayout->addLayout(titleLayout);
    m_mainLayout->addLayout(versionLayout);
    m_mainLayout->addLayout(authorLayout);
    m_mainLayout->addLayout(iconLayout);

    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    */

    auto iconPickerLayout = new QHBoxLayout();
    iconPickerLayout->addWidget(m_gameIconLineEdit);
    iconPickerLayout->addWidget(m_gameIconPickerButton);

    auto mainLayout = new QGridLayout();

    mainLayout->addWidget(new QLabel("Game Title:"), 0, 0);
    mainLayout->addWidget(m_gameTitleLineEdit, 0, 1);
    mainLayout->addWidget(new QLabel("Game Version:"), 1, 0);
    mainLayout->addWidget(m_gameVersionLineEdit, 1, 1);
    mainLayout->addWidget(new QLabel("Author:"), 2, 0);
    mainLayout->addWidget(m_gameAuthorLineEdit, 2, 1);
    mainLayout->addWidget(new QLabel("Game Icon:"), 3, 0);
    mainLayout->addLayout(iconPickerLayout, 3, 1);
    /*
    mainLayout->addWidget(m_gameIconLineEdit, 3, 1);
    mainLayout->addWidget(m_gameIconPickerButton, 3, 2);
    */

    mainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(mainLayout);
}

void GameDataWidget::connectSignalsAndSlots()
{
    connect(m_gameTitleLineEdit, &QLineEdit::textChanged, 
        this, &GameDataWidget::handleDataTxtFieldsChange
    );
    connect(m_gameVersionLineEdit, &QLineEdit::textChanged, 
        this, &GameDataWidget::handleDataTxtFieldsChange
    );
    connect(m_gameAuthorLineEdit, &QLineEdit::textChanged, 
        this, &GameDataWidget::handleDataTxtFieldsChange
    );
    connect(m_gameIconLineEdit, &QLineEdit::textChanged, 
        this, &GameDataWidget::handleDataTxtFieldsChange
    );
    connect(m_gameIconPickerButton, &QPushButton::pressed,
        this, &GameDataWidget::handleIconPickerClicked
    );
}

void GameDataWidget::loadDataTxt(const PyWrightGameInfo& gameInfo)
{
    m_gameInfo = gameInfo;
    m_pyWrightPath = gameInfo.getPyWrightFolderPath();
    populateGameInfo();
}

void GameDataWidget::populateGameInfo()
{
    if (m_gameInfo.has_value())
    {
        auto gameData = m_gameInfo.value().getGameDataInfo();

        m_gameTitleLineEdit->setText(gameData.gameTitle);
        m_gameVersionLineEdit->setText(gameData.gameVersion);
        m_gameAuthorLineEdit->setText(gameData.gameAuthor);
        m_gameIconLineEdit->setText(QString{gameData.gameIconPath.c_str()});
    }
}

void GameDataWidget::saveDataTxt()
{
    if (m_gameInfo.has_value())
    {
        auto gameData = m_gameInfo.value().getGameDataInfo();
        
        gameData.gameTitle = m_gameTitleLineEdit->text();
        gameData.gameVersion = m_gameVersionLineEdit->text();
        gameData.gameAuthor = m_gameAuthorLineEdit->text();
        gameData.gameIconPath = fs::path{m_gameIconLineEdit->text().toStdString()};

        m_gameInfo.value().setGameDataInfo(gameData);
        m_gameInfo.value().writeDataTxt();

        handleDataTxtFieldsChange();
    }
}

void GameDataWidget::setGameIconPathTextField(const QString &newPath)
{
    m_gameIconLineEdit->setText(newPath);
    handleDataTxtFieldsChange();
}

QString GameDataWidget::getGameTitle() const
{
    return m_gameTitleLineEdit->text();
}

fs::path GameDataWidget::getGameIconPath() const
{
    return fs::path{m_gameIconLineEdit->text().toStdString()};
}

QString GameDataWidget::getGameVersion() const
{
    return m_gameVersionLineEdit->text();
}

QString GameDataWidget::getGameAuthor() const
{
    return m_gameAuthorLineEdit->text();
}

void GameDataWidget::setPyWrightPath(const fs::path &newPath)
{
    m_pyWrightPath = newPath;
}

bool GameDataWidget::areDataTxtAreasDifferent() const
{
    if (!m_gameInfo.has_value())
        return false;

    auto gameData = m_gameInfo.value().getGameDataInfo();

    return !areDataTxtAreasEmpty()
        && m_gameTitleLineEdit->text() != gameData.gameTitle
        && m_gameAuthorLineEdit->text() != gameData.gameAuthor
        && m_gameVersionLineEdit->text() != gameData.gameVersion
        && m_gameIconLineEdit->text() != gameData.gameIconPath.string();
}

bool GameDataWidget::areDataTxtAreasEmpty() const
{
    return m_gameTitleLineEdit->text().isEmpty()
        && m_gameAuthorLineEdit->text().isEmpty()
        && m_gameVersionLineEdit->text().isEmpty()
        && m_gameIconLineEdit->text().isEmpty();
}

void GameDataWidget::handleDataTxtFieldsChange()
{
    emit dataTxtFieldsChanged();
}

void GameDataWidget::handleIconPickerClicked()
{
    if (!m_pyWrightPath.empty())
    {
        auto iconPicker = new IconPickerDialog(m_pyWrightPath, m_gameInfo, this);

        if (iconPicker->exec())
            m_gameIconLineEdit->setText(iconPicker->getSelectedIcon());
    }
}
