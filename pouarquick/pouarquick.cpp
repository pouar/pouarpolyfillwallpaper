#include "pouarquick.h"

PouarQuick::PouarQuick(QObject *parent) : QObject(parent)
{
}

QImageProvider::QImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{
}

QImage QImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
	Q_UNUSED(size);
	QUrl dir(id);
	RsvgDimensionData dimensions;
	int width = 1;
	int height = 1;
	/* Set the locale so that UTF-8 filenames work */
	setlocale(LC_ALL, "");
	double zoom = 1.0;
	rsvg_set_default_dpi_x_y (-1, -1);
	

	RsvgHandle *rsvg = rsvg_handle_new_from_file(dir.toLocalFile().toStdString().c_str(),NULL);
	if(rsvg!=NULL)
	{
		rsvg_handle_get_dimensions(rsvg, &dimensions);
		QRect desktop = QRect(0,0,requestedSize.width(),requestedSize.height());
		if(dimensions.width!=dimensions.height)
		{
			width = (dimensions.width<dimensions.height) ? (double)desktop.height() / (double)dimensions.height * dimensions.width : desktop.width() ;
			height = (dimensions.height<dimensions.width) ? (double)desktop.width() / (double)dimensions.width * dimensions.height : desktop.height() ;
			zoom = (width > height) ? (double)width / (double)dimensions.width : (double)height / (double)dimensions.height;
		}
		else if(desktop.width() > desktop.height())
		{
			width = desktop.height();
			height = desktop.height();
			zoom = (double)height / (double)dimensions.height;
		}
		else
		{
			width = desktop.width();
			height = desktop.width();
			zoom = (double)width / (double)dimensions.width;
		}
		qDebug() << width << "x" << height << ";" << dimensions.width << "x" << dimensions.height << ";" << zoom;
	}
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);

	cairo_t *cr = cairo_create(surface);
	cairo_scale(cr, zoom, zoom);
	
	rsvg_handle_render_cairo(rsvg, cr);
	QImage *source = new QImage(
		cairo_image_surface_get_data(surface),
		cairo_image_surface_get_width(surface),
		cairo_image_surface_get_height(surface),
		cairo_image_surface_get_stride(surface),
		QImage::Format_ARGB32_Premultiplied
	);
	QImage image(cairo_image_surface_get_width(surface), cairo_image_surface_get_height(surface), QImage::Format_ARGB32_Premultiplied);
	image.fill(qRgba(0, 0, 0, 0));
	QPainter painter(&image);
	painter.drawImage(QPoint(0, 0), *source);

	delete source;
	g_object_unref(rsvg);
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	return image;
}

QUrl PouarQuick::randomfile(QUrl url)
{
	QDir dir(url.toLocalFile());
	QFileInfoList dirs = QFileInfoList(dir.entryInfoList(QDir::Files));
	if(dirs.size()>0)
	{
		dir.setPath(dirs.at(arc4random_uniform(dirs.size())).filePath());
		url.setUrl("file://"+dir.absolutePath());
	}
	else
		url.setUrl("file:///dev/null");
	return url;
}

QObject *singleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
	Q_UNUSED(engine);
	Q_UNUSED(scriptEngine);
	PouarQuick *pouarquick = new PouarQuick();
	return pouarquick;
}
