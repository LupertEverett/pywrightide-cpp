#pragma once

#include <QStatusBar>
#include <QLabel>

class MainWindowStatusBar : public QStatusBar
{
    public:
        MainWindowStatusBar(QWidget* parent = nullptr);

        void setSelectionLengthInfo(int selectionLength);
        void setPyWrightPathInfo(QString installationPath);
        void setCursorPositionInfo(int line, int col);
        
    private:
        QLabel* m_selectionInfo;
        QLabel* m_lineColInfo;
        QLabel* m_pyWrightPathInfo;
};