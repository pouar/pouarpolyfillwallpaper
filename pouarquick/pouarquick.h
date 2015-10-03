#ifndef POUARQUICK_H
#define POUARQUICK_H
#define QT_NO_KEYWORDS
#include <locale.h>
#include <librsvg/rsvg.h>
#include <QDir>
#include <QPainter>
#include <QtQuick/qquickimageprovider.h>
#include <bsd/bsd.h>


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
extern QObject *singleton(QQmlEngine *engine, QJSEngine *scriptEngine);
#endif
