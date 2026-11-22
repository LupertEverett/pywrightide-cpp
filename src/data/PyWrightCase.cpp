#include "data/PyWrightCase.hpp"

#include <QFile>
#include <QTextStream>
#include <stdexcept>

PyWrightCase::PyWrightCase(
        const QString& caseName,
        const QString& initialScript,
        const QString& caseBackgroundName,
        int textboxLines,
        bool textboxWrap,
        const QStringList& initialEvidences
    ) : initialEvidenceList(initialEvidences), caseName(caseName), initialScriptName(initialScript),
        caseBackgroundName(caseBackgroundName), textboxLines(textboxLines), textboxWrap(textboxWrap)
{
}

QString PyWrightCase::generateCaseIntroTxt() const
{
    QString result;

    QTextStream str(&result);

    str << "set _textbox_wrap " << (textboxWrap ? "true" : "false") << '\n'
        << "set _textbox_lines " << QString::number(textboxLines) << '\n'
        << "include evidence\n";

    for (size_t i = 0 ; i < initialEvidenceList.size() ; i++)
        str << "addev " << initialEvidenceList[i] << '\n';

    str << "script " << initialScriptName;

    return result;
}

void PyWrightCase::readFromIntroTxt(const fs::path& caseFolderPath)
{
    if (!fs::exists(caseFolderPath) || !fs::is_directory(caseFolderPath))
        throw new std::runtime_error("Not a valid case folder path!");

    auto introTxt = caseFolderPath / "intro.txt";

    if (!fs::exists(introTxt) || !fs::is_regular_file(introTxt))
        throw new std::runtime_error("Missing intro.txt file!");

    QFile introFile(introTxt);

    if (!introFile.open(QIODevice::ReadOnly | QIODevice::Text))
        throw new std::runtime_error("Failed to open file");

    QTextStream in(&introFile);

    while (!in.atEnd())
    {
        QString currentLine = in.readLine();

        QStringList lineList = currentLine.split(u' ');

        if (lineList.size() < 2)
            continue;

        if (lineList[0] == "set")
        {
            if (lineList[1] == "_textbox_wrap")
                textboxWrap = lineList[2].toLower() == "true";
            else if (lineList[1] == "_textbox_lines")
                textboxLines = lineList[2].toInt();
        }
        else if (lineList[0] == "addev")
            initialEvidenceList.push_back(lineList[1].trimmed());
        else if (lineList[0] == "script")
            initialScriptName = lineList[1].trimmed();
    }

    introFile.close();
}

void PyWrightCase::readFromCaseScreenTxt(const fs::path& caseFolderPath)
{
    if (!fs::exists(caseFolderPath) || !fs::is_directory(caseFolderPath))
        throw new std::runtime_error("Not a valid case folder path!");

    auto caseScreenTxt = caseFolderPath / "case_screen.txt";

    if (!fs::exists(caseScreenTxt) || !fs::is_regular_file(caseScreenTxt))
        return;

    QFile caseScreenFile(caseScreenTxt);

    if (!caseScreenFile.open(QIODevice::ReadOnly | QIODevice::Text))
        throw new std::runtime_error("Failed to open file");

    QTextStream in(&caseScreenFile);

    auto line = in.readLine();

    caseScreenFile.close();

    if (!line.startsWith("bg"))
        throw new std::runtime_error("Invalid case_screen.txt file!");

    auto lineSplit = line.split(' ');

    if (lineSplit.size() == 2)
        caseBackgroundName = lineSplit[1].trimmed();
}

void PyWrightCase::updateIntroTxt(const fs::path &caseFolderPath)
{
    if (!fs::exists(caseFolderPath) || !fs::is_directory(caseFolderPath))
        throw new std::runtime_error("Invalid case folder");

    if (!fs::exists(caseFolderPath / "intro.txt") || !fs::is_regular_file(caseFolderPath / "intro.txt"))
        throw new std::runtime_error("Missing intro.txt file for the case");

    QFile introTxt(caseFolderPath / "intro.txt");

    QStringList allLines;

    if (introTxt.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&introTxt);
        allLines = in.readAll().split('\n');

        introTxt.close();
    }

    for (size_t idx = 0 ; idx < allLines.size() ; idx++)
    {
        auto line = allLines[idx];
        auto lineSplitted = line.split(' ');

        if (lineSplitted.size() <= 1)
            continue;

        // bool endsWithNewLine = line.endsWith('\n');

        if (lineSplitted[0] == "set")
        {
            if (lineSplitted[1] == "_textbox_wrap")
                allLines[idx] = "set _textbox_wrap " + QString{textboxWrap ? "true" : "false"};
            else if (lineSplitted[1] == "_textbox_lines")
                allLines[idx] = "set _textbox_lines " + QString::number(textboxLines);

            // if (endsWithNewLine)
            //     allLines[idx] += '\n';
        }
        else if (lineSplitted[0] == "script")
        {
            allLines[idx] = "script " + initialScriptName;

            // if (endsWithNewLine)
            //    allLines[idx] += '\n';
        }
    }

    // Write everything back to the file
    if (introTxt.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&introTxt);
        out << allLines.join('\n');

        introTxt.close();
    }
}

void PyWrightCase::updateCaseScreenTxt(const fs::path &caseFolderPath)
{
    if (!fs::exists(caseFolderPath) || !fs::is_directory(caseFolderPath))
        throw new std::runtime_error("Invalid case folder");

    QFile caseScreenTxt(caseFolderPath / "case_screen.txt");

    if (caseScreenTxt.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&caseScreenTxt);

        out << "bg " << caseBackgroundName;

        caseScreenTxt.close();
    }
}

PyWrightCase PyWrightCase::fromExistingCaseFolder(const fs::path& caseFolderPath)
{
    if (!fs::exists(caseFolderPath) || !fs::is_directory(caseFolderPath))
        throw new std::runtime_error("Not a valid case folder path!");

    PyWrightCase pwcase(QString{caseFolderPath.stem().c_str()}, "", "");
    pwcase.readFromIntroTxt(caseFolderPath);
    pwcase.readFromCaseScreenTxt(caseFolderPath);

    return pwcase;
}
