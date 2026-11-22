#include "data/IconThemes.hpp"
#include "data/IDESettings.hpp"

const auto iconThemesFolder = fs::path{"res/iconthemes"};

bool isIconThemeFolderValid(const std::string& themeFolderName)
{
    fs::path folder = iconThemesFolder;

    folder /= themeFolderName;

    if (!fs::exists(folder) || !fs::is_directory(folder))
        return false;

    for (auto icon_name : allIcons)
        if (!fs::exists(folder / icon_name) || !fs::is_regular_file(folder / icon_name))
            return false;

    return true;
}

bool isIconNameValid(const QString& iconName)
{
    for (const auto& icon : allIcons)
    {
        if (icon == iconName)
            return true;
    }

    return false;
}

QString iconPathFromTheme(const QString& iconName)
{
    if (!isIconNameValid(iconName))
        throw new std::runtime_error((QString{"Invalid icon name: "} + iconName).toStdString().c_str());

    auto settings = IDESettings::getSettings();

    const auto iconThemeName = settings.getIconTheme();

    auto fullPath = iconThemesFolder / iconThemeName.toStdString() / iconName.toStdString();

    if (!fs::exists(fullPath))
        throw new std::runtime_error((std::string{"Couldn\'t find icon: "} + fullPath.string()).c_str());

    return QString(fullPath.c_str());
}

void setNewIconTheme(const QString& newIconThemeName)
{
    if (!isIconThemeFolderValid(newIconThemeName.toStdString()))
        return;

    auto settings = IDESettings::getSettings();

    settings.setIconTheme(newIconThemeName);
}

std::vector<QString> queryIconThemes()
{
    std::vector<QString> result;

    for (const auto& dir_entry : fs::directory_iterator{iconThemesFolder})
    {
        auto path = dir_entry.path();

        if (fs::is_directory(path) && isIconThemeFolderValid(path.string()))
        {
            result.push_back(path.stem().c_str());
        }
    }

    return result;
}