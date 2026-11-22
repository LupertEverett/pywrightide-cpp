#pragma once

#include <QDialog>
#include <QStringList>
#include <QComboBox>
#include <QCheckBox>
#include <QListView>
#include <QBoxLayout>
#include <QMessageBox>
#include <QDialogButtonBox>

#include "data/PyWrightGameInfo.hpp"

class IconPickerDialog : public QDialog
{
    Q_OBJECT

    public:
        IconPickerDialog(const fs::path& pyWrightRootPath, std::optional<PyWrightGameInfo> selectedGameInfo, QWidget* parent = nullptr, const QStringList& limitToFolders = {});
        QString getSelectedIcon();
    
    private:
        QStringList querySubfolders();
        void connectSignalsAndSlots();
        void prepareMainLayout();

        QListView* m_iconsListView;
        QComboBox* m_subfolderCombobox;
        QCheckBox* m_globalFolderCheckbox;
        QStringList m_limitToFolders;
        QDialogButtonBox* m_buttonBox;
        QStringList m_artSubfolders;
        std::optional<PyWrightGameInfo> m_currentGame;
        fs::path m_pyWrightPath;
        QString m_selectedIcon;

    private slots:
        void refreshIconView();
        void handleAccept();
        void refreshSubfolders();
};