#include "pouarquick.h"



PouarQuick::PouarQuick(QObject *parent) : QObject(parent)
{
}

PouarQuick::~PouarQuick()
{
}


QImageProvider::QImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{
}

QImage QImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
	Q_UNUSED(size);
	QUrl dir(id);
	ImageBuffer imagebuffer = runrsvg(
		requestedSize.width(),
		requestedSize.height(),
		dir.toLocalFile().toStdString().c_str()
	);
	QImage image(imagebuffer.data,imagebuffer.width,imagebuffer.height,imagebuffer.stride,QImage::Format_ARGB32_Premultiplied);
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