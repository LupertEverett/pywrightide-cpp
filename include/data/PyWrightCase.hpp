#pragma once

#include <filesystem>

#include <QString>
#include <QStringList>

namespace fs = std::filesystem;

class PyWrightCase
{
    public:
        PyWrightCase(const QString& caseName = "", const QString& initialScript = "scene1", const QString& caseBackgroundName = "", int textboxLines = 3, bool textboxWrap = true, const QStringList& initialEvidences = {});
        QString generateCaseIntroTxt() const;
        void readFromIntroTxt(const fs::path& caseFolderPath);
        void readFromCaseScreenTxt(const fs::path& caseFolderPath);
        void updateIntroTxt(const fs::path& caseFolderPath);
        void updateCaseScreenTxt(const fs::path& caseFolderPath);
        static PyWrightCase fromExistingCaseFolder(const fs::path& caseFolderPath);

        QStringList initialEvidenceList;
        QString caseName;
        QString initialScriptName;
        QString caseBackgroundName;
        int textboxLines = 3;
        bool textboxWrap = true;
};