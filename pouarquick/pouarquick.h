#ifndef POUARQUICK_H
#define POUARQUICK_H
#include <QDir>

#include <QtQuick/qquickimageprovider.h>
extern "C" {
	#include "rsvg-convert.h"
	#include <bsd/bsd.h>
}


class PouarQuick : public QObject
{
	Q_OBJECT
	public:
		PouarQuick(QObject *parent = 0);
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
class QImageProvider2 : public QQuickImageProvider
{
	public:
		QImageProvider2();
		QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
};
extern QObject *singleton(QQmlEngine *engine, QJSEngine *scriptEngine);
#endif
