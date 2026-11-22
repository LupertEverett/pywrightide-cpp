#pragma once

#include "data/PyWrightCase.hpp"
#include <filesystem>

#include <QString>
#include <QStringList>

#include <optional>
#include <map>

namespace fs = std::filesystem;

struct PyWrightGameData
{
    PyWrightGameData();
    PyWrightGameData(const QString& gameTitle, const QString& gameVersion, const QString& gameAuthor, const fs::path& gameIconPath);
    QString gameTitle;
    QString gameVersion;
    QString gameAuthor;
    fs::path gameIconPath;
};

class PyWrightGameInfo
{
    public:
        PyWrightGameInfo(
            const PyWrightGameData& gameData,
            const QStringList& gameCases,
            const fs::path& gamePath
        );
        PyWrightGameInfo(
            const QString& gameTitle, const QString& gameVersion, const QString& gameAuthor,
            const fs::path& gameIconPath, const QStringList& gameCases, const fs::path& gamePath
        );
        static bool isValidGameFolder(const fs::path& folderPath);
        static PyWrightGameInfo createNewGame(
            const fs::path& pyWrightFolderPath, const QString& gameFolderName, const QString& gameVersion,
            const QString& gameTitle, const QString& gameAuthor, const fs::path& gameIconPath
        );
        static PyWrightGameInfo loadFromFolder(const fs::path& folderPath);

        PyWrightGameData getGameDataInfo() const;
        void setGameDataInfo(const PyWrightGameData& newData);
        fs::path getPyWrightFolderPath() const;
        fs::path getGamePath() const;
        QStringList getGameCases() const;
        QStringList getBuiltinMacros() const;
        QStringList getGameMacros() const;
        std::map<QString, QStringList> getCaseMacros() const;

        void createNewCase(const PyWrightCase& caseInfo);
        void appendCase(const QString& caseName);
        void removeCase(const QString& caseName, bool alsoRemoveFolder);

        void moveCaseUp(const size_t index);
        void moveCaseDown(const size_t index);

        void writeDataTxt();
        void writeIntroTxt();

        void updateIntroTxt();

        QStringList checkForMissingCases();

        void parseBuiltinMacros();
        void parseGameMacros();
        void parseCaseMacros();

        QString getGameFolderName();

        void clear();
        void clearDataTxtFields();
        void clearCaseList();
    
    private:
        static PyWrightGameData loadDataTxt(const fs::path& gameFolderPath);
        static QStringList loadIntroTxt(const fs::path& gameFolderPath);
        static QStringList parseMacrosInFile(const fs::path& macroFileName);
        static QStringList parseMacrosInFolder(const fs::path& folder);
        
        PyWrightGameData m_GameData;
        QStringList m_GameCases;
        QStringList m_BuiltinMacros;
        QStringList m_GameMacros;
        std::map<QString, QStringList> m_caseMacros;
        fs::path m_GamePath;
        fs::path m_PyWrightFolderPath;
};

class CurrentPyWrightGame
{
    public:
        static CurrentPyWrightGame& getCurrentGame()
        {
            static CurrentPyWrightGame current;
            return current;
        }

        std::optional<PyWrightGameInfo> currentGame();
        fs::path currentPyWrightFolderPath();
        QString currentPyWrightExecutable();
        void setCurrentGame(const std::optional<PyWrightGameInfo>& newGameInfo);


    private:
        CurrentPyWrightGame() : m_CurrentGame(std::nullopt) {}
        std::optional<PyWrightGameInfo> m_CurrentGame;
};
