#ifndef RSVGCONVERT_H
#define RSVGCONVERT_H
#include <QObject>
#include <QString>
#include <QUrl>
#include <QDir>
#include <QImage>
#include <qqmlextensionplugin.h>

#include <qqmlengine.h>
#include <QtQuick/qquickimageprovider.h>
#include <QPainter>
extern "C" {
#include "rsvg-convert.h"
#include <bsd/bsd.h>
}

struct ImageBuffer {
    unsigned char* data;
    int width;
    int height;
    int stride;
};

class PouarQuick : public QObject
{
    Q_OBJECT
public:
    PouarQuick(QObject *parent = 0);
    ~PouarQuick();
    Q_INVOKABLE QImage getImage(QUrl url);
    Q_INVOKABLE QUrl randomfile(QUrl url);

public Q_SLOTS:

    
Q_SIGNALS:
private:
};
class QImageProvider : public QQuickImageProvider
{
public:
    QImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
};
#endif
extern QObject *singleton(QQmlEngine *engine, QJSEngine *scriptEngine);
