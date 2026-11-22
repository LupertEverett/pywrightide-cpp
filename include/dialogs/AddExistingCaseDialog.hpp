#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QBoxLayout>
#include <QPushButton>

#include <filesystem>

#include "data/PyWrightGameInfo.hpp"

namespace fs = std::filesystem;


class AddExistingCaseDialog : public QDialog
{
    Q_OBJECT

    public:
        AddExistingCaseDialog(PyWrightGameInfo& selectedGameInfo, QWidget* parent = nullptr);
        
    private:
        void connectSignalsAndSlots();
        void prepareMainLayout();
        void populateCasesList();
        bool isValidCase(const fs::path& casePath);

        QListWidget* m_availableCasesListWidget;
        QDialogButtonBox* m_buttonBox;
        QPushButton* m_okButton;
        PyWrightGameInfo& m_selectedGameInfo;

    private slots:
        void handleAccept();
        void updateOKButton();
};
