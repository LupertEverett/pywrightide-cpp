#pragma once

#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>

#include "data/PyWrightGameInfo.hpp"
#include "data/PyWrightCase.hpp"


class CasePropertiesWidget : public QWidget
{
    Q_OBJECT

    public:
        CasePropertiesWidget(PyWrightGameInfo& currentGameInfo, PyWrightCase* selectedCase = nullptr, QWidget* parent = nullptr);
        bool checkInputValidity();
        QString getCaseNameFieldText();
        void saveFieldsToCase();

    private:
        void connectSignalsAndSlots();
        void prepareMainLayout();
        void setupWhatsThisInfo();
        void fillFieldsFromCase();

        QLineEdit* m_caseNameLineEdit;
        QLineEdit* m_caseInitialScriptLineEdit;
        QCheckBox* m_caseTextboxWrapCheckbox;
        QLineEdit* m_caseBackgroundNameLineEdit;
        QSpinBox* m_caseTextboxLinesSpinbox;
        QPushButton* m_caseBGIconPickerButton;

        PyWrightGameInfo& m_gameInfo;
        std::unique_ptr<PyWrightCase> m_selectedCase;

    private slots:
        void handleCaseBGPickerButtonClicked();
};