#include "data/PyWrightFolder.hpp"

bool isValidPyWrightFolder(const fs::path& selectedPath)
{
    return fs::exists(selectedPath) && fs::is_directory(selectedPath)
        && fs::exists(selectedPath / "core") && fs::is_directory(selectedPath / "core")
        && fs::exists(selectedPath / "art") && fs::is_directory(selectedPath / "art")
        && fs::exists(selectedPath / "games") && fs::is_directory(selectedPath / "games")
        && (
            fs::exists(selectedPath / "PyWright.exe") && fs::is_regular_file(selectedPath / "PyWright.exe")
            || fs::exists(selectedPath / "PyWright.py") && fs::is_regular_file(selectedPath / "PyWright.py")
        );
}

QString pickPyWrightExecutable(const fs::path& selectedPath)
{
    auto winPyWright = selectedPath / "PyWright.exe";
    auto pyPyWright = selectedPath / "PyWright.py";

    if (fs::exists(winPyWright) && fs::is_regular_file(winPyWright))
        return QString{winPyWright.c_str()};
    else if (fs::exists(pyPyWright) && fs::is_regular_file(pyPyWright))
        return QString{pyPyWright.c_str()};
    
    return "";
}