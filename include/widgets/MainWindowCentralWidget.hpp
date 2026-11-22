#pragma once

#include <QTabWidget>
#include <QBoxLayout>
#include <QString>
#include <QMessageBox>

#include <optional>
#include <memory>

#include "widgets/IDETabWidget.hpp"
#include "widgets/FileEditWidget.hpp"
#include "widgets/GamePropertiesWidget.hpp"
#include "data/PyWrightGameInfo.hpp"

class MainWindowCentralWidget : public QWidget
{
    Q_OBJECT

    public:
        MainWindowCentralWidget(QWidget* parent = nullptr);

        void openNewTab(IDETabWidget* tabWidget, const QString& tabTitle);
        void openNewEditingTab(const fs::path& filePath);
        void openGamePropertiesTab();

        bool isFileEditingTab(int index);
        bool isGamePropertiesTab(int index);
        bool isImageViewerTab(int index);

        void undoInCurrentTab();
        void redoInCurrentTab();

        void setSelectedGame(PyWrightGameInfo& selectedGameInfo);

    signals:
        void currentTabCursorPositionChanged(Scintilla::sptr_t line, Scintilla::sptr_t column);
        void updateSaveButtonRequested(bool enable);
        void undoRedoInfoChanged(bool canUndo, bool canRedo);

    private:
        void connectSignalsAndSlots();
        void connectFileEditWidgetSignals(const FileEditWidget* widget);
        QMessageBox::StandardButton askForClosingTab(int index, bool multipleTabs);
        void updateLineAndCol();
        void updateEditingTabInfo(int tabIndex, bool modified);

        QTabWidget* m_tabWidget;
        std::unique_ptr<GamePropertiesWidget> m_gamePropertiesWidget;
        std::optional<PyWrightGameInfo> m_selectedGameInfo;

    private slots:
        void updateSaveButtonAndCurrentTab(bool modified);
        void handleFileEditingTabNameChanged(const QString& newName);
        void handleCursorPositionChanged(Scintilla::sptr_t line, Scintilla::sptr_t column);
        void handleTabChanged(int index);
        void handleRemoveTab(int index);
};