#include "dialogs/AddExistingCaseDialog.hpp"

const static QStringList disallowedFolders = {
    "art", "sfx", "music", "movies"
};

AddExistingCaseDialog::AddExistingCaseDialog(PyWrightGameInfo& selectedGameInfo, QWidget* parent)
    : QDialog(parent), m_availableCasesListWidget(new QListWidget()),
      m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel)),
      m_selectedGameInfo(selectedGameInfo)
{
    setWindowTitle("Add Existing Case");

    m_okButton = m_buttonBox->button(QDialogButtonBox::Ok);
    updateOKButton();

    connectSignalsAndSlots();
    prepareMainLayout();

    populateCasesList();
}

void AddExistingCaseDialog::connectSignalsAndSlots()
{
    connect(m_availableCasesListWidget, &QListWidget::itemSelectionChanged,
        this, &AddExistingCaseDialog::updateOKButton);
    connect(m_buttonBox, &QDialogButtonBox::accepted,
        this, &AddExistingCaseDialog::handleAccept);
    connect(m_buttonBox, &QDialogButtonBox::rejected,
        this, &QDialog::reject);
}

void AddExistingCaseDialog::prepareMainLayout()
{
    auto mainLayout = new QVBoxLayout();

    mainLayout->addWidget(m_availableCasesListWidget);
    mainLayout->addWidget(m_buttonBox);

    setLayout(mainLayout);
}

void AddExistingCaseDialog::populateCasesList()
{
    auto p = m_selectedGameInfo.getGamePath();

    auto allCases = m_selectedGameInfo.getGameCases();

    QStringList folders;

    for (auto& dir_entry : fs::directory_iterator{p})
    {
        bool isDisallowedFolder = false;
        bool isAlreadyInCasesList = false;

        if (dir_entry.is_directory())
        {
            for (auto& folder : disallowedFolders)
            {
                if (dir_entry.path().stem() == folder.toStdString())
                {
                    isDisallowedFolder = true;
                    break;
                }
            }

            if (isDisallowedFolder)
                continue;

            for (auto& case_ : allCases)
            {
                if (case_.toStdString() == dir_entry.path().stem())
                {
                    isAlreadyInCasesList = true;
                    break;
                }
            }

            if (isAlreadyInCasesList)
                continue;

            if (isValidCase(dir_entry.path()))
                folders.append(QString{dir_entry.path().stem().c_str()});
        }
    }

    m_availableCasesListWidget->addItems(folders);
}

bool AddExistingCaseDialog::isValidCase(const fs::path &casePath)
{
    // Every valid case folder must include an intro.txt,
    // rather easily foolable but should be fine in almost all cases
    return fs::exists(casePath / "intro.txt") && fs::is_regular_file(casePath / "intro.txt");
}

void AddExistingCaseDialog::handleAccept()
{
    m_selectedGameInfo.appendCase(m_availableCasesListWidget->selectedItems()[0]->text());
    accept();
}

void AddExistingCaseDialog::updateOKButton()
{
    m_okButton->setEnabled(m_availableCasesListWidget->count() > 0);
}
