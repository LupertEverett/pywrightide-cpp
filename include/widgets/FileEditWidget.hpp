#pragma once

#include <QFont>
#include <QFontMetrics>
#include <QString>

#include "widgets/IDETabWidget.hpp"
#include "ScintillaEdit.h"

#include <filesystem>

namespace fs = std::filesystem;

class FileEditWidget : public IDETabWidget
{
    Q_OBJECT

    public:
        FileEditWidget(const fs::path& pyWrightDir, const fs::path& filePath = "", QWidget* parent = nullptr);

        void supplyBuiltinMacrosToLexer(const QStringList& macros);
        void supplyGameMacrosToLexer(const QStringList& macros);
        void supplyCaseMacrosToLexer(const QStringList& macros);

        void supplyFontPropertiesToLexer(const QString& fontName, int fontSize, bool boldFont);
        void supplyEditorColorThemeToLexer();

        std::pair<Scintilla::sptr_t, Scintilla::sptr_t> getCursorPosition() const;

        void SaveToFile();
        void insertAtCursorPosition(const QString& text);
        bool isFileModified() const;
        void setWordWrap(bool enable);
        void setHighlightStyle(bool fill);
        fs::path getFilePath();
        QString getFileName();

        size_t getSelectionLength();

        bool canUndo();
        bool canRedo();
        void doUndo();
        void doRedo();

    signals:
        void fileNameChanged(const QString& newName);
        void fileModifiedChanged(bool modified);
        // void moveToTabRequested(const QString&, FindType);
        // void replaceNextInNextTabsRequested(const QString&, const QString&);
        void selectedTextChanged();
        void cursorPositionChanged(Scintilla::sptr_t line, Scintilla::sptr_t col);

    protected:
        void keyPressEvent(QKeyEvent* event) override;

    private:
        void connectSignalsAndSlots();
        void prepareMainLayout();
        void loadFileContents(const fs::path& selectedFile);
        void emitFileModified();

        ScintillaEdit* m_scintilla;
        QFont* m_font;
        QFontMetrics* m_fontMetrics;
        fs::path m_pyWrightDir;
        fs::path m_filePath;
        fs::path m_fileFolder;
        QString m_fileName;
        std::pair<Scintilla::sptr_t, Scintilla::sptr_t> m_caretPosition;
        long m_oldCursorPos;
        bool m_isFileModified;
        bool m_isANewFile;

    private slots:
        void handleSavePointChanged(bool dirty);
        void handleUpdateUI(Scintilla::Update update);
        // void handleCursorPositionChanged();
        void handleSelectionChanged();
        void handleTextChanged();
};
