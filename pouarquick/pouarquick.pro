TEMPLATE = lib
TARGET = pouarquick

DEFINES += QT5_BUILD
QT += qml

CONFIG += qt plugin

QMAKE_EXT_CPP = .C
QMAKE_EXT_MOC = .C

TARGET = $$qtLibraryTarget($$TARGET)
uri = PouarQuick

# Input
SOURCES += \
	pouarquickplugin.C \
	pouarquick.C

HEADERS += \
	pouarquickplugin.h \
	pouarquick.h

QMAKE_CXXFLAGS += $(shell pkg-config --cflags librsvg-2.0 libbsd) -std=c++14
QMAKE_LIBS += $(shell pkg-config --libs librsvg-2.0 libbsd)
OTHER_FILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
	copy_qmldir.target = $$OUT_PWD/qmldir
	copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
	copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
	QMAKE_EXTRA_TARGETS += copy_qmldir
	PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir
