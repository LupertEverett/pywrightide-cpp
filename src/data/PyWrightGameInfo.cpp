#include "data/PyWrightGameInfo.hpp"
#include "data/PyWrightFolder.hpp"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMessageBox>
#include <stdexcept>


PyWrightGameData::PyWrightGameData()
    : gameTitle(""), gameVersion(""), gameAuthor(""), gameIconPath("")
{
}

PyWrightGameData::PyWrightGameData(const QString& gameTitle, const QString& gameVersion, const QString& gameAuthor, const fs::path& gameIconPath)
    : gameTitle(gameTitle), gameVersion(gameVersion), gameAuthor(gameAuthor), gameIconPath(gameIconPath)
{
}

PyWrightGameInfo::PyWrightGameInfo(
        const PyWrightGameData& gameData,
        const QStringList& gameCases,
        const fs::path& gamePath
    ) : m_GameData(gameData), m_GameCases(gameCases),
        m_BuiltinMacros({}), m_GameMacros({}), m_caseMacros({}),
        m_GamePath(gamePath), m_PyWrightFolderPath("")
{
    if (!m_GamePath.empty())
        m_PyWrightFolderPath = m_GamePath.parent_path().parent_path();
}

PyWrightGameInfo::PyWrightGameInfo(
        const QString& gameTitle, const QString& gameVersion, const QString& gameAuthor,
        const fs::path& gameIconPath, const QStringList& gameCases, const fs::path& gamePath
    ) : m_GameData(gameTitle, gameVersion, gameAuthor, gameIconPath),
        m_GameCases(gameCases), m_BuiltinMacros({}), m_GameMacros({}), m_caseMacros({}),
        m_GamePath(gamePath), m_PyWrightFolderPath("")
{
    if (!m_GamePath.empty())
        m_PyWrightFolderPath = m_GamePath.parent_path().parent_path();
}

bool PyWrightGameInfo::isValidGameFolder(const fs::path& folderPath)
{
    if (folderPath.stem().empty() || folderPath.parent_path().stem().string() != "games")
        return false;

    return isValidPyWrightFolder(folderPath.parent_path().parent_path());
}

PyWrightGameInfo PyWrightGameInfo::createNewGame(
        const fs::path& pyWrightFolderPath, const QString& gameFolderName, const QString& gameVersion,
        const QString& gameTitle, const QString& gameAuthor, const fs::path& gameIconPath
    )
{
    if (pyWrightFolderPath.empty())
        throw new std::runtime_error("PyWright folder path is empty!");

    if (gameFolderName.isEmpty())
        throw new std::runtime_error("Game folder name is empty!");

    auto gamePath = pyWrightFolderPath / "games" / gameFolderName.toStdString();

    if (fs::exists(gamePath) && fs::is_directory(gamePath))
        throw new std::runtime_error("Game folder already exists!");

    fs::create_directory(gamePath);

    // Also create the subfolders
    fs::create_directory(gamePath / "art");
    fs::create_directory(gamePath / "music");
    fs::create_directory(gamePath / "sfx");

    // Create the game info
    PyWrightGameInfo info(gameTitle, gameVersion, gameAuthor, gameIconPath, {}, gamePath);

    // Make it write the necessary files
    info.writeDataTxt();
    info.writeIntroTxt();

    return info;
}

PyWrightGameInfo PyWrightGameInfo::loadFromFolder(const fs::path &folderPath)
{
    if (folderPath.parent_path().stem() != "games")
        throw new std::runtime_error(folderPath.string() + " is not a valid PyWright game folder!");

    if (!isValidPyWrightFolder(folderPath.parent_path().parent_path()))
        throw new std::runtime_error(folderPath.parent_path().parent_path().string() + " is not a valid PyWright folder!");

    auto gameData = loadDataTxt(folderPath);
    auto gameCases = loadIntroTxt(folderPath);

    PyWrightGameInfo gameInfo(gameData, gameCases, folderPath);

    auto missingCases = gameInfo.checkForMissingCases();

    for (auto missingCase : missingCases)
    {
        QMessageBox::information(nullptr, "Missing case",
            "The case <b>" + missingCase + "</b> is missing! It will be deleted from the case list!",
            QMessageBox::Ok,
            QMessageBox::Ok
        );
    }

    gameInfo.parseBuiltinMacros();
    gameInfo.parseGameMacros();
    gameInfo.parseCaseMacros();

    return gameInfo;
}

PyWrightGameData PyWrightGameInfo::getGameDataInfo() const
{
    return m_GameData;
}

void PyWrightGameInfo::setGameDataInfo(const PyWrightGameData &newData)
{
    m_GameData = newData;
}

fs::path PyWrightGameInfo::getPyWrightFolderPath() const
{
    return m_PyWrightFolderPath;
}

fs::path PyWrightGameInfo::getGamePath() const
{
    return m_GamePath;
}

QStringList PyWrightGameInfo::getGameCases() const
{
    return m_GameCases;
}

QStringList PyWrightGameInfo::getBuiltinMacros() const
{
    return m_BuiltinMacros;
}

QStringList PyWrightGameInfo::getGameMacros() const
{
    return m_GameMacros;
}

std::map<QString, QStringList> PyWrightGameInfo::getCaseMacros() const
{
    return m_caseMacros;
}

void PyWrightGameInfo::createNewCase(const PyWrightCase &caseInfo)
{
    fs::path casePath = m_GamePath / caseInfo.caseName.toStdString();

    if (fs::exists(casePath) && fs::is_directory(casePath))
        throw new std::runtime_error("Case folder already exists!");

    fs::create_directory(casePath);

    // Create an empty evidence.txt and the initial script file
    fs::path initScriptPath = casePath / (caseInfo.initialScriptName + ".txt").toStdString();
    fs::path evidenceTxtPath = casePath / "evidence.txt";
    fs::path introTxtPath = casePath / "intro.txt";

    QFile initScript(initScriptPath);
    QFile evidenceScript(evidenceTxtPath);
    QFile introTxtFile(introTxtPath);

    // We just need to open them with WriteOnly flags to create em.
    bool ok = initScript.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!ok)
        throw new std::runtime_error("Error while trying to create the initial script");
    initScript.close();
    ok = evidenceScript.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!ok)
        throw new std::runtime_error("Error while trying to create evidence.txt");
    evidenceScript.close();

    // intro.txt must have some initial data, that'll be dealt from new_case parameter
    ok = introTxtFile.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!ok)
        throw new std::runtime_error("Error while trying to create evidence.txt");

    QTextStream introOut(&introTxtFile);
    introOut << caseInfo.generateCaseIntroTxt();

    introTxtFile.close();

    // Finally, append the new case at the end of the cases list
    m_GameCases.append(caseInfo.caseName);
    updateIntroTxt();
}

void PyWrightGameInfo::appendCase(const QString &caseName)
{
    m_GameCases.append(caseName);
}

void PyWrightGameInfo::removeCase(const QString &caseName, bool alsoRemoveFolder)
{
    auto index = m_GameCases.indexOf(caseName);

    if (index == -1)
        return;

    m_GameCases.remove(index);
    updateIntroTxt();

    if (alsoRemoveFolder)
    {
        fs::path caseFolder = m_GamePath / caseName.toStdString();

        if (fs::exists(caseFolder) && fs::is_directory(caseFolder))
            fs::remove_all(caseFolder);
    }
}

void PyWrightGameInfo::moveCaseUp(const size_t index)
{
    if (index == 0)
        return;

    m_GameCases.swapItemsAt(index - 1, index);
}

void PyWrightGameInfo::moveCaseDown(const size_t index)
{
    if (index >= m_GameCases.size() - 1)
        return;

    m_GameCases.swapItemsAt(index, index + 1);
}

void PyWrightGameInfo::writeDataTxt()
{
    QFile dataTxt(m_GamePath / "data.txt");

    if (!dataTxt.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&dataTxt);

    if (!m_GameData.gameTitle.isEmpty())
        out << "title " << m_GameData.gameTitle << '\n';
    if (!m_GameData.gameVersion.isEmpty())
        out << "version " << m_GameData.gameVersion << '\n';
    if (!m_GameData.gameAuthor.isEmpty())
        out << "author " << m_GameData.gameAuthor << '\n';
    if (!m_GameData.gameIconPath.empty())
        out << "icon " << m_GameData.gameIconPath.c_str();

    dataTxt.close();
}

void PyWrightGameInfo::writeIntroTxt()
{
    QFile introTxt(m_GamePath / "intro.txt");

    if (!introTxt.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&introTxt);

    out << "set _order_cases variable\n";
    for (size_t idx = 0 ; idx < m_GameCases.size() ; idx++)
        out << "set _case_" << idx + 1 << " " << m_GameCases[idx] << '\n';
    out << "casemenu";

    introTxt.close();
}

void PyWrightGameInfo::updateIntroTxt()
{
    QFile introTxt(m_GamePath / "intro.txt");

    QStringList allLines;

    if (introTxt.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&introTxt);

        allLines = in.readAll().split('\n');

        introTxt.close();
    }

    for (size_t idx = 0 ; idx < allLines.size() ; idx++)
    {
        if (allLines[idx].startsWith("set _order_cases variable"))
        {
            // Remove all cases
            while (allLines[idx + 1].startsWith("set _case"))
                allLines.remove(idx + 1);

            // Add them again
            for (size_t caseidx = 0 ; caseidx < m_GameCases.size() ; caseidx++)
            {
                QString newCaseLine;
                QTextStream caseLineStream(&newCaseLine);

                caseLineStream << "set _case_" << (caseidx + 1) << " " << m_GameCases[caseidx];

                allLines.insert(idx + caseidx + 1, newCaseLine);
            }
        }
    }

    // Write the updated lines to the file
    if (introTxt.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&introTxt);

        out << allLines.join('\n');

        introTxt.close();
    }
}

QStringList PyWrightGameInfo::checkForMissingCases()
{
    QStringList result;

    for (auto& case_ : m_GameCases)
    {
        auto path = m_GamePath / case_.toStdString();

        if (!fs::exists(path) || !fs::is_directory(path))
            result.append(QString{path.c_str()});
    }

    if (!result.isEmpty())
    {
        m_GameCases.removeIf([&result](auto& it) {
            return result.indexOf(it) != -1;
        });
    }

    return result;
}

void PyWrightGameInfo::parseBuiltinMacros()
{
    m_BuiltinMacros = parseMacrosInFolder(m_PyWrightFolderPath / "core" / "macros");
}

void PyWrightGameInfo::parseGameMacros()
{
    m_GameMacros = parseMacrosInFolder(m_GamePath);

    // If there is a macros.txt file inside the game's root folder, that too should be scanned
    if (fs::exists(m_GamePath / "macros.txt") && fs::is_regular_file(m_GamePath / "macros.txt"))
        m_GameMacros.append(parseMacrosInFile(m_GamePath / "macros.txt"));
}

void PyWrightGameInfo::parseCaseMacros()
{
    for (size_t idx = 0 ; idx < m_GameCases.size() ; idx++)
    {
        auto caseFolder = m_GamePath / m_GameCases[idx].toStdString();
        QStringList caseMacros = parseMacrosInFolder(caseFolder);

        // If there is a macros.txt file inside the case folder, that too should be scanned
        if (fs::exists(caseFolder / "macros.txt") && fs::is_regular_file(caseFolder / "macros.txt"))
            caseMacros.append(parseMacrosInFile(caseFolder / "macros.txt"));

        m_caseMacros[m_GameCases[idx]] = caseMacros;
    }
}

QString PyWrightGameInfo::getGameFolderName()
{
    return QString{m_GamePath.filename().c_str()};
}

void PyWrightGameInfo::clear()
{
    clearDataTxtFields();
    clearCaseList();
    m_BuiltinMacros.clear();
    m_GameMacros.clear();
}

void PyWrightGameInfo::clearDataTxtFields()
{
    m_GameData.gameTitle.clear();
    m_GameData.gameAuthor.clear();
    m_GameData.gameIconPath.clear();
    m_GameData.gameVersion.clear();
}

void PyWrightGameInfo::clearCaseList()
{
    m_GameCases.clear();
    m_caseMacros.clear();
}

PyWrightGameData PyWrightGameInfo::loadDataTxt(const fs::path& gameFolderPath)
{
    auto dataTxtPath = gameFolderPath / "data.txt";

    if (!fs::exists(dataTxtPath) || !fs::is_regular_file(dataTxtPath))
        throw new std::runtime_error(dataTxtPath.string() + " does not exist!");

    PyWrightGameData data;

    QFile dataTxt(dataTxtPath);

    if (dataTxt.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&dataTxt);

        while (!in.atEnd())
        {
            auto line = in.readLine().trimmed();

            auto index = line.indexOf(' ');

            if (index == -1)
                continue;

            auto firstParam = line.mid(0, index);
            auto secondParam = line.mid(index + 1);

            if (secondParam.isEmpty())
                continue;

            if (firstParam == "version")
                data.gameVersion = secondParam;
            else if (firstParam == "icon")
                data.gameIconPath = fs::path{secondParam.toStdString()};
            else if (firstParam == "title")
                data.gameTitle = secondParam;
            else if (firstParam == "author")
                data.gameAuthor = secondParam;
        }

        dataTxt.close();
    }

    return data;
}

QStringList PyWrightGameInfo::loadIntroTxt(const fs::path& gameFolderPath)
{
    auto introTxtPath = gameFolderPath / "intro.txt";

    if (!fs::exists(introTxtPath) || !fs::is_regular_file(introTxtPath))
        throw new std::runtime_error(introTxtPath.string() + " does not exist!");

    QStringList cases;

    QFile introTxt(introTxtPath);

    if (introTxt.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&introTxt);

        auto allLines = in.readAll().split('\n');

        for(auto& line: allLines)
        {
            if (!line.trimmed().startsWith("set _case_"))
                continue;
            
            // Now we should be only dealing with the lines that start with "set _case_"
            
            auto trimmedLine = line.trimmed();

            // Only splitting from the first two whitespaces (because of "set" and "_case_")
            // should be more than enough

            auto caseName = trimmedLine.section(' ', 2);

            if (caseName.isEmpty())
                continue;

            cases.append(caseName);
        }

        introTxt.close();
    }

    return cases;
}

QStringList PyWrightGameInfo::parseMacrosInFile(const fs::path& macroFileName)
{
    QStringList macrosList;

    QFile macrosFile(macroFileName);

    if (macrosFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&macrosFile);

        while (!in.atEnd())
        {
            QString line = in.readLine();

            if (!line.startsWith("macro"))
                continue;

            QStringList macroLine = line.split(' ');

            if (macroLine.size() != 2)
                continue;

            macrosList.append(macroLine[1].trimmed());
        }

        macrosFile.close();
    }

    return macrosList;
}

QStringList PyWrightGameInfo::parseMacrosInFolder(const fs::path &folder)
{
    if (!fs::exists(folder) || !fs::is_directory(folder))
        throw new std::runtime_error("Selected folder doesn\'t exist!");

    QStringList result;

    for (auto& dir_entry : fs::directory_iterator{folder})
    {
        auto path = dir_entry.path();

        if (path.extension() == ".mcro")
            result.append(parseMacrosInFile(path));
    }

    return result;
}

/* --- CurrentPyWrightGame functions --- */

std::optional<PyWrightGameInfo> CurrentPyWrightGame::currentGame()
{
    return m_CurrentGame;
}

fs::path CurrentPyWrightGame::currentPyWrightFolderPath()
{
    return m_CurrentGame.has_value() ? m_CurrentGame->getPyWrightFolderPath() : "";
}

QString CurrentPyWrightGame::currentPyWrightExecutable()
{
    return m_CurrentGame.has_value() ? pickPyWrightExecutable(m_CurrentGame->getPyWrightFolderPath()) : "";
}

void CurrentPyWrightGame::setCurrentGame(const std::optional<PyWrightGameInfo> &newGameInfo)
{
    m_CurrentGame = newGameInfo;
}
