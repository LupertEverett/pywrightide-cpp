#include "dialogs/IconPickerDialog.hpp"
#include "data/PyWrightGameInfo.hpp"
#include "data/ThumbnailIconProvider.hpp"

#include <QFileSystemModel>
#include <QDir>
#include <QMessageBox>
#include <stdexcept>

IconPickerDialog::IconPickerDialog(const fs::path& pyWrightRootPath, std::optional<PyWrightGameInfo> selectedGameInfo, QWidget* parent, const QStringList& limitToFolders)
    : QDialog(parent),
      m_iconsListView(new QListView()),
      m_subfolderCombobox(new QComboBox()),
      m_globalFolderCheckbox(new QCheckBox("Check the global art folder?")),
      m_limitToFolders(limitToFolders),
      m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel)),
      m_artSubfolders({}), m_selectedIcon(""),
      m_currentGame(selectedGameInfo),
      m_pyWrightPath(pyWrightRootPath)
{
    setWindowTitle("Pick an Icon");

    bool enabledCondition = m_currentGame.has_value() && fs::exists(m_currentGame.value().getGamePath() / "art");

    m_globalFolderCheckbox->setChecked(true);
    m_globalFolderCheckbox->setEnabled(enabledCondition);
    m_globalFolderCheckbox->setWhatsThis(
        "If checked, the icon picker will query the jpg and png files in the "
        "art folder of PyWright installation, otherwise it will "
        "query the selected game's art folder instead, if it exists."
    );

    m_iconsListView->setViewMode(QListView::IconMode);
    m_iconsListView->setResizeMode(QListView::Adjust);
    m_iconsListView->setUniformItemSizes(true);
    m_iconsListView->setSpacing(5);

    connectSignalsAndSlots();
    prepareMainLayout();

    refreshSubfolders();
}

void IconPickerDialog::connectSignalsAndSlots()
{
    connect(m_subfolderCombobox, &QComboBox::currentIndexChanged,
        this, &IconPickerDialog::refreshIconView);
    connect(m_globalFolderCheckbox, &QCheckBox::clicked,
        this, &IconPickerDialog::refreshSubfolders);
    connect(m_iconsListView, &QListView::doubleClicked,
        this, &IconPickerDialog::handleAccept);
    connect(m_buttonBox, &QDialogButtonBox::accepted,
        this, &IconPickerDialog::handleAccept);
    connect(m_buttonBox, &QDialogButtonBox::rejected,
        this, &QDialog::reject);
}

void IconPickerDialog::prepareMainLayout()
{
    auto mainLayout = new QVBoxLayout();

    auto topBar = new QHBoxLayout();

    topBar->addWidget(m_subfolderCombobox);
    topBar->addStretch();
    topBar->addWidget(m_globalFolderCheckbox);

    mainLayout->addLayout(topBar);
    mainLayout->addWidget(m_iconsListView);
    mainLayout->addWidget(m_buttonBox);

    setLayout(mainLayout);
}

QString IconPickerDialog::getSelectedIcon()
{
    return m_selectedIcon;
}

QStringList IconPickerDialog::querySubfolders()
{
    bool checkingRootArt = m_globalFolderCheckbox->isChecked();

    auto selectedRootFolder = checkingRootArt || !m_currentGame.has_value() ? m_pyWrightPath : m_currentGame->getGamePath();

    auto folderPath = selectedRootFolder / "art";

    if (!fs::exists(folderPath) || !fs::is_directory(folderPath))
        throw new std::runtime_error("art folder doesn\'t exist!");

    QStringList result;

    for (auto& dir_entry: fs::directory_iterator{folderPath})
    {
        bool unwanted = false;

        if (dir_entry.is_directory())
        {
            for (auto& limitFolder : m_limitToFolders)
            {
                if (dir_entry.path().filename().string() == limitFolder.toStdString())
                {
                    unwanted = true;
                    break;
                }
            }

            if (unwanted)
                continue;

            result.append(QString{dir_entry.path().stem().c_str()});
        }
    }

    return result;
}

void IconPickerDialog::refreshIconView()
{
    auto subfolderName = m_subfolderCombobox->currentText();

    bool checkingRootArt = m_globalFolderCheckbox->isChecked();

    auto selectedRootFolder = checkingRootArt || !m_currentGame.has_value() ? m_pyWrightPath : m_currentGame->getGamePath();

    auto folderPath = selectedRootFolder / "art" / subfolderName.toStdString();

    auto fsModel = new QFileSystemModel();
    fsModel->setIconProvider(new ThumbnailIconProvider());

    fsModel->setFilter(QDir::Files);
    fsModel->setNameFilters({"*.png", "*.jpg"});
    fsModel->setNameFilterDisables(false);

    m_iconsListView->setModel(fsModel);
    m_iconsListView->setRootIndex(fsModel->setRootPath(QString{folderPath.c_str()}));
}

void IconPickerDialog::handleAccept()
{
    auto indexes = m_iconsListView->selectionModel()->selectedIndexes();

    if (indexes.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Nothing has selected!");
        return;
    }

    auto name = m_iconsListView->model()->data(indexes[0], Qt::DisplayRole).toString();

    m_selectedIcon = "art/" + m_subfolderCombobox->currentText() + "/" + name;

    accept();
}

void IconPickerDialog::refreshSubfolders()
{
    m_artSubfolders = querySubfolders();
    m_subfolderCombobox->clear();
    m_subfolderCombobox->addItems(m_artSubfolders);
    refreshIconView();
}
