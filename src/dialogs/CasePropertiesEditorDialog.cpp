#include "dialogs/CasePropertiesEditorDialog.hpp"

#include <QMessageBox>


CasePropertiesEditorDialog::CasePropertiesEditorDialog(PyWrightGameInfo& selectedGameInfo, PyWrightCase* selectedCase, QWidget* parent)
    : QDialog(parent),
      m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel)),
      m_casePropertiesWidget(new CasePropertiesWidget(selectedGameInfo, selectedCase, this)),
      m_selectedGameInfo(selectedGameInfo), m_selectedCase(selectedCase)
{
    setFixedSize(QSize(380, 200));

    m_isANewCase = selectedCase == nullptr;

    if (m_isANewCase)
    {
        setWindowTitle("Add New Case");
        m_selectedCase = std::make_unique<PyWrightCase>();
    }
    else
        setWindowTitle("Case Properties: " + selectedCase->caseName);

    connectSignalsAndSlots();
    prepareMainLayout();
}

void CasePropertiesEditorDialog::connectSignalsAndSlots()
{
    connect(m_buttonBox, &QDialogButtonBox::accepted,
        this, &CasePropertiesEditorDialog::handleAccept);
    connect(m_buttonBox, &QDialogButtonBox::rejected,
        this, &QDialog::reject);
}

void CasePropertiesEditorDialog::prepareMainLayout()
{
    auto mainLayout = new QVBoxLayout();

    mainLayout->addWidget(m_casePropertiesWidget);
    mainLayout->addStretch();
    mainLayout->addWidget(m_buttonBox);

    setLayout(mainLayout);
}

PyWrightCase* CasePropertiesEditorDialog::getCase()
{
    return m_selectedCase.get();
}

void CasePropertiesEditorDialog::handleAccept()
{
    if (!m_casePropertiesWidget->checkInputValidity())
    {
        QMessageBox::critical(this, "Error", "One or more important fields are empty!");
        return;
    }

    if (m_isANewCase)
    {
        for (auto& case_ : m_selectedGameInfo.getGameCases())
        {
            if (m_casePropertiesWidget->getCaseNameFieldText().toLower() == case_.toLower())
            {
                QMessageBox::critical(this, "Error",
                     "A case with name " + case_ + " already exists!");
                return;
            }
        }
    }

    m_casePropertiesWidget->saveFieldsToCase();
    accept();
}
