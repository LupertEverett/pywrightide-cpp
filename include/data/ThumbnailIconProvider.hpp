#pragma once

#include <QFileIconProvider>

class ThumbnailIconProvider : public QFileIconProvider
{
    public:
        ThumbnailIconProvider();

        QIcon icon(const QFileInfo& info) const override;
};