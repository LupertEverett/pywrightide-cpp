#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "widgets/GameDataWidget.hpp"
#include "data/PyWrightGameInfo.hpp"

#include <filesystem>
#include <optional>

namespace fs = std::filesystem;


class NewGameDialog : public QDialog
{
    Q_OBJECT

    public:
        NewGameDialog(const fs::path& pyWrightRootPath = "", QWidget* parent = nullptr);
        std::optional<PyWrightGameInfo> getNewGame();
    
    private:
        void connectSignalsAndSlots();
        void prepareMainLayout();

        QLineEdit* m_pyWrightPathEdit;
        QLineEdit* m_folderNameEdit;
        QLabel* m_pyWrightLinkLabel;
        QPushButton* m_pyWrightPathBrowseButton;
        QDialogButtonBox* m_buttonBox;
        GameDataWidget* m_gameDataWidget;
        std::optional<PyWrightGameInfo> m_newGame;

    private slots:
        void handleBrowseButtonClicked();
        void handleAccepted();
};
