#pragma once

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QIcon>
#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QListView>
#include <QPushButton>

#include <filesystem>

namespace fs = std::filesystem;

class OpenGameDialog : public QDialog
{
    Q_OBJECT

    public:
        OpenGameDialog(const fs::path& pyWrightRootDir, QWidget* parent = nullptr);
        fs::path getSelectedGamePath();

    private:
        void connectSignalsAndSlots();
        void prepareMainLayout();
        void populateList(const fs::path& pyWrightRootDir);
        void addItemToModel(const QString& iconPath, const QString& itemText);

        QDialogButtonBox* m_buttonBox;
        QListView* m_listView;
        QStandardItemModel* m_itemModel;
        QPushButton* m_okButton;
        fs::path m_selectedGamePath;

    private slots:
        void handleListViewClick();
        void handleListViewDoubleClick();
        void handleAccept();
};
