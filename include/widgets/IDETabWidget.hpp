#pragma once

#include <QWidget>

#include <cstdint>

enum class TabKind : uint8_t
{
    FileEditing,
    GameProperties,
    ImageViewer
};

class IDETabWidget : public QWidget
{
    public:
        IDETabWidget(TabKind kind, QWidget* parent) : QWidget(parent), m_kind(kind) {}
        TabKind getTabKind() const { return m_kind; }

    private:
        TabKind m_kind;
};
