#include "widgets/GameIntroWidget.hpp"

#include <QMessageBox>
#include <QIcon>

#include "dialogs/AddExistingCaseDialog.hpp"
#include "dialogs/CasePropertiesEditorDialog.hpp"

#include "data/PyWrightCase.hpp"
#include "data/IconThemes.hpp"


GameIntroWidget::GameIntroWidget(PyWrightGameInfo& gameInfo, QWidget* parent)
    : QWidget(parent), m_gameCasesListWidget(new QListWidget()),
      m_widgetToolbar(new QToolBar()), m_gameInfo(gameInfo)
{
    createActions();
    connectSignalsAndSlots();
    prepareMainLayout();

    updateWidgetToolbarButtons();
}

void GameIntroWidget::createActions()
{
    auto addNewCaseIconPath = iconPathFromTheme(ICON_NAME_PLUS);
    auto addExistingCaseIconPath = iconPathFromTheme(ICON_NAME_DOUBLE_PLUS);
    auto removeCaseIconPath = iconPathFromTheme(ICON_NAME_MINUS);
    auto casePropertiesIconPath = iconPathFromTheme(ICON_NAME_SETTINGS);
    auto upArrowIconPath = iconPathFromTheme(ICON_NAME_UP_ARROW);
    auto downArrowIconPath = iconPathFromTheme(ICON_NAME_DOWN_ARROW);

    m_addNewCaseAction = new QAction(QIcon(addNewCaseIconPath), "New Case");
    m_addExistingCaseAction = new QAction(QIcon(addExistingCaseIconPath), "Add Existing Case");
    m_removeCaseAction = new QAction(QIcon(removeCaseIconPath), "Remove Case");
    m_casePropertiesAction = new QAction(QIcon(casePropertiesIconPath), "Case Properties");
    m_moveCaseUpAction = new QAction(QIcon(upArrowIconPath), "Move Case Up");
    m_moveCaseDownAction = new QAction(QIcon(downArrowIconPath), "Move Case Down");
}

void GameIntroWidget::connectSignalsAndSlots()
{
    connect(m_gameCasesListWidget, &QListWidget::clicked,
        this, &GameIntroWidget::handleListWidgetClicked);
    connect(m_addNewCaseAction, &QAction::triggered,
        this, &GameIntroWidget::handleAddNewCase);
    connect(m_addExistingCaseAction, &QAction::triggered,
        this, &GameIntroWidget::handleAddExistingCase);
    connect(m_removeCaseAction, &QAction::triggered,
        this, &GameIntroWidget::handleRemoveCase);
    connect(m_casePropertiesAction, &QAction::triggered,
        this, &GameIntroWidget::handleCaseProperties);
    connect(m_moveCaseUpAction, &QAction::triggered,
        this, &GameIntroWidget::handleCaseMoveUp);
    connect(m_moveCaseDownAction, &QAction::triggered,
        this, &GameIntroWidget::handleCaseMoveDown);
}

void GameIntroWidget::prepareMainLayout()
{
    m_widgetToolbar->addAction(m_addNewCaseAction);
    m_widgetToolbar->addAction(m_addExistingCaseAction);
    m_widgetToolbar->addSeparator();
    m_widgetToolbar->addAction(m_removeCaseAction);
    m_widgetToolbar->addSeparator();
    m_widgetToolbar->addAction(m_moveCaseUpAction);
    m_widgetToolbar->addAction(m_moveCaseDownAction);
    m_widgetToolbar->addSeparator();
    m_widgetToolbar->addAction(m_casePropertiesAction);

    auto mainLayout = new QVBoxLayout();

    mainLayout->addWidget(m_widgetToolbar);
    mainLayout->addWidget(m_gameCasesListWidget);

    mainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(mainLayout);
}

void GameIntroWidget::loadIntroTxt(const PyWrightGameInfo& selectedGameInfo)
{
    m_gameInfo = selectedGameInfo;
    populateCasesList();
}

void GameIntroWidget::saveIntroTxt()
{
    m_gameInfo.writeIntroTxt();
}

void GameIntroWidget::populateCasesList()
{
    m_gameCasesListWidget->clear();
    m_gameCasesListWidget->addItems(m_gameInfo.getGameCases());
}

void GameIntroWidget::updateWidgetToolbarButtons()
{
    auto numSelectedItems = m_gameCasesListWidget->selectedItems().size();

    m_removeCaseAction->setEnabled(numSelectedItems > 0);
    m_casePropertiesAction->setEnabled(numSelectedItems > 0);
    m_moveCaseUpAction->setEnabled(numSelectedItems > 0 && m_gameInfo.getGameCases().size() > 1);
    m_moveCaseDownAction->setEnabled(numSelectedItems > 0 && m_gameInfo.getGameCases().size() > 1);
}

void GameIntroWidget::handleListWidgetClicked()
{
    updateWidgetToolbarButtons();
}

void GameIntroWidget::handleAddNewCase()
{
    auto addNewCaseDialog = new CasePropertiesEditorDialog(m_gameInfo, nullptr, this);

    if (addNewCaseDialog->exec())
    {
        m_gameInfo.createNewCase(*addNewCaseDialog->getCase());
        m_gameInfo.updateIntroTxt();
        populateCasesList();
    }
}

void GameIntroWidget::handleAddExistingCase()
{
    auto addExistingCaseDialog = new AddExistingCaseDialog(m_gameInfo, this);

    if (addExistingCaseDialog->exec())
    {
        m_gameInfo.updateIntroTxt();
        populateCasesList();
    }
}

void GameIntroWidget::handleRemoveCase()
{
    auto answer = QMessageBox::question(this, 
        "Are you sure?",
        "Are you sure you want to remove this case?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (answer == QMessageBox::No)
        return;

    auto alsoRemoveFolder = QMessageBox::question(this, 
        "Also remove the folder?", 
        "Would you like to remove the folder as well?", 
        QMessageBox::Yes | QMessageBox::No,  
        QMessageBox::No
    );

    auto selectedCase = m_gameCasesListWidget->currentItem()->text();

    m_gameInfo.removeCase(selectedCase, alsoRemoveFolder == QMessageBox::Yes);
    populateCasesList();
}

void GameIntroWidget::handleCaseProperties()
{
    auto selectedCaseName = m_gameCasesListWidget->selectedItems()[0]->text();
    auto selectedCasePath = m_gameInfo.getGamePath() / selectedCaseName.toStdString();
    auto selectedCase = PyWrightCase::fromExistingCaseFolder(selectedCasePath);

    auto casePropertiesDialog = new CasePropertiesEditorDialog(m_gameInfo, &selectedCase, this);

    if (casePropertiesDialog->exec())
    {
        selectedCase.updateIntroTxt(selectedCasePath);
        populateCasesList();
    }
}

void GameIntroWidget::handleCaseMoveUp()
{
    // Stop if nothing selected
    if (!m_gameCasesListWidget->currentItem())
        return;

    auto currIndex = m_gameCasesListWidget->currentRow();

    if (currIndex == 0)
        return;

    m_gameInfo.moveCaseUp(currIndex);
    populateCasesList();
    m_gameCasesListWidget->setCurrentRow(currIndex - 1);
}

void GameIntroWidget::handleCaseMoveDown()
{
    // Stop if nothing selected
    if (!m_gameCasesListWidget->currentItem())
        return;

    auto currIndex = m_gameCasesListWidget->currentRow();

    if (currIndex == m_gameCasesListWidget->count() - 1)
        return;

    m_gameInfo.moveCaseDown(currIndex);
    populateCasesList();
    m_gameCasesListWidget->setCurrentRow(currIndex + 1);
}
