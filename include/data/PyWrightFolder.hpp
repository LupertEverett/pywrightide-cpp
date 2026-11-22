#pragma once

#include <QString>

#include <filesystem>

namespace fs = std::filesystem;

bool isValidPyWrightFolder(const fs::path& selectedPath);
QString pickPyWrightExecutable(const fs::path& selectedPath);