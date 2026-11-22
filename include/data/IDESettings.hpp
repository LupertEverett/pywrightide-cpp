#pragma once

#include <QSettings>
#include <QString>
#include <QStringList>
#include <QByteArray>

// Key Names
const auto FONT_NAME_KEY = "editor/font/name";
const auto FONT_SIZE_KEY = "editor/font/size";
const auto FONT_BOLD_KEY = "editor/font/bold";
const auto AUTOLOAD_LAST_GAME_KEY = "autoload_last_game";
const auto AUTOLOAD_LAST_GAME_PATH_KEY = "last_game_path";
const auto WINDOW_GEOMETRY_KEY = "window_geometry";
const auto WINDOW_STATE_KEY = "window_state";
const auto ICON_THEME_KEY = "icon_theme";
const auto COLOR_THEME_KEY = "color_theme";
const auto EDITOR_THEME_KEY = "editor/color_theme";
const auto EDITOR_WORD_WRAP_KEY = "editor/enable_word_wrap";
const auto RECENT_GAMES_KEY = "recent_games";
const auto LAST_OPEN_TABS_KEY = "last_open_tabs";
const auto LAST_OPEN_TAB_INDEX_KEY = "last_open_tab_index";
const auto ENABLE_AUTOCOMPLETION_KEY = "enable_autocompletion";
const auto AUTOCOMPLETION_THRESHOLD_KEY = "autocompletion_suggestions_threshold";
const auto HIGHLIGHT_MATCHING_TEXT_KEY = "editor/highlight_matching_text";
const auto HIGHLIGHT_FILL_RECT_KEY = "editor/highlight_fill_rect";
const auto IMAGE_VIEWER_USE_CONTROL_TO_ZOOM_KEY = "image_viewer/use_control_to_zoom";
const auto SHOW_DIRECTORY_VIEW_KEY = "show_directory_view";
const auto SHOW_ASSET_BROWSER_KEY = "show_asset_browser";
const auto SHOW_LOGGER_VIEW_KEY = "show_logger_view";
const auto SETTINGS_VERSION_KEY = "settings_version";

class IDESettings
{
    public:
        IDESettings& operator =(const IDESettings&) = delete;

        static IDESettings& getSettings()
        {
            static IDESettings settings;
            return settings;
        }

        void saveSettings();
        void resetSettings();
        QStringList allKeys();
        
        QString getFontName() const;
        void setFontName(const QString& newValue);

        int getFontSize() const;
        void setFontSize(const int newValue);

        bool getFontBoldness() const;
        void setFontBoldness(bool newValue);

        bool getAutoloadLastGameCheck() const;
        void setAutoloadLastGameCheck(bool newValue);

        QString getAutoloadLastGamePath() const;
        void setAutoloadLastGamePath(const QString& newGamePath);

        QByteArray getWindowGeometry() const;
        void setWindowGeometry(const QByteArray& newGeometry);

        QByteArray getWindowState() const;
        void setWindowState(const QByteArray& newState);

        QString getIconTheme() const;
        void setIconTheme(const QString& newThemeName);

        QString getColorTheme() const;
        void setColorTheme(const QString& newValue);

        QString getEditorColorTheme() const;
        void setEditorColorTheme(const QString& newValue);

        bool getEditorEnableWordWrap() const;
        void setEditorEnableWordWrap(bool newValue);

        QStringList getRecentGames() const;
        void setRecentGames(const QStringList& newGames);

        QStringList getLastOpenTabs() const;
        void setLastOpenTabs(const QStringList& newTabs);

        int getLastOpenTabIndex() const;
        void setLastOpenTabIndex(const int newIndex);

        bool getEnableAutocompletion() const;
        void setEnableAutocompletion(bool newValue);

        int getAutocompletionTriggerThreshold() const;
        void setAutoCompletionTriggerThreshold(const int newValue);

        bool getHighlightMatchingText() const;
        void setHighlightMatchingText(bool newValue);

    private:
        QSettings* m_settings;

        IDESettings()
        {
            m_settings = new QSettings("PyWrightIDE", "PyWrightIDE");
        }
};