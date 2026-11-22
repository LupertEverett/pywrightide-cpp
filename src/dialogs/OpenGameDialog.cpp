#include "dialogs/OpenGameDialog.hpp"
#include "data/IconThemes.hpp"


OpenGameDialog::OpenGameDialog(const fs::path& pyWrightRootDir, QWidget* parent)
    : QDialog(parent), m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok)),
      m_listView(new QListView), m_itemModel(new QStandardItemModel),
      m_selectedGamePath("")
{
    setWindowTitle("Pick a Game");
    
    m_listView->setModel(m_itemModel);
    m_listView->setIconSize(QSize(32, 32));

    m_okButton = m_buttonBox->button(QDialogButtonBox::Ok);
    m_okButton->setEnabled(m_listView->selectionModel()->selectedIndexes().size() > 0);

    connectSignalsAndSlots();
    prepareMainLayout();

    populateList(pyWrightRootDir);
}

fs::path OpenGameDialog::getSelectedGamePath()
{
    return m_selectedGamePath;
}

void OpenGameDialog::connectSignalsAndSlots()
{
    connect(m_listView, &QListView::clicked,
        this, &OpenGameDialog::handleListViewClick);
    connect(m_listView, &QListView::doubleClicked,
        this, &OpenGameDialog::handleListViewDoubleClick);
    connect(m_okButton, &QPushButton::pressed,
        this, &OpenGameDialog::handleAccept);
}

void OpenGameDialog::prepareMainLayout()
{
    auto mainLayout = new QVBoxLayout();

    mainLayout->addWidget(m_listView);
    mainLayout->addWidget(m_buttonBox);

    setLayout(mainLayout);
}

void OpenGameDialog::populateList(const fs::path& pyWrightRootDir)
{
    auto gamePath = pyWrightRootDir / "games";

    QStringList games;

    for (auto& dir_entry : fs::directory_iterator{gamePath})
    {
        if (dir_entry.is_directory())
            games.append(QString{dir_entry.path().stem().c_str()});
    }

    auto gameIconPath = iconPathFromTheme(ICON_NAME_OPEN_GAME);

    for (auto& game : games)
        addItemToModel(gameIconPath, game);
}

void OpenGameDialog::addItemToModel(const QString &iconPath, const QString &itemText)
{
    auto item = new QStandardItem(QIcon(iconPath), itemText);
    item->setEditable(false);
    m_itemModel->appendRow(item);
}

void OpenGameDialog::handleListViewClick()
{
    m_okButton->setEnabled(m_listView->selectionModel()->selectedIndexes().size() > 0);
}

void OpenGameDialog::handleListViewDoubleClick()
{
    if (m_listView->selectionModel()->selectedIndexes().size() > 0)
        handleAccept();
}

void OpenGameDialog::handleAccept()
{
    auto idx = m_listView->currentIndex().row();
    m_selectedGamePath = fs::path{m_itemModel->item(idx)->text().toStdString()};
    accept();
}
