#include "data/IDESettings.hpp"

void IDESettings::saveSettings()
{
    m_settings->setValue(SETTINGS_VERSION_KEY, 1);
    m_settings->sync();
}

void IDESettings::resetSettings()
{
    m_settings->clear();
    m_settings->setValue(FONT_NAME_KEY, "Consolas");
    m_settings->setValue(FONT_SIZE_KEY, 10);
    m_settings->setValue(FONT_BOLD_KEY, true);
    m_settings->setValue(AUTOLOAD_LAST_GAME_KEY, false);
    m_settings->setValue(AUTOLOAD_LAST_GAME_PATH_KEY, "");
    m_settings->setValue(WINDOW_GEOMETRY_KEY, QVariant());
    m_settings->setValue(ICON_THEME_KEY, "default");
    m_settings->setValue(COLOR_THEME_KEY, "System Theme");
    m_settings->setValue(EDITOR_THEME_KEY, "default");
    m_settings->setValue(EDITOR_WORD_WRAP_KEY, true);
    m_settings->setValue(ENABLE_AUTOCOMPLETION_KEY, true);
    m_settings->setValue(AUTOCOMPLETION_THRESHOLD_KEY, 1);
    m_settings->setValue(HIGHLIGHT_MATCHING_TEXT_KEY, true);
    m_settings->setValue(HIGHLIGHT_FILL_RECT_KEY, true);
    m_settings->setValue(IMAGE_VIEWER_USE_CONTROL_TO_ZOOM_KEY, false);
    m_settings->setValue(SHOW_DIRECTORY_VIEW_KEY, true);
    m_settings->setValue(SHOW_ASSET_BROWSER_KEY, true);
    m_settings->setValue(SHOW_LOGGER_VIEW_KEY, false);
}

QStringList IDESettings::allKeys()
{
    return m_settings->allKeys();
}

QString IDESettings::getFontName() const
{
    return m_settings->value(FONT_NAME_KEY, "Consolas").toString();
}

void IDESettings::setFontName(const QString& newValue)
{
    m_settings->setValue(FONT_NAME_KEY, newValue);
}

int IDESettings::getFontSize() const
{
    return m_settings->value(FONT_SIZE_KEY, 10).toInt();
}

void IDESettings::setFontSize(const int newValue)
{
    m_settings->setValue(FONT_SIZE_KEY, newValue);
}

bool IDESettings::getFontBoldness() const
{
    return m_settings->value(FONT_BOLD_KEY, true).toBool();
}

void IDESettings::setFontBoldness(bool newValue)
{
    m_settings->setValue(FONT_BOLD_KEY, newValue);
}

bool IDESettings::getAutoloadLastGameCheck() const
{
    return m_settings->value(AUTOLOAD_LAST_GAME_KEY, true).toBool();
}

void IDESettings::setAutoloadLastGameCheck(bool newValue)
{
    m_settings->setValue(AUTOLOAD_LAST_GAME_KEY, newValue);
}

QString IDESettings::getAutoloadLastGamePath() const
{
    return m_settings->value(AUTOLOAD_LAST_GAME_PATH_KEY, "").toString();
}

void IDESettings::setAutoloadLastGamePath(const QString& newGamePath)
{
    m_settings->setValue(AUTOLOAD_LAST_GAME_PATH_KEY, newGamePath);
}

QByteArray IDESettings::getWindowGeometry() const
{
    return m_settings->value(WINDOW_GEOMETRY_KEY).toByteArray();
}

void IDESettings::setWindowGeometry(const QByteArray& newGeometry)
{
    m_settings->setValue(WINDOW_GEOMETRY_KEY, newGeometry);
}

QByteArray IDESettings::getWindowState() const
{
    return m_settings->value(WINDOW_STATE_KEY).toByteArray();
}

void IDESettings::setWindowState(const QByteArray& newGeometry)
{
    m_settings->setValue(WINDOW_STATE_KEY, newGeometry);
}

QString IDESettings::getIconTheme() const
{
    return m_settings->value(ICON_THEME_KEY, "default").toString();
}

void IDESettings::setIconTheme(const QString& newThemeName)
{
    if (newThemeName.isEmpty())
        throw new std::runtime_error((QString{"Invalid Icon Theme Name: "} + newThemeName).toStdString().c_str());
    m_settings->setValue(ICON_THEME_KEY, newThemeName);
}

QString IDESettings::getColorTheme() const
{
    return m_settings->value(COLOR_THEME_KEY, "System Theme").toString();
}

void IDESettings::setColorTheme(const QString& newValue)
{
    if (newValue.isEmpty())
        throw new std::runtime_error((QString{"Invalid Color Theme Name: "} + newValue).toStdString().c_str());
    m_settings->setValue(COLOR_THEME_KEY, newValue);
}

QString IDESettings::getEditorColorTheme() const
{
    return m_settings->value(EDITOR_THEME_KEY, "default").toString();
}

void IDESettings::setEditorColorTheme(const QString& newValue)
{
    if (newValue.isEmpty())
        throw new std::runtime_error((QString{"Invalid Editor Color Theme Name: "} + newValue).toStdString().c_str());
    m_settings->setValue(EDITOR_THEME_KEY, newValue);
}

bool IDESettings::getEditorEnableWordWrap() const
{
    return m_settings->value(EDITOR_WORD_WRAP_KEY, true).toBool();
}

void IDESettings::setEditorEnableWordWrap(bool newValue)
{
    m_settings->setValue(EDITOR_WORD_WRAP_KEY, newValue);
}

QStringList IDESettings::getRecentGames() const
{
    QStringList result;

    size_t size = m_settings->beginReadArray(RECENT_GAMES_KEY);

    for (size_t i = 0 ; i < size ; i++)
    {
        m_settings->setArrayIndex(i);
        result.push_back(m_settings->value("folder_path").toString());
    }

    m_settings->endArray();

    return result;
}

void IDESettings::setRecentGames(const QStringList& newGames)
{
    m_settings->beginWriteArray(RECENT_GAMES_KEY);

    for (size_t i = 0 ; i < newGames.size() ; i++)
    {
        m_settings->setArrayIndex(i);
        m_settings->setValue("folder_path", newGames[i]);
    }

    m_settings->endArray();
}

QStringList IDESettings::getLastOpenTabs() const
{
    QStringList result;

    size_t size = m_settings->beginReadArray(LAST_OPEN_TABS_KEY);

    for (size_t i = 0 ; i < size ; i++)
    {
        m_settings->setArrayIndex(i);
        result.push_back(m_settings->value("file_path").toString());
    }

    m_settings->endArray();

    return result;
}

void IDESettings::setLastOpenTabs(const QStringList& newTabs)
{
    m_settings->beginWriteArray(LAST_OPEN_TABS_KEY);

    for (size_t i = 0 ; i < newTabs.size() ; i++)
    {
        m_settings->setArrayIndex(i);
        m_settings->setValue("file_path", newTabs[i]);
    }

    m_settings->endArray();
}

int IDESettings::getLastOpenTabIndex() const
{
    return m_settings->value(LAST_OPEN_TAB_INDEX_KEY, 0).toInt();
}

void IDESettings::setLastOpenTabIndex(const int newValue)
{
    m_settings->setValue(LAST_OPEN_TAB_INDEX_KEY, newValue);
}

bool IDESettings::getEnableAutocompletion() const
{
    return m_settings->value(ENABLE_AUTOCOMPLETION_KEY, true).toBool();
}

void IDESettings::setEnableAutocompletion(bool newValue)
{
    m_settings->setValue(ENABLE_AUTOCOMPLETION_KEY, newValue);
}

int IDESettings::getAutocompletionTriggerThreshold() const
{
    return m_settings->value(AUTOCOMPLETION_THRESHOLD_KEY, 1).toInt();
}

void IDESettings::setAutoCompletionTriggerThreshold(const int newValue)
{
    m_settings->setValue(AUTOCOMPLETION_THRESHOLD_KEY, newValue);
}

bool IDESettings::getHighlightMatchingText() const
{
    return m_settings->value(HIGHLIGHT_MATCHING_TEXT_KEY, true).toBool();
}

void IDESettings::setHighlightMatchingText(bool newValue)
{
    m_settings->setValue(HIGHLIGHT_MATCHING_TEXT_KEY, newValue);
}
