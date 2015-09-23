#include "rsvgconvert_plugin.h"
#include "rsvgconvert.h"

#include <QtQml/qqml.h>

void PouarQuickPlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri);
    qmlRegisterSingletonType<PouarQuick>("PouarQuick", 1, 0, "PouarQuick", singleton);
}
void PouarQuickPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
    engine->addImageProvider("image", new QImageProvider);
}
