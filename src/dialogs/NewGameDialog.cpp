#include "dialogs/NewGameDialog.hpp"
#include "data/PyWrightFolder.hpp"

#include <QBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

static const QString PYWRIGHT_URL_TEXT = "<a href=\"https://pywright.dawnsoft.org/\">(Download PyWright from its official website)</a>";

NewGameDialog::NewGameDialog(const fs::path& pyWrightRootPath, QWidget* parent)
    : QDialog(parent),
      m_pyWrightPathEdit(new QLineEdit()), m_folderNameEdit(new QLineEdit()),
      m_pyWrightLinkLabel(new QLabel(PYWRIGHT_URL_TEXT)),
      m_pyWrightPathBrowseButton(new QPushButton("...")),
      m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel)),
      m_gameDataWidget(new GameDataWidget(this))
{
    setWindowTitle("New PyWright Game");

    if (isValidPyWrightFolder(pyWrightRootPath))
        m_pyWrightPathEdit->setText(QString{pyWrightRootPath.c_str()});

    m_pyWrightPathEdit->setFixedWidth(255);
    m_folderNameEdit->setFixedWidth(250);

    m_pyWrightLinkLabel->setOpenExternalLinks(true);

    m_pyWrightPathBrowseButton->setFixedWidth(40);

    connectSignalsAndSlots();
    prepareMainLayout();
}

void NewGameDialog::connectSignalsAndSlots()
{
    connect(m_pyWrightPathBrowseButton, &QPushButton::clicked,
        this, &NewGameDialog::handleBrowseButtonClicked);
    connect(m_buttonBox, &QDialogButtonBox::accepted,
        this, &NewGameDialog::handleAccepted);
    connect(m_buttonBox, &QDialogButtonBox::rejected,
        this, &QDialog::reject);
}

void NewGameDialog::prepareMainLayout()
{
    auto mainLayout = new QVBoxLayout();
    auto pyWrightFolderPathBox = new QHBoxLayout();
    auto folderNameBox = new QHBoxLayout();

    pyWrightFolderPathBox->addWidget(new QLabel("PyWright Folder Path:"));
    pyWrightFolderPathBox->addStretch();
    pyWrightFolderPathBox->addWidget(m_pyWrightPathEdit);
    pyWrightFolderPathBox->addWidget(m_pyWrightPathBrowseButton);

    folderNameBox->addWidget(new QLabel("Game (Folder) Name:"));
    folderNameBox->addStretch();
    folderNameBox->addWidget(m_folderNameEdit);

    mainLayout->addLayout(pyWrightFolderPathBox);
    mainLayout->addWidget(m_pyWrightLinkLabel);
    mainLayout->addLayout(folderNameBox);
    mainLayout->addWidget(m_gameDataWidget);
    mainLayout->addWidget(m_buttonBox);

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(mainLayout);
}

std::optional<PyWrightGameInfo> NewGameDialog::getNewGame()
{
    return m_newGame;
}

void NewGameDialog::handleBrowseButtonClicked()
{
    auto picker = QFileDialog::getExistingDirectory(this);

    if (!picker.isEmpty())
    {
        auto pickerPath = fs::path{picker.toStdString()};

        if (isValidPyWrightFolder(pickerPath))
        {
            m_pyWrightPathEdit->setText(picker);
            m_gameDataWidget->setPyWrightPath(pickerPath);
        }
        else
        {
            QMessageBox::critical(this,
                "Error",
                "Selected folder is not a PyWright folder."
            );
        }
    }
}

void NewGameDialog::handleAccepted()
{
    auto rootPath = fs::path{m_pyWrightPathEdit->text().toStdString()};

    if (!isValidPyWrightFolder(rootPath))
    {
        QMessageBox::critical(this,
            "Error",
            "Selected folder is not a PyWright folder."
        );
        return;
    }

    auto folder = m_folderNameEdit->text();

    auto finalPath = rootPath / "games" / folder.toStdString();

    if (fs::exists(finalPath) && fs::is_directory(finalPath))
    {
        QMessageBox::critical(this, 
            "Error", 
            "A game named " + folder + " already exists!"
        );
        return;
    }
    
    auto title = m_gameDataWidget->getGameTitle();
    auto iconPath = m_gameDataWidget->getGameIconPath();
    auto author = m_gameDataWidget->getGameAuthor();
    auto version = m_gameDataWidget->getGameVersion();

    try
    {
        m_newGame = PyWrightGameInfo::createNewGame(rootPath, folder, version, title, author, iconPath);
        accept();
    }
    catch (std::exception& e)
    {
        QMessageBox::critical(this, 
            "Error",
            e.what()
        );
    }
    
}
