#include "data/ThumbnailIconProvider.hpp"

#include <QPixmap>

ThumbnailIconProvider::ThumbnailIconProvider()
    : QFileIconProvider()
{
}

QIcon ThumbnailIconProvider::icon(const QFileInfo& info) const
{
    QString fn = info.filePath();

    if (fn.endsWith(".png") || fn.endsWith(".jpg"))
    {
        QPixmap a(QSize(32, 32));
        a.load(fn);

        return QIcon(a);
    }

    return QFileIconProvider::icon(info);
}