#include "widgets/MainWindowCentralWidget.hpp"
#include "widgets/FileEditWidget.hpp"

MainWindowCentralWidget::MainWindowCentralWidget(QWidget* parent)
    : QWidget(parent)
{
    m_tabWidget = new QTabWidget();
    m_tabWidget->setTabsClosable(true);

    connectSignalsAndSlots();

    auto main_layout = new QVBoxLayout();

    main_layout->addWidget(m_tabWidget);

    setLayout(main_layout);
}

void MainWindowCentralWidget::connectSignalsAndSlots()
{
    connect(m_tabWidget, &QTabWidget::tabCloseRequested,
        this, &MainWindowCentralWidget::handleRemoveTab);
    connect(m_tabWidget, &QTabWidget::currentChanged,
        this, &MainWindowCentralWidget::handleTabChanged);
}

void MainWindowCentralWidget::connectFileEditWidgetSignals(const FileEditWidget* widget)
{
    connect(widget, &FileEditWidget::cursorPositionChanged,
        this, &MainWindowCentralWidget::handleCursorPositionChanged);
    connect(widget, &FileEditWidget::fileNameChanged,
        this, &MainWindowCentralWidget::handleFileEditingTabNameChanged);
    connect(widget, &FileEditWidget::fileModifiedChanged,
        this, &MainWindowCentralWidget::updateSaveButtonAndCurrentTab);
}

void MainWindowCentralWidget::openNewTab(IDETabWidget* tabWidget, const QString& tabTitle)
{
    m_tabWidget->addTab(tabWidget, tabTitle);
    m_tabWidget->setMovable(m_tabWidget->count() > 1);
    m_tabWidget->setCurrentIndex(m_tabWidget->count() - 1);
}

void MainWindowCentralWidget::openNewEditingTab(const fs::path &filePath)
{
    // If there is a editing tab open for the given file, just switch to it instead
    for (int i = 0 ; i < m_tabWidget->count() ; i++)
    {
        if (!isFileEditingTab(i))
            continue;

        auto openedTab = static_cast<FileEditWidget*>(m_tabWidget->widget(i));

        if (openedTab->getFilePath() == filePath)
        {
            m_tabWidget->setCurrentIndex(i);
            return;
        }
    }

    auto fileEditWidget = new FileEditWidget(m_selectedGameInfo->getPyWrightFolderPath(), filePath);  

    fileEditWidget->supplyBuiltinMacrosToLexer(m_selectedGameInfo->getBuiltinMacros());
    fileEditWidget->supplyGameMacrosToLexer(m_selectedGameInfo->getGameMacros());

    fileEditWidget->supplyEditorColorThemeToLexer();

    QString fileName;

    if (!filePath.empty())
        fileName = QString{filePath.filename().c_str()};
    else
        fileName = "New File";

    // TODO: Signal connections...

    // Append folder name if two tabs with the same name are open
    for (int i = 0 ; i < m_tabWidget->count() ; i++)
    {
        if (!isFileEditingTab(i))
            continue;

        auto openedTab = static_cast<FileEditWidget*>(m_tabWidget->widget(i));

        if (openedTab->getFileName() == fileName)
        {
            fileName += " @ " + QString{fileEditWidget->getFilePath().parent_path().filename().c_str()};
            m_tabWidget->setTabText(i, openedTab->getFileName() + " @ " + QString{openedTab->getFilePath().parent_path().filename().c_str()});
        }
    }
    
    connectFileEditWidgetSignals(fileEditWidget);
    openNewTab(fileEditWidget, fileName);
}

void MainWindowCentralWidget::openGamePropertiesTab()
{
    for (int i = 0 ; i < m_tabWidget->count() ; i++)
    {
        if (isGamePropertiesTab(i))
        {
            m_tabWidget->setCurrentIndex(i);
            return;
        }
    }

    openNewTab(m_gamePropertiesWidget.get(), "Game Properties");
}

bool MainWindowCentralWidget::isFileEditingTab(int index)
{
    return static_cast<IDETabWidget*>(m_tabWidget->widget(index))->getTabKind() == TabKind::FileEditing;
}

bool MainWindowCentralWidget::isGamePropertiesTab(int index)
{
    return static_cast<IDETabWidget*>(m_tabWidget->widget(index))->getTabKind() == TabKind::GameProperties;
}

bool MainWindowCentralWidget::isImageViewerTab(int index)
{
    return static_cast<IDETabWidget*>(m_tabWidget->widget(index))->getTabKind() == TabKind::ImageViewer;
}

void MainWindowCentralWidget::undoInCurrentTab()
{
    if (isFileEditingTab(m_tabWidget->currentIndex()))
        static_cast<FileEditWidget*>(m_tabWidget->currentWidget())->doUndo();
}

void MainWindowCentralWidget::redoInCurrentTab()
{
    if (isFileEditingTab(m_tabWidget->currentIndex()))
        static_cast<FileEditWidget*>(m_tabWidget->currentWidget())->doRedo();
}

void MainWindowCentralWidget::setSelectedGame(PyWrightGameInfo& selectedGameInfo)
{
    m_selectedGameInfo = selectedGameInfo;
    m_gamePropertiesWidget = std::make_unique<GamePropertiesWidget>(selectedGameInfo, this);
    m_gamePropertiesWidget->loadGame(m_selectedGameInfo.value());
}

QMessageBox::StandardButton MainWindowCentralWidget::askForClosingTab(int index, bool multipleTabs)
{
    auto answers = QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;

    if (multipleTabs)
        answers |= QMessageBox::YesToAll | QMessageBox::NoToAll;

    auto tab = static_cast<FileEditWidget*>(m_tabWidget->widget(index));

    auto tabText = tab->getFileName();

    return QMessageBox::question(this,
        "Confirm Save",
        "Would you like to save " + tabText + "?",
        answers, QMessageBox::Cancel
    );
}

void MainWindowCentralWidget::updateLineAndCol()
{
    if (m_tabWidget->count() == 0)
        emit currentTabCursorPositionChanged(-1, -1);
    else if (isFileEditingTab(m_tabWidget->currentIndex()))
    {
        auto [line, col] = static_cast<FileEditWidget*>(m_tabWidget->currentWidget())->getCursorPosition();
        emit currentTabCursorPositionChanged(line + 1, col + 1);
    }
    else
        emit currentTabCursorPositionChanged(-1, -1);
}

void MainWindowCentralWidget::updateEditingTabInfo(int tabIndex, bool modified)
{
    if (isFileEditingTab(tabIndex))
    {
        auto fileEditWidget = static_cast<FileEditWidget*>(m_tabWidget->widget(tabIndex));

        if (tabIndex == m_tabWidget->currentIndex())
            emit updateSaveButtonRequested(modified);

        auto tabText = fileEditWidget->getFileName();
        // Prepend a * to the tab name if the file is modified
        m_tabWidget->setTabText(tabIndex, modified ? QString{'*'} + tabText : tabText);
        emit undoRedoInfoChanged(fileEditWidget->canUndo(), fileEditWidget->canRedo());
    }
}

void MainWindowCentralWidget::updateSaveButtonAndCurrentTab(bool modified)
{
    updateEditingTabInfo(m_tabWidget->currentIndex(), modified);
}

void MainWindowCentralWidget::handleFileEditingTabNameChanged(const QString &newName)
{
    m_tabWidget->setTabText(m_tabWidget->currentIndex(), newName);
}

void MainWindowCentralWidget::handleCursorPositionChanged(Scintilla::sptr_t line, Scintilla::sptr_t column)
{
    updateLineAndCol();
}

void MainWindowCentralWidget::handleTabChanged(int index)
{
    bool saveCondition = m_tabWidget->count() > 0
        && isFileEditingTab(index)
        && static_cast<FileEditWidget*>(m_tabWidget->widget(index))->isFileModified();

    bool undoCondition = isFileEditingTab(index)
        && static_cast<FileEditWidget*>(m_tabWidget->widget(index))->canUndo();

    bool redoCondition = isFileEditingTab(index)
        && static_cast<FileEditWidget*>(m_tabWidget->widget(index))->canRedo();

    
    updateLineAndCol();
    emit updateSaveButtonRequested(saveCondition);
    emit undoRedoInfoChanged(undoCondition, redoCondition);
}

void MainWindowCentralWidget::handleRemoveTab(int index)
{
    if (isFileEditingTab(index))
    {
        auto tab = static_cast<FileEditWidget*>(m_tabWidget->widget(index));

        if (tab->isFileModified())
        {
            auto result = askForClosingTab(index, false);

            if (result == QMessageBox::Cancel)
                return;
            // QMessageBox::No will just ignore the current file, so no need to handle it.
            else if (result == QMessageBox::Yes)
                tab->SaveToFile();
        }
    }

    m_tabWidget->removeTab(index);
    m_tabWidget->setMovable(m_tabWidget->count() > 1);
}
