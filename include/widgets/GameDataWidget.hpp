#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <optional>

#include "data/PyWrightGameInfo.hpp"


class GameDataWidget : public QWidget
{
    Q_OBJECT

    public:
        GameDataWidget(QWidget* parent);

        void loadDataTxt(const PyWrightGameInfo& gameInfo);
        void populateGameInfo();

        void saveDataTxt();
        void setGameIconPathTextField(const QString& newPath);

        QString getGameTitle() const;
        fs::path getGameIconPath() const;
        QString getGameVersion() const;
        QString getGameAuthor() const;

        void setPyWrightPath(const fs::path& newPath);

        bool areDataTxtAreasDifferent() const;
        bool areDataTxtAreasEmpty() const;

    signals:
        void dataTxtFieldsChanged();
    
    private:
        void prepareMainLayout();
        void connectSignalsAndSlots();
    
        std::optional<PyWrightGameInfo> m_gameInfo;
        fs::path m_pyWrightPath;

        QLineEdit* m_gameTitleLineEdit;
        QLineEdit* m_gameVersionLineEdit;
        QLineEdit* m_gameAuthorLineEdit;
        QLineEdit* m_gameIconLineEdit;
        QPushButton* m_gameIconPickerButton;
    
    private slots:
        void handleDataTxtFieldsChange();
        void handleIconPickerClicked();
};