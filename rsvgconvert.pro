TEMPLATE = lib
TARGET = pouarquick

contains(QT_VERSION, ^5\\..\\..*) {
    DEFINES += QT5_BUILD
    QT += qml
} else {
    QT += declarative
}
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = PouarQuick

# Input
SOURCES += \
    rsvgconvert_plugin.cpp \
    rsvgconvert.cpp \
    rsvg-convert.c

HEADERS += \
    rsvgconvert_plugin.h \
    rsvgconvert.h \
    rsvg-convert.h

QMAKE_CFLAGS += $(shell pkg-config --cflags librsvg-2.0 gio-unix-2.0 libbsd) -std=c11 -march=native -O2 -flto=8 -finline-functions -fpredictive-commoning -ftree-partial-pre -fgcse-after-reload -fgcse-las -fmerge-all-constants -fira-hoist-pressure -fivopts -freorder-blocks-and-partition --param large-function-growth=0 --param inline-unit-growth=0 --param large-stack-frame-growth=0 --param max-inline-recursive-depth-auto=1000 --param max-inline-recursive-depth=1000
QMAKE_CXXFLAGS += -std=c++14  -march=native -O2 -flto=8 -finline-functions -fpredictive-commoning -ftree-partial-pre -fgcse-after-reload -fgcse-las -fmerge-all-constants -fira-hoist-pressure -fivopts -freorder-blocks-and-partition --param large-function-growth=0 --param inline-unit-growth=0 --param large-stack-frame-growth=0 --param max-inline-recursive-depth-auto=1000 --param max-inline-recursive-depth=1000
QMAKE_LIBS += $(shell pkg-config --libs librsvg-2.0 gio-unix-2.0 libbsd)
QMAKE_LFLAGS +=  -flto=8 -finline-functions -fpredictive-commoning -ftree-partial-pre -fgcse-after-reload -fgcse-las -fmerge-all-constants -fira-hoist-pressure -fivopts -freorder-blocks-and-partition --param large-function-growth=0 --param inline-unit-growth=0 --param large-stack-frame-growth=0 --param max-inline-recursive-depth-auto=1000 --param max-inline-recursive-depth=1000
OTHER_FILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}
