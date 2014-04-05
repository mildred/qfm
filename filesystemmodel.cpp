#ifdef __linux
#include <sys/types.h>
#include <sys/xattr.h>
#endif

#include <QDebug>

#include "filesystemmodel.h"
#include "qfmdeclarations.h"

static QPoint deserialize_point(QByteArray value) {
    bool ok[2] = {false, false};
    int  xy[2] = {0, 0};
    int  n = 0;

    for(int i = 0; i < value.size() && n < 2; ++i) {
        QChar c(value[i]);
        if (c.isDigit()) {
            xy[n] = xy[n] * 10 + c.digitValue();
            ok[n] = true;
        } else {
            n++;
        }
    }

    if(!ok[0] || !ok[1]) return QPoint();

    return QPoint(xy[0], xy[1]);
}

FileSystemModel::FileSystemModel(QObject *parent) :
    QFileSystemModel(parent)
{
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    if(role == Qfm::PositionHintRole) {
#ifdef __linux
        QByteArray value(128, 0);
        QByteArray path = filePath(index).toLocal8Bit();
        int len = lgetxattr(path.constData(), "user.filemanager.position", value.data(), value.length());
        //qDebug() << "lgetxattr" << path << "user.filemanager.position" << value << "=" << len;
        if (len != -1) {
            QPoint pt = deserialize_point(value);
            if(!pt.isNull()) return pt;
        }
#endif
    }
    return QFileSystemModel::data(index, role);
}

bool FileSystemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qfm::PositionHintRole) {
        bool changed = false;
#ifdef __linux
        QByteArray path = filePath(index).toLocal8Bit();
        if (value.isValid()) {
            QPoint point = value.value<QPoint>();
            QByteArray pointSerialized = QByteArray::number(point.x()) + ' ' + QByteArray::number(point.y());
            int err = lsetxattr(path.constData(), "user.filemanager.position", pointSerialized, pointSerialized.size(), 0);
            //qDebug() << "lsetxattr" << path << "user.filemanager.position" << pointSerialized << "=" << err;
            changed = err != -1;
        } else {
            int err = lremovexattr(path.constData(), "user.filemanager.position");
            changed = err != -1;
        }
#endif
        if (changed) {
            emit dataChanged(index, index, QVector<int>(1) << role);
        }
    }
    return QFileSystemModel::setData(index, value, role);
}
