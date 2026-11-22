#include "widgets/CasePropertiesWidget.hpp"
#include "dialogs/IconPickerDialog.hpp"

CasePropertiesWidget::CasePropertiesWidget(PyWrightGameInfo& currentGameInfo, PyWrightCase* selectedCase, QWidget* parent)
    : m_caseNameLineEdit(new QLineEdit()), m_caseInitialScriptLineEdit(new QLineEdit()),
      m_caseTextboxWrapCheckbox(new QCheckBox()),
      m_caseBackgroundNameLineEdit(new QLineEdit()), m_caseTextboxLinesSpinbox(new QSpinBox()),
      m_caseBGIconPickerButton(new QPushButton("...")),
      m_gameInfo(currentGameInfo),
      m_selectedCase(selectedCase ? selectedCase : new PyWrightCase())
{
    m_caseNameLineEdit->setEnabled(m_selectedCase->caseName.isEmpty());

    m_caseTextboxLinesSpinbox->setMinimum(2);
    m_caseTextboxLinesSpinbox->setMaximum(30);

    m_caseBackgroundNameLineEdit->setEnabled(false);

    m_caseBGIconPickerButton->setFixedWidth(30);

    fillFieldsFromCase();

    setupWhatsThisInfo();
    connectSignalsAndSlots();
    prepareMainLayout();
}

void CasePropertiesWidget::connectSignalsAndSlots()
{
    connect(m_caseBGIconPickerButton, &QPushButton::clicked,
        this, &CasePropertiesWidget::handleCaseBGPickerButtonClicked);
}

void CasePropertiesWidget::prepareMainLayout()
{
    auto mainLayout = new QVBoxLayout();

    auto caseNameBox = new QHBoxLayout();
    caseNameBox->addWidget(new QLabel("Case Name:"));
    caseNameBox->addStretch();
    caseNameBox->addWidget(m_caseNameLineEdit);

    auto initialScriptNameBox = new QHBoxLayout();
    initialScriptNameBox->addWidget(new QLabel("Initial Script:"));
    initialScriptNameBox->addStretch();
    initialScriptNameBox->addWidget(m_caseInitialScriptLineEdit);

    auto textboxWrapBox = new QHBoxLayout();
    textboxWrapBox->addWidget(new QLabel("Textbox (Auto)Wrap:"));
    textboxWrapBox->addStretch();
    textboxWrapBox->addWidget(m_caseTextboxWrapCheckbox);

    auto textboxLinesBox = new QHBoxLayout();
    textboxLinesBox->addWidget(new QLabel("Textbox Lines:"));
    textboxLinesBox->addStretch();
    textboxLinesBox->addWidget(m_caseTextboxLinesSpinbox);

    auto backgroundNameBox = new QHBoxLayout();
    backgroundNameBox->addWidget(new QLabel("Case Background Image:"));
    backgroundNameBox->addStretch();
    backgroundNameBox->addWidget(m_caseBackgroundNameLineEdit);
    backgroundNameBox->addWidget(m_caseBGIconPickerButton);

    mainLayout->addLayout(caseNameBox);
    mainLayout->addLayout(initialScriptNameBox);
    mainLayout->addLayout(textboxWrapBox);
    mainLayout->addLayout(textboxLinesBox);
    mainLayout->addLayout(backgroundNameBox);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(mainLayout);
}

void CasePropertiesWidget::setupWhatsThisInfo()
{
    m_caseNameLineEdit->setWhatsThis("This will be the name of your case. It will appear in "
                                    "the case selection menu, and it will be the name of the folder "
                                    "that\'s going to be created."
    );
    m_caseInitialScriptLineEdit->setWhatsThis("Indicates the name of the script that\'ll be run "
                                              "when starting this case from the beginning."
    );
    m_caseTextboxWrapCheckbox->setWhatsThis("Check this to enable auto wrapping of the text in textbox.\n"
                                            "If unchecked, {n} must be used to switch to new line."
    );
    m_caseTextboxLinesSpinbox->setWhatsThis("Indicates how many lines of text the textbox will show.\n"
                                            "Default is 3 lines (or 2 lines for the Japanese version)."
    );
    m_caseBackgroundNameLineEdit->setWhatsThis("Name of the file the case will use as background.");
}

bool CasePropertiesWidget::checkInputValidity()
{
    return !m_caseNameLineEdit->text().isEmpty()
        && !m_caseInitialScriptLineEdit->text().isEmpty();
}

QString CasePropertiesWidget::getCaseNameFieldText()
{
    return m_caseNameLineEdit->text();
}

void CasePropertiesWidget::saveFieldsToCase()
{
    m_selectedCase->caseName = m_caseNameLineEdit->text();
    m_selectedCase->initialScriptName = m_caseInitialScriptLineEdit->text();
    m_selectedCase->textboxWrap = m_caseTextboxWrapCheckbox->isChecked();
    m_selectedCase->textboxLines = m_caseTextboxLinesSpinbox->value();
    m_selectedCase->caseBackgroundName = m_caseBackgroundNameLineEdit->text();
}

void CasePropertiesWidget::fillFieldsFromCase()
{
    m_caseNameLineEdit->setText(m_selectedCase->caseName);
    m_caseInitialScriptLineEdit->setText(m_selectedCase->initialScriptName);
    m_caseTextboxWrapCheckbox->setChecked(m_selectedCase->textboxWrap);
    m_caseTextboxLinesSpinbox->setValue(m_selectedCase->textboxLines);
    m_caseBackgroundNameLineEdit->setText(m_selectedCase->caseBackgroundName);
}

void CasePropertiesWidget::handleCaseBGPickerButtonClicked()
{
    auto iconPicker = new IconPickerDialog(m_gameInfo.getPyWrightFolderPath(), m_gameInfo, this, {"bg"});

    if (iconPicker->exec())
    {
        auto iconName = iconPicker->getSelectedIcon();
        m_caseBackgroundNameLineEdit->setText(iconName);
    }
}
