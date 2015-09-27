#include "pouarquick.h"



PouarQuick::PouarQuick(QObject *parent) : QObject(parent)
{
}


QImageProvider::QImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{
}

QImageProvider2::QImageProvider2() : QQuickImageProvider(QQuickImageProvider::Image)
{
}
int i = 0;
int j = 0;
QImage QImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
	if(i!=0)
		cleanup(0);
	Q_UNUSED(size);
	QUrl dir(id);
	i = runrsvg(
		requestedSize.width(),
		requestedSize.height(),
		dir.toLocalFile().toStdString().c_str(),0
	);
	QImage image(svgbuf(0),svgwidth(0),svgheight(0),svgstride(0),QImage::Format_ARGB32_Premultiplied);
	return image;
}
QImage QImageProvider2::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
	if(j!=0)
		cleanup(1);
	Q_UNUSED(size);
	QUrl dir(id);
	j = runrsvg(
		requestedSize.width(),
		requestedSize.height(),
		dir.toLocalFile().toStdString().c_str(),1
	);
	QImage image(svgbuf(1),svgwidth(1),svgheight(1),svgstride(1),QImage::Format_ARGB32_Premultiplied);
	return image;
}


QUrl PouarQuick::randomfile(QUrl url)
{
	QDir dir(url.toLocalFile());
	QFileInfoList dirs = QFileInfoList(dir.entryInfoList(QDir::Files));
	dir.setPath(dirs.at(arc4random_uniform(dirs.size())).filePath());
	url.setUrl(dir.absolutePath());
	return url;
}
QObject *singleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
	Q_UNUSED(engine)
	Q_UNUSED(scriptEngine)
	PouarQuick *pouarquick = new PouarQuick();
	return pouarquick;
}