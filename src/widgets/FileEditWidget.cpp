#include "widgets/FileEditWidget.hpp"

#include <QBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QTimer>
#include <QKeyEvent>

FileEditWidget::FileEditWidget(const fs::path& pyWrightDir, const fs::path& filePath, QWidget* parent)
    : IDETabWidget(TabKind::FileEditing, parent),
      m_scintilla(new ScintillaEdit()),
      m_pyWrightDir(pyWrightDir),
      m_filePath(filePath),
      m_fileFolder(filePath.parent_path()),
      m_fileName("New File"),
      m_caretPosition(std::make_pair(0, 0)),
      m_oldCursorPos(-1),
      m_isFileModified(false),
      m_isANewFile(filePath.empty())
{
    if (!m_isANewFile)
    {
        m_fileName = QString{m_filePath.filename().c_str()};
        loadFileContents(m_filePath);
    }

    m_scintilla->send(SCI_EMPTYUNDOBUFFER);

    connectSignalsAndSlots();
    prepareMainLayout();
}

void FileEditWidget::connectSignalsAndSlots()
{
    // TODO: Implement
    connect(m_scintilla, &ScintillaEdit::updateUi,
        this, &FileEditWidget::handleUpdateUI);
    connect(m_scintilla, &ScintillaEdit::savePointChanged,
        this, &FileEditWidget::handleSavePointChanged);
}

void FileEditWidget::prepareMainLayout()
{
    auto mainLayout = new QVBoxLayout();

    mainLayout->addWidget(m_scintilla);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(mainLayout);
}

void FileEditWidget::supplyBuiltinMacrosToLexer(const QStringList& macros)
{
    // TODO: Implement
}

void FileEditWidget::supplyGameMacrosToLexer(const QStringList& macros)
{
    // TODO: Implement
}

void FileEditWidget::supplyCaseMacrosToLexer(const QStringList &macros)
{
    // TODO: Implement
}

void FileEditWidget::supplyFontPropertiesToLexer(const QString &fontName, int fontSize, bool boldFont)
{
    // TODO: Implement
}

void FileEditWidget::supplyEditorColorThemeToLexer()
{
    // TODO: Implement
}

std::pair<Scintilla::sptr_t, Scintilla::sptr_t> FileEditWidget::getCursorPosition() const
{
    return m_caretPosition;
}

void FileEditWidget::SaveToFile()
{
    // TODO: Implement
    if (m_isANewFile)
    {
        auto newFileName = QFileDialog::getSaveFileName(this);

        if (newFileName.isEmpty())
            return;

        m_isANewFile = false;
        m_fileName = newFileName;
    }

    QFile outFile(m_filePath);

    if (outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&outFile);
                
        auto bytes = m_scintilla->getText(m_scintilla->textLength());

        out << QString::fromUtf8(bytes);

        m_scintilla->setSavePoint();
        m_isFileModified = false;

        outFile.close();
    }
}

void FileEditWidget::insertAtCursorPosition(const QString& text)
{
    m_scintilla->insertText(m_scintilla->currentPos(), text.toStdString().c_str());
}

bool FileEditWidget::isFileModified() const
{
    return m_isFileModified;
}

void FileEditWidget::setWordWrap(bool enable)
{
    m_scintilla->setWrapMode(enable ? SC_WRAP_WORD : SC_WRAP_NONE);
}

void FileEditWidget::setHighlightStyle(bool fill)
{
    // TODO: Implement
}

fs::path FileEditWidget::getFilePath()
{
    return m_filePath;
}

QString FileEditWidget::getFileName()
{
    return m_fileName;
}

size_t FileEditWidget::getSelectionLength()
{
    // TODO: Implement
    
    return 0;
}

bool FileEditWidget::canUndo()
{
    return m_scintilla->get_doc()->can_undo();
}

bool FileEditWidget::canRedo()
{
    return m_scintilla->get_doc()->can_redo();
}

void FileEditWidget::doUndo()
{
    m_scintilla->undo();
}

void FileEditWidget::doRedo()
{
    m_scintilla->redo();
}

void FileEditWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->keyCombination() == QKeyCombination(Qt::ControlModifier, Qt::Key_Z))
        doUndo();
    else if (event->keyCombination() == QKeyCombination(Qt::ControlModifier, Qt::Key_Y))
        doRedo();
    
    QWidget::keyPressEvent(event);
}

void FileEditWidget::loadFileContents(const fs::path& selectedFile)
{
    QFile readFile(selectedFile);

    if (readFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&readFile);

        m_scintilla->setText(in.readAll().toStdString().c_str());

        readFile.close();
    }
}

void FileEditWidget::emitFileModified()
{
    emit fileModifiedChanged(false);
}

void FileEditWidget::handleSavePointChanged(bool dirty)
{
    m_isFileModified = dirty;

    // Do not emit this signal immediately, otherwise you'll get a crash
    QTimer::singleShot(50, [this, dirty]() { emit fileModifiedChanged(dirty); });
}

void FileEditWidget::handleUpdateUI(Scintilla::Update update)
{
    auto newcursorPos = m_scintilla->currentPos();

    if (newcursorPos != m_oldCursorPos)
    {
        m_oldCursorPos = newcursorPos;

        auto line = m_scintilla->lineFromPosition(newcursorPos);
        auto column = m_scintilla->column(newcursorPos);

        m_caretPosition.first = line;
        m_caretPosition.second = column;

        emit cursorPositionChanged(line, column);
    }
}

/*
void FileEditWidget::handleCursorPositionChanged()
{
    // TODO: Implement
    auto line = m_scintilla->lineFromPosition(m_scintilla->currentPos());
    auto column = m_scintilla->column(m_scintilla->currentPos());

    m_caretPosition = {line, column};
}
*/

void FileEditWidget::handleSelectionChanged()
{
    // TODO: Implement
}

void FileEditWidget::handleTextChanged()
{
    // TODO: Implement
}
