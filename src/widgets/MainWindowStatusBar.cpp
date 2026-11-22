#include "widgets/MainWindowStatusBar.hpp"

#include <QTextStream>
#include <QFile>

MainWindowStatusBar::MainWindowStatusBar(QWidget* parent)
    : QStatusBar(parent),
      m_lineColInfo(new QLabel()),
      m_selectionInfo(new QLabel()),
      m_pyWrightPathInfo(new QLabel("No PyWright Folder Selected"))
{
    setSizeGripEnabled(false);

    addPermanentWidget(m_selectionInfo);
    addPermanentWidget(m_lineColInfo);
    addPermanentWidget(m_pyWrightPathInfo);
    
    setContentsMargins(4, 2, 4, 2);
}

void MainWindowStatusBar::setSelectionLengthInfo(int selectionLength)
{
    if (selectionLength > 0)
    {
        QString selectionText;
        QTextStream(&selectionText) << selectionLength << " characters selected";

        m_selectionInfo->setText(selectionText);
    }
    else
        m_selectionInfo->setText("");
}

void MainWindowStatusBar::setPyWrightPathInfo(QString installationPath)
{
    QString verString = "";

    QFile dataFile{installationPath + "/data.txt"};

    if (dataFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&dataFile);

        while (!in.atEnd())
        {
            QString line = in.readLine();

            if (line.startsWith("version "))
            {
                verString = line.section(' ', 1);
                break;
            }
        }

        dataFile.close();
    }

    QString name = "PyWright";

    if (!verString.isEmpty())
        name += " (" + verString + ")";

    m_pyWrightPathInfo->setText(name);
    m_pyWrightPathInfo->setToolTip(installationPath);
}

void MainWindowStatusBar::setCursorPositionInfo(int line, int col)
{
    if (line > 0 && col > 0)
    {
        QString lineColText;
        QTextStream(&lineColText) << "Ln: " << line << " Col: " << col;

        m_lineColInfo->setText(lineColText);
    }
    else
        m_lineColInfo->setText("");
}