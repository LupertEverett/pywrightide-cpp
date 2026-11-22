#pragma once

#include <vector>
#include <QString>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// Icon names
const auto ICON_NAME_PYWRIGHT = "pwicon.png";
const auto ICON_NAME_NEW_GAME = "newgame.png";
const auto ICON_NAME_OPEN_GAME = "opengame.png";
const auto ICON_NAME_NEW_FILE = "newfile.png";
const auto ICON_NAME_OPEN_FILE = "openfile.png";
const auto ICON_NAME_SAVE_FILE = "save.png";
const auto ICON_NAME_FIND_REPLACE = "findreplace.png";
const auto ICON_NAME_RUN_PYWRIGHT = "runpywright.png";
const auto ICON_NAME_SETTINGS = "settings.png";
const auto ICON_NAME_ABOUT = "about.png";
const auto ICON_NAME_DIRECTORY_VIEW_TOGGLE = "directoryviewtoggle.png";
const auto ICON_NAME_ASSET_BROWSER_TOGGLE = "assetbrowsertoggle.png";
const auto ICON_NAME_LOGGER_TOGGLE = "loggertoggle.png";
const auto ICON_NAME_PLUS = "plus.png";
const auto ICON_NAME_DOUBLE_PLUS = "doubleplus.png";
const auto ICON_NAME_MINUS = "minus.png";
const auto ICON_NAME_UP_ARROW = "uparrow.png";
const auto ICON_NAME_DOWN_ARROW = "downarrow.png";
const auto ICON_NAME_REFRESH = "refresh.png";
const auto ICON_NAME_AUDIO_PLAY = "audio_play.png";
const auto ICON_NAME_AUDIO_STOP = "audio_stop.png";
const auto ICON_NAME_AUDIO_FILE = "audio_file.png";
const auto ICON_NAME_AUDIO_FILE_PLAYING = "audio_file_playing.png";
const auto ICON_NAME_MUSIC_FILE = "music_file.png";
const auto ICON_NAME_CHARACTER_VIEWER = "character_viewer.png";
const auto ICON_NAME_IMPORT_IMAGE_AS_CHARACTER = "import_image_as_character.png";
const auto ICON_NAME_IMPORT_FOLDER_AS_CHARACTER = "import_folder_as_character.png";
const auto ICON_NAME_UNDO = "undo.png";
const auto ICON_NAME_REDO = "redo.png";

const std::vector<std::string> allIcons = {
    ICON_NAME_PYWRIGHT, ICON_NAME_NEW_GAME, ICON_NAME_OPEN_GAME, ICON_NAME_NEW_FILE, ICON_NAME_OPEN_FILE,
    ICON_NAME_SAVE_FILE, ICON_NAME_FIND_REPLACE, ICON_NAME_RUN_PYWRIGHT, ICON_NAME_SETTINGS, ICON_NAME_ABOUT,
    ICON_NAME_DIRECTORY_VIEW_TOGGLE, ICON_NAME_ASSET_BROWSER_TOGGLE, ICON_NAME_LOGGER_TOGGLE, ICON_NAME_PLUS,
    ICON_NAME_DOUBLE_PLUS, ICON_NAME_MINUS, ICON_NAME_UP_ARROW, ICON_NAME_DOWN_ARROW, ICON_NAME_REFRESH,
    ICON_NAME_AUDIO_PLAY, ICON_NAME_AUDIO_STOP, ICON_NAME_AUDIO_FILE, ICON_NAME_AUDIO_FILE_PLAYING,
    ICON_NAME_MUSIC_FILE, ICON_NAME_CHARACTER_VIEWER, ICON_NAME_IMPORT_IMAGE_AS_CHARACTER,
    ICON_NAME_IMPORT_FOLDER_AS_CHARACTER, ICON_NAME_UNDO, ICON_NAME_REDO
};

QString iconPathFromTheme(const QString& iconName);

void setNewIconTheme(const QString& newIconThemeName);

std::vector<QString> queryIconThemes();