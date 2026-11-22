#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QBoxLayout>

#include "widgets/CasePropertiesWidget.hpp"

#include "data/PyWrightGameInfo.hpp"
#include "data/PyWrightCase.hpp"

#include <memory>


class CasePropertiesEditorDialog : public QDialog
{
    Q_OBJECT

    public:
        CasePropertiesEditorDialog(PyWrightGameInfo& selectedGameInfo, PyWrightCase* selectedCase, QWidget* parent = nullptr);
        PyWrightCase* getCase();

    private:
        void connectSignalsAndSlots();
        void prepareMainLayout();

        QDialogButtonBox* m_buttonBox;

        CasePropertiesWidget* m_casePropertiesWidget;
        PyWrightGameInfo& m_selectedGameInfo;
        std::unique_ptr<PyWrightCase> m_selectedCase;
        bool m_isANewCase;

    private slots:
        void handleAccept();
};
